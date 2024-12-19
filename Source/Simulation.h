#pragma once
#include <vector>
#include "Line.h"
#include "Vector.h"


class CSimulation
{
public:
	using SWall = SLine;

	struct SBall
	{
		SVector Position;
		SVector Direction;
		float Speed;  // pixels per second
	};

public:
	void Init(uint16_t Width, uint16_t Height);
	void Reset();

	void Tick();
	void Render();

	void SpawnWall(const SWall& Wall);
	void SpawnBall(const SBall& Ball);

	void SpawnRandomWalls(size_t Count);
	void SpawnRandomBalls(size_t Count);

	void ToggleWallDestruction();

private:
	void SpawnBoundsWalls();

private:
	bool WallDestructionEnabled = false;

	uint16_t Width = 0;
	uint16_t Height = 0;

	std::vector<SWall> Walls;
	std::vector<SBall> Balls;
};