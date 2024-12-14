#pragma once
#include <vector>
#include "Line.h"
#include "Vector.h"


class CSimulation
{
private:
	using SWall = SLine;

	struct SBullet
	{
		SVector Position;
		SVector Direction;
		float Speed;
	};

public:
	void Init(uint16_t Width, uint16_t Height);
	void Reset();

	void Update();
	void Render();

	void SpawnRandomWalls(size_t Count);
	void SpawnRandomBullets(size_t Count);

private:
	void InitBoundsWalls();

private:
	uint16_t Width;
	uint16_t Height;

	std::vector<SWall> Walls;
	std::vector<SBullet> Bullets;
};