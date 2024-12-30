#include "Simulation.h"
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

std::optional<CSimulation::SWallIntersection> CSimulation::GetWallIntersection(const SLine& Trajectory) const
{
	std::optional<SWallIntersection> Result = std::nullopt;
	float MinDistance = std::numeric_limits<float>().max();

	for (size_t i = 0; i < Walls.size(); ++i)
	{
		const SWall& Wall = Walls[i];
		if (const std::optional<SVector> IntersectionPoint = Intersection(Trajectory, Wall.Line))
		{
			const float Distance = LengthSquared(*IntersectionPoint - Trajectory.P1);
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				Result = SWallIntersection
				{
					.Wall = Wall,
					.WallIndex = i,
					.IntersectionPoint = IntersectionPoint.value()
				};
			}
		}
	}

	return Result;
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

		const std::optional<SWallIntersection> WallIntersection = GetWallIntersection(BallTrajectory);
		if (WallIntersection)
		{
			const SWall& Wall = WallIntersection->Wall;

			Ball.Direction = Reflect(Ball.Direction, Normalize(Normal(Wall.Line.P2 - Wall.Line.P1)));
			Ball.Position = WallIntersection->IntersectionPoint + Ball.Direction * 0.01f;

			const bool IsDynamicWall = (WallIntersection->WallIndex >= 4);
			if (IsDynamicWall)
			{
				std::lock_guard lock(TouchedWallsMutex);
				TouchedWalls.insert(WallIntersection->WallIndex);
			}
		}
	});

	if (WallDestructionEnabled)
	{
		size_t LastIndex = Walls.size() - 1;
		for (const size_t WallIndex : TouchedWalls)
		{
			Walls[WallIndex] = Walls[LastIndex];
			--LastIndex;
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
	Walls.push_back(Wall);
}

void CSimulation::SpawnBall(const SBall& Ball)
{
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
	Walls.reserve(Walls.size() + Count);

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
	Balls.reserve(Balls.size() + Count);

	for (size_t i = 0; i < Count; ++i)
	{
		SBall Ball;

		Ball.Position.X = Random(Width);
		Ball.Position.Y = Random(Height);

		Ball.Direction.X = Random(-1.0f, 1.0f);
		Ball.Direction.Y = Random(-1.0f, 1.0f);
		Ball.Direction = Normalize(Ball.Direction);

		Ball.Speed = 250.0f;

		SpawnBall(Ball);
	}
}

void CSimulation::ToggleWallDestruction()
{
	WallDestructionEnabled = !WallDestructionEnabled;
}
