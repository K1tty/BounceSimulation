template<typename T>
CQuadTree<T>::CQuadTree(const SRectangle& Bounds, size_t MaxElementsPerNode, const TOverlapPredicate& OverlapPredicate)
{
	this->MaxElementsPerNode = MaxElementsPerNode;
	this->OverlapPredicate = OverlapPredicate;
	this->RootNode.Bounds = Bounds;
}

template<typename TElement>
bool CQuadTree<TElement>::Contains(const TElement& Element) const
{
	return ElementIndices.find(Element) != ElementIndices.end();
}

template<typename TElement>
void CQuadTree<TElement>::AddToIndex(const TElement& Element)
{
	ElementIndices[Element] = Elements.size();
	Elements.push_back(Element);
}

template<typename TElement>
void CQuadTree<TElement>::RemoveFromIndex(const TElement& Element)
{
	const TElementIndex ElementIndex = ElementToIndex(Element);
	const TElementIndex LastIndex = Elements.size() - 1;

	if (ElementIndex != LastIndex)
	{
		const TElement& LastElement = Elements.back();

		RemoveFromNode(RootNode, LastElement);
		
		ElementIndices[LastElement] = ElementIndex;
		ElementIndices[Element] = Elements.size() - 1;

		Elements[ElementIndex] = LastElement;

		Elements.pop_back();
		ElementIndices.erase(Element);

		AddToNode(RootNode, LastElement);
	}
	else
	{
		Elements.pop_back();
		ElementIndices.erase(Element);
	}
}

template<typename TElement>
void CQuadTree<TElement>::ValidateNode(const SNode& Node) const
{
	if (Node.Childs)
	{
		for (const SNode& ChildNode : *Node.Childs)
			ValidateNode(ChildNode);
	}
	else if (Node.ElementIndices)
	{
		for (TElementIndex Index : *Node.ElementIndices)
		{
			if (Index >= Elements.size())
				__debugbreak();
		}
	}
}

template<typename TElement>
void CQuadTree<TElement>::Add(const TElement& Element)
{
	if (!Contains(Element))
	{
		AddToIndex(Element);
		AddToNode(RootNode, Element);
	}
}

template<typename TElement>
void CQuadTree<TElement>::Remove(const TElement& Element)
{
	if (Contains(Element))
	{
		RemoveFromNode(RootNode, Element);
		RemoveFromIndex(Element);
	}
}

template<typename TElement>
const CQuadTree<TElement>::TElements& CQuadTree<TElement>::Query() const
{
	return Elements;
}

template<typename TElement>
CQuadTree<TElement>::TElements CQuadTree<TElement>::Query(const TQueryPredicate& Predicate) const
{
	TElements Result;
	Result.reserve(MaxElementsPerNode);

	QueryNode(RootNode, Predicate, Result);

	return Result;
}

template<typename TElement>
void CQuadTree<TElement>::Render() const
{
	RenderNode(RootNode);
}

template<typename TElement>
CQuadTree<TElement>::TElementIndex CQuadTree<TElement>::GetIndex(const TElement& Element) const
{
	return ElementToIndex(Element);
}

template<typename TElement>
void CQuadTree<TElement>::AddToNode(SNode& Node, const TElement& Element)
{
	if (!OverlapsWithNode(Element, Node))
		return;

	if (Node.Childs)
	{
		for (SNode& ChildNode: *Node.Childs)
		{
			AddToNode(ChildNode, Element);
		};
	}
	else
	{
		if (!Node.ElementIndices)
			Node.ElementIndices = std::make_unique<SNode::TElementIndices>();

		const TElementIndex ElementIndex = ElementToIndex(Element);
		Node.ElementIndices->push_back(ElementIndex);

		if (Node.ElementIndices->size() >= MaxElementsPerNode)
			SplitNode(Node);
	}
}

template<typename TElement>
void CQuadTree<TElement>::RemoveFromNode(SNode& Node, const TElement& Element)
{
	if (!OverlapsWithNode(Element, Node))
		return;

	if (Node.Childs)
	{
		for (SNode& ChildNode : *Node.Childs)
			RemoveFromNode(ChildNode, Element);
	}
	else if (Node.ElementIndices)
	{
		const auto it = std::find(Node.ElementIndices->begin(), Node.ElementIndices->end(), ElementToIndex(Element));
		if (it != Node.ElementIndices->cend())
		{
			*it = Node.ElementIndices->back();
			Node.ElementIndices->pop_back();
		}
		
		if (Node.ElementIndices->empty())
			Node.ElementIndices.reset();
	}
}

template<typename TElement>
void CQuadTree<TElement>::QueryNode(const SNode& Node, const TQueryPredicate& Predicate, TElements& Result) const
{
	if (!Predicate(Node.Bounds))
		return;

	if (Node.Childs)
	{
		for (const SNode& ChildNode : *Node.Childs)
			QueryNode(ChildNode, Predicate, Result);
	}
	else if (Node.ElementIndices)
	{
		Result.resize(Result.size() + Node.ElementIndices->size());
		auto it = Result.end() - Node.ElementIndices->size();

		const auto TransformPredicate = std::bind(&CQuadTree<TElement>::IndexToElement, this, std::placeholders::_1);
		std::transform(Node.ElementIndices->cbegin(), Node.ElementIndices->cend(), it, TransformPredicate);
	}
}

template<typename TElement>
void CQuadTree<TElement>::SplitNode(SNode& Node)
{
	Node.Childs = std::make_unique<SNode::TChilds>();

	const float HalfWidth = Node.Bounds.Width / 2;
	const float HalfHeight = Node.Bounds.Height / 2;

	typename SNode::TChilds& ChildNodes = *Node.Childs;
	ChildNodes[0].Bounds = { .X = Node.Bounds.X, .Y = Node.Bounds.Y, .Width = HalfWidth, .Height = HalfHeight };
	ChildNodes[1].Bounds = { .X = Node.Bounds.X + HalfWidth, .Y = Node.Bounds.Y, .Width = HalfWidth, .Height = HalfHeight };
	ChildNodes[2].Bounds = { .X = Node.Bounds.X, .Y = Node.Bounds.Y + HalfHeight, .Width = HalfWidth, .Height = HalfHeight };
	ChildNodes[3].Bounds = { .X = Node.Bounds.X + HalfWidth, .Y = Node.Bounds.Y + HalfHeight, .Width = HalfWidth, .Height = HalfHeight };

	for (SNode& ChildNode: *Node.Childs)
	{
		for (const TElementIndex ElementIndex : *Node.ElementIndices)
		{
			const TElement& Element = IndexToElement(ElementIndex);
			AddToNode(ChildNode, Element);
		}
	}

	Node.ElementIndices.reset();
}

template<typename TElement>
void CQuadTree<TElement>::RenderNode(const SNode& Node) const
{
	if (Node.Childs)
	{
		for (const SNode& ChildNode : *Node.Childs)
			RenderNode(ChildNode);
	}
	else
	{
		SColor Color = Node.ElementIndices ? Colors::Blue : Colors::White;
		GRenderer->DrawRect(Node.Bounds, Color);
	}
}

template<typename TElement>
bool CQuadTree<TElement>::OverlapsWithNode(const TElement& Element, const SNode& Node) const
{
	return OverlapPredicate(Element, Node.Bounds);
}

template<typename TElement>
CQuadTree<TElement>::TElementIndex CQuadTree<TElement>::ElementToIndex(const TElement& Element) const
{
	return ElementIndices.find(Element)->second;
}

template<typename TElement>
const TElement& CQuadTree<TElement>::IndexToElement(TElementIndex Index) const
{
	return Elements[Index];
}