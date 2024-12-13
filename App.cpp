#include "App.h"
#include "SmoothedValue.h"
#include "ScopedStopwatch.h"
#include <print>

static const size_t WindowWidth = 1200;
static const size_t WindowHeight = 800;


bool CApp::Init()
{
	srand(0);

	Simulation.Init(WindowWidth, WindowHeight);
	
	if (!Renderer.Init(WindowWidth, WindowHeight))
		return false;

	Renderer.SetKeyDownCallback(std::bind(&CApp::OnKeyDown, this, std::placeholders::_1));

	return true;
}

void CApp::Run()
{
	if (!Init())
		return;

	while (!Renderer.ShouldQuit())
	{
		Update();
		Render();
	}
}

void CApp::DrawFPS()
{
	const float FrameTime = Timer.GetTickTime();
	const float SmoothingFactor = std::max(FrameTime, 0.1f);

	static CSmoothedValue FPSSmoothed;
	FPSSmoothed.Update(Timer.GetFPS(), SmoothingFactor);

	static CSmoothedValue FrameTimeSmoothed;
	FrameTimeSmoothed.Update(1000 * FrameTime, SmoothingFactor);

	static CSmoothedValue UpdateTimeSmoothed;
	UpdateTimeSmoothed.Update(1000 * UpdateTime, SmoothingFactor);

	static CSmoothedValue RenderTimeSmoothed;
	RenderTimeSmoothed.Update(1000 * RenderTime, SmoothingFactor);

	const std::string Text = std::format("{} fps / {:.1f} ms (update: {:.1f} ms, render: {:.1f} ms)",
		static_cast<int>(Timer.GetFPS()), //static_cast<int>(FPSSmoothed.Get()),
		1000*FrameTime,//FrameTimeSmoothed.Get(),
		1000*UpdateTime,//UpdateTimeSmoothed.Get(),
		1000*RenderTime//RenderTimeSmoothed.Get()
	);

	std::println("{} fps / {:.1f} ms (update: {:.1f} ms, render: {:.1f} ms)", static_cast<int>(Timer.GetFPS()), 1000 * FrameTime, 1000 * UpdateTime, 1000 * RenderTime);
	Renderer.DrawText(SVector{ 0, 0 }, Colors::White, Text.c_str());
}

void CApp::Update()
{
	CScopedStopwatch _(UpdateTime);

	Timer.Tick();
	Simulation.Update();
}

void CApp::Render()
{
	CScopedStopwatch _(RenderTime);

	Renderer.PreRender();
	
	Simulation.Render();
	DrawFPS();

	Renderer.PostRender();
}

void CApp::OnKeyDown(SDL_Keycode Key)
{
	switch (Key)
	{
		case SDLK_ESCAPE:
			Simulation.Reset();
			break;

		case SDLK_1:
			Simulation.Reset();
			Simulation.SpawnRandomWalls(100'000);
			Simulation.SpawnRandomBullets(1'000);
			break;
	}
}