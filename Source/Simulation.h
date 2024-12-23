#pragma once
#include <mutex>
#include <vector>
#include "Line.h"
#include "Vector.h"


class CSimulation
{
public:
	struct SWall
	{
		SLine Line;
		bool Touched = false;
	};

	struct SBall
	{
		SVector Position;
		SVector Direction;
		float Speed;  // pixels per second
	};

private:
	struct SWallIntersection
	{
		size_t WallIndex;
		SVector IntersectionPoint;
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
	const size_t BoundsWallsCount = 4;
	bool WallDestructionEnabled = false;

	uint16_t Width = 0;
	uint16_t Height = 0;

	std::vector<SWall> Walls;
	std::vector<SBall> Balls;

	std::mutex WallsMutex;
	std::mutex BallsMutex;
};