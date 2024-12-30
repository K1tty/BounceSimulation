#pragma once
#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include "Rectangle.h"
#include "Renderer.h"


template<typename TElement>
class CQuadTree
{
private:
	using TOverlapPredicate = std::function<bool(const TElement&, const SRectangle&)>;
	using TQueryPredicate = std::function<bool(const SRectangle&)>;
	using TElements = std::vector<TElement>;
	using TElementIndex = size_t;

	struct SNode
	{
		using TChilds = std::array<SNode, 4>;
		using TElementIndices = std::vector<TElementIndex>;

		SRectangle Bounds;
		std::unique_ptr<TChilds> Childs;
		std::unique_ptr<TElementIndices> ElementIndices;
	};

public:
	CQuadTree(const SRectangle& Bounds, size_t MaxElementsPerNode, const TOverlapPredicate& OverlapPredicate);

	void Add(const TElement& Element);
	void Remove(const TElement& Element);

	const TElements& Query() const;
	TElements Query(const TQueryPredicate& Predicate) const;

	TElementIndex GetIndex(const TElement& Element) const;

	void Render() const;

private:
	void ValidateNode(const SNode& Node) const;

	void AddToNode(SNode& Node, const TElement& Element);
	void RemoveFromNode(SNode& Node, const TElement& Element);
	void QueryNode(const SNode& Node, const TQueryPredicate& Predicate, TElements& Result) const;
	void SplitNode(SNode& Node);
	void RenderNode(const SNode& Node) const;
	bool OverlapsWithNode(const TElement& Element, const SNode& Node) const;

	bool Contains(const TElement& Element) const;
	void AddToIndex(const TElement& Element);
	void RemoveFromIndex(const TElement& Element);

	TElementIndex ElementToIndex(const TElement& Element) const;
	const TElement& IndexToElement(TElementIndex Index) const;

private:
	size_t MaxElementsPerNode;
	TOverlapPredicate OverlapPredicate;

	TElements Elements;
	std::unordered_map<TElement, TElementIndex> ElementIndices;

	SNode RootNode;
};


#include "QuadTree.inl"