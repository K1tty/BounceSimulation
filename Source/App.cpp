#include "App.h"
#include <algorithm>
#include <print>
#include "SmoothedValue.h"
#include "ScopedStopwatch.h"

static const size_t WindowWidth = 1200;
static const size_t WindowHeight = 800;


bool CApp::Init()
{
	Simulation.Init(WindowWidth, WindowHeight);
	
	if (!Renderer.Init(WindowWidth, WindowHeight))
		return false;

	return true;
}

void CApp::Run()
{
	if (!Init())
		return;

	while (PollEvents())
	{
		Tick();
		Render();
	}
}

void CApp::DrawFPS()
{
	const bool UseSmoothing = true;
	
	const float FrameTime = Timer.GetTickTime();
	const float SmoothingFactor = UseSmoothing ? std::max(FrameTime, 0.1f) : 1.0f;

	static CSmoothedValue FPSSmoothed;
	FPSSmoothed.Update(Timer.GetFPS(), SmoothingFactor);

	static CSmoothedValue FrameTimeSmoothed;
	FrameTimeSmoothed.Update(1000 * FrameTime, SmoothingFactor);

	static CSmoothedValue UpdateTimeSmoothed;
	UpdateTimeSmoothed.Update(1000 * UpdateTime, SmoothingFactor);

	static CSmoothedValue RenderTimeSmoothed;
	RenderTimeSmoothed.Update(1000 * RenderTime, SmoothingFactor);

	const std::string Text = std::format("{} fps / {:.1f} ms (update: {:.1f} ms, render: {:.1f} ms)",
		static_cast<int>(FPSSmoothed.Get()),
		FrameTimeSmoothed.Get(),
		UpdateTimeSmoothed.Get(),
		RenderTimeSmoothed.Get()
	);

	//std::println("{} fps / {:.1f} ms (update: {:.1f} ms, render: {:.1f} ms)", static_cast<int>(Timer.GetFPS()), 1000 * FrameTime, 1000 * UpdateTime, 1000 * RenderTime);
	Renderer.DrawText(SVector{ 0, 0 }, Colors::White, Text.c_str());
}

void CApp::Tick()
{
	CScopedStopwatch _(UpdateTime);

	Timer.Tick();
	Simulation.Tick();
}

void CApp::Render()
{
	CScopedStopwatch _(RenderTime);

	Renderer.PreRender();
	
	Simulation.Render();

	if (LineGizmo)
		LineGizmo->Render();

	DrawFPS();

	Renderer.PostRender();
}

bool CApp::PollEvents()
{
	static bool IsWallGizmo = false;

	bool ShouldQuit = false;

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				ShouldQuit = true;
				break;

			case SDL_KEYDOWN:
				{
					const SDL_Keycode Key = event.key.keysym.sym;

					if (Key == SDLK_BACKSPACE)
					{
						Simulation.Reset();
					}
					else if (Key == SDLK_0)
					{
						Simulation.ToggleWallDestruction();
					}
					else if (Key == SDLK_1)
					{
						Simulation.Reset();
						Simulation.SpawnRandomWalls(100'000);
						Simulation.SpawnRandomBalls(1'000);
					}
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT)
				{
					IsWallGizmo = event.button.button == SDL_BUTTON_LEFT;

					const SVector Position =
					{
						.X = static_cast<float>(event.button.x),
						.Y = static_cast<float>(event.button.y)
					};

					LineGizmo = CLineGizmo();
					LineGizmo->SetStartPoint(Position);
					LineGizmo->SetEndPoint(Position);
				}
				break;

			case SDL_MOUSEMOTION:
				if (LineGizmo)
				{
					const SVector Position =
					{
						.X = static_cast<float>(event.motion.x),
						.Y = static_cast<float>(event.motion.y)
					};

					LineGizmo->SetEndPoint(Position);
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (LineGizmo && (event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT))
				{
					const SLine& Line = LineGizmo->GetLine();
					const SVector Direction = Line.P2 - Line.P1;
					const float DirectionLength = Length(Direction);

					if (DirectionLength > 10.0f)
					{
						if (IsWallGizmo)
						{
							CSimulation::SWall Wall{ Line };
							Simulation.SpawnWall(Wall);
						}
						else // is ball
						{
							CSimulation::SBall Ball;
							Ball.Position = Line.P1;
							Ball.Direction = Normalize(Direction);
							Ball.Speed = std::clamp(DirectionLength, 10.0f, 500.0f);

							Simulation.SpawnBall(Ball);
						}
					}

					LineGizmo.reset();
				}
				break;
		}
	}

	return !ShouldQuit;
}