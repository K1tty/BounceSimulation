#include "Simulation.h"
#include <algorithm>
#include <execution>
#include <set>
#include "Renderer.h"
#include "Timer.h"
#include "Rectangle.h"
#include "StrideView.h"
#include "Intersection.h"
#include "Random.h"

// Test case: 100'000 walls/1000 Balls
// Single-threaded Naive: 14.0 ms
// Multi-threaded Naive: 4.0 ms
// Single-threaded QuadTree: ?
// Multi-threaded QuadTree: ?
// GPU: ?


void CSimulation::Init(uint16_t Width, uint16_t Height)
{
	this->Width = Width;
	this->Height = Height;

	SpawnBoundsWalls();
}

void CSimulation::Reset()
{
	Walls.clear();
	Balls.clear();

	SpawnBoundsWalls();
}

void CSimulation::Tick()
{
	std::mutex TouchedWallsMutex;
	std::set<size_t, std::greater<size_t>> TouchedWalls;

	std::for_each(std::execution::par, Balls.begin(), Balls.end(), [&, this](SBall& Ball)
	{
		const SVector OldPosition = Ball.Position;
		Ball.Position += Ball.Direction * (Ball.Speed * GTimer->GetTickTime());
		
		const SLine BallTrajectory{ OldPosition, Ball.Position };

		for (size_t i = 0; i < Walls.size(); ++i)
		{
			SWall& Wall = Walls[i];

			const std::optional<SVector> IntersectionPoint = Intersection(BallTrajectory, Wall.Line);
			if (IntersectionPoint)
			{
				Ball.Direction = Reflect(Ball.Direction, Normalize(Normal(Wall.Line.P2 - Wall.Line.P1)));
				Ball.Position = IntersectionPoint.value() + Ball.Direction * 0.01f;

				const bool IsDynamicWall = (i >= 4);
				if (IsDynamicWall)
				{
					std::lock_guard lock(TouchedWallsMutex);
					TouchedWalls.insert(i);
				}

				break;
			}
		}
	});

	if (WallDestructionEnabled)
	{
		size_t SwapIndex = Walls.size() - 1;
		for (const size_t i : TouchedWalls)
		{
			std::swap(Walls[i], Walls[SwapIndex]);
			--SwapIndex;
		}

		Walls.resize(Walls.size() - TouchedWalls.size());
	}
}

void CSimulation::Render()
{
	if (Walls.size() > BoundsWallsCount)
	{
		const TStrideView<SLine> WallsView(Walls[BoundsWallsCount].Line, Walls.size() - BoundsWallsCount, sizeof(SWall));
		GRenderer->DrawLines(WallsView, Colors::Green);
	}

	if (!Balls.empty())
	{
		const TStrideView<SVector> BulletsView(Balls[0].Position, Balls.size(), sizeof(SBall));
		GRenderer->DrawPoints(BulletsView, Colors::Red);
	}

	std::string Text = std::format("Balls: {} / Walls: {}", Balls.size(), Walls.size() - BoundsWallsCount);
	GRenderer->DrawText({0, 18}, Colors::White, Text.c_str());
}

void CSimulation::SpawnWall(const SWall& Wall)
{
	std::lock_guard lock(WallsMutex);
	Walls.push_back(Wall);
}

void CSimulation::SpawnBall(const SBall& Ball)
{
	std::lock_guard lock(BallsMutex);
	Balls.push_back(Ball);
}

void CSimulation::SpawnBoundsWalls()
{
	const SRectangle Bounds(0, 0, static_cast<float>(Width), static_cast<float>(Height));

	SpawnWall({ Bounds.Top() });
	SpawnWall({ Bounds.Bottom() });
	SpawnWall({ Bounds.Left() });
	SpawnWall({ Bounds.Right() });
}

void CSimulation::SpawnRandomWalls(size_t Count)
{
	{
		std::lock_guard lock(WallsMutex);
		Walls.reserve(Walls.size() + Count + BoundsWallsCount);
	}

	for (size_t i = 0; i < Count; ++i)
	{
		SWall Wall;

		Wall.Line.P1.X = Random(Width);
		Wall.Line.P1.Y = Random(Height);
		Wall.Line.P2.X = Random(Width);
		Wall.Line.P2.Y = Random(Height);

		SpawnWall(Wall);
	}
}

void CSimulation::SpawnRandomBalls(size_t Count)
{
	{
		std::lock_guard lock(BallsMutex);
		Balls.reserve(Balls.size() + Count);
	}

	for (size_t i = 0; i < Count; ++i)
	{
		SBall Ball;

		Ball.Position.X = Random(Width);
		Ball.Position.Y = Random(Height);

		Ball.Direction.X = Random(-1.0f, 1.0f);
		Ball.Direction.Y = Random(-1.0f, 1.0f);
		Ball.Direction = Normalize(Ball.Direction);

		Ball.Speed = 100.0f;

		SpawnBall(Ball);
	}
}

void CSimulation::ToggleWallDestruction()
{
	WallDestructionEnabled = !WallDestructionEnabled;
}
