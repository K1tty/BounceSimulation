#include "Simulation.h"
#include "Renderer.h"
#include "Timer.h"
#include "Rectangle.h"
#include "StrideView.h"
#include "Intersection.h"


void CSimulation::Init(uint16_t Width, uint16_t Height)
{
	this->Width = Width;
	this->Height = Height;

	InitBoundsWalls();
}

void CSimulation::Reset()
{
	Walls.clear();
	Bullets.clear();

	InitBoundsWalls();
}

void CSimulation::Update()
{
	for (SBullet& Bullet : Bullets)
	{
		const SVector OldPosition = Bullet.Position;
		Bullet.Position += Bullet.Direction * (Bullet.Speed * GTimer->GetTickTime());
		
		const SLine BulletTrajectory{ OldPosition, Bullet.Position };

		for (size_t i = 0; i < Walls.size(); ++i)
		{
			const SWall& Wall = Walls[i];

			const std::optional<SVector> IntersectionPoint = Intersection(BulletTrajectory, Wall);
			if (IntersectionPoint)
			{
				Bullet.Direction = Reflect(Bullet.Direction, Normalize(Normal(Wall.P2 - Wall.P1)));
				Bullet.Position = IntersectionPoint.value() + Bullet.Direction * 0.01f;

				const bool IsDynamicWall = (i >= 4);
				//if (IsDynamicWall)
				if (false)
				{
					std::swap(Walls[i], Walls.back());
					Walls.pop_back();
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

	if (!Bullets.empty())
	{
		const TStrideView<SVector> BulletsView(Bullets[0].Position, Bullets.size(), sizeof(SBullet));
		GRenderer->DrawPoints(BulletsView, Colors::Red);
	}

	std::string Text = std::format("Bullets: {} / Walls: {}", Bullets.size(), Walls.size() - 4);
	GRenderer->DrawText({0, 18}, Colors::White, Text.c_str());
}

void CSimulation::InitBoundsWalls()
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

		Wall.P1.X = static_cast<float>(rand() % Width);
		Wall.P1.Y = static_cast<float>(rand() % Height);
		Wall.P2.X = static_cast<float>(rand() % Width);
		Wall.P2.Y = static_cast<float>(rand() % Height);

		Walls.push_back(Wall);
	}
}

void CSimulation::SpawnRandomBullets(size_t Count)
{
	Bullets.reserve(Bullets.size() + Count);

	for (size_t i = 0; i < Count; ++i)
	{
		SBullet Bullet;

		Bullet.Position.X = static_cast<float>(rand() % Width);
		Bullet.Position.Y = static_cast<float>(rand() % Height);

		Bullet.Direction.X = static_cast<float>(rand() % Width - Width / 2);
		Bullet.Direction.Y = static_cast<float>(rand() % Height - Height / 2);
		Bullet.Direction = Normalize(Bullet.Direction);

		Bullet.Speed = static_cast<float>(100);

		Bullets.push_back(Bullet);
	}
}