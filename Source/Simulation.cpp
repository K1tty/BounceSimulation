#include "Simulation.h"
#include "Renderer.h"
#include "Timer.h"
#include "Rectangle.h"
#include "StrideView.h"
#include "Intersection.h"
#include "Random.h"

// Test case: 100'000 walls/1000 Balls
// Single-threaded Naive: 16.7 ms
// Multi-threaded Naive: ?
// Single-threaded QuadTree: ?
// Multi-threaded QuadTree: ?
// Single-threaded BVH: ?
// Multi-threaded BVH: ?
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
	for (SBall& Ball : Balls)
	{
		const SVector OldPosition = Ball.Position;
		Ball.Position += Ball.Direction * (Ball.Speed * GTimer->GetTickTime());
		
		const SLine BallTrajectory{ OldPosition, Ball.Position };

		for (size_t i = 0; i < Walls.size(); ++i)
		{
			const SWall& Wall = Walls[i];

			const std::optional<SVector> IntersectionPoint = Intersection(BallTrajectory, Wall);
			if (IntersectionPoint)
			{
				Ball.Direction = Reflect(Ball.Direction, Normalize(Normal(Wall.P2 - Wall.P1)));
				Ball.Position = IntersectionPoint.value() + Ball.Direction * 0.01f;

				if (WallDestructionEnabled)
				{
					const bool IsDynamicWall = (i >= 4);
					if (IsDynamicWall)
					{
						std::swap(Walls[i], Walls.back());
						Walls.pop_back();
					}
				}

				break;
			}
		}
	}
}

void CSimulation::Render()
{
	if (Walls.size() > 4)
	{
		const TStrideView<SLine> WallsView(Walls[4], Walls.size() - 4, sizeof(SWall));
		GRenderer->DrawLines(WallsView, Colors::Green);
	}

	if (!Balls.empty())
	{
		const TStrideView<SVector> BulletsView(Balls[0].Position, Balls.size(), sizeof(SBall));
		GRenderer->DrawPoints(BulletsView, Colors::Red);
	}

	std::string Text = std::format("Balls: {} / Walls: {}", Balls.size(), Walls.size() - 4);
	GRenderer->DrawText({0, 18}, Colors::White, Text.c_str());
}

void CSimulation::SpawnWall(const SWall& Wall)
{
	Walls.push_back(Wall);
}

void CSimulation::SpawnBall(const SBall& Ball)
{
	Balls.push_back(Ball);
}

void CSimulation::SpawnBoundsWalls()
{
	const SRectangle Bounds(0, 0, static_cast<float>(Width), static_cast<float>(Height));

	Walls.push_back(Bounds.Top());
	Walls.push_back(Bounds.Bottom());
	Walls.push_back(Bounds.Left());
	Walls.push_back(Bounds.Right());
}

void CSimulation::SpawnRandomWalls(size_t Count)
{
	Walls.reserve(Walls.size() + Count);

	for (size_t i = 0; i < Count; ++i)
	{
		SWall Wall;

		Wall.P1.X = Random(Width);
		Wall.P1.Y = Random(Height);
		Wall.P2.X = Random(Width);
		Wall.P2.Y = Random(Height);

		SpawnWall(Wall);
	}
}

void CSimulation::SpawnRandomBalls(size_t Count)
{
	Balls.reserve(Balls.size() + Count);

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
