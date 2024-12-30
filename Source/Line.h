#pragma once
#include <type_traits>
#include "Vector.h"


struct SLine
{
	SVector P1;
	SVector P2;
};


inline bool operator==(const SLine& Line1, const SLine& Line2)
{
	return Line1.P1 == Line2.P1 && Line1.P2 == Line2.P2;
}

namespace std
{
	template<>
	struct hash<SLine>
	{
		size_t operator()(const SLine& Line) const
		{
			return std::hash<SVector>()(Line.P1) ^ (std::hash<SVector>()(Line.P2) << 1);
		}
	};
}