#pragma once
#include <vector>
#include "Line.h"
#include "Vector.h"


class CSimulation
{
private:
	using SWall = SLine;

	struct SBall
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
	void SpawnRandomBalls(size_t Count);

	void ToggleWallDestruction();

private:
	void InitBoundsWalls();

private:
	uint16_t Width = 0;
	uint16_t Height = 0;

	bool WallDestructionEnabled = false;

	std::vector<SWall> Walls;
	std::vector<SBall> Balls;
};