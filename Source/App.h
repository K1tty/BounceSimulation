#pragma once
#include <optional>
#include "Timer.h"
#include "Renderer.h"
#include "Simulation.h"
#include "LineGizmo.h"


class CApp
{
public:
	void Run();

private:
	bool Init();
	void Tick();
	void Render();
	bool PollEvents();

	void DrawFPS();

private:
	CTimer Timer;
	CRenderer Renderer;
	CSimulation Simulation;
	std::optional<CLineGizmo> LineGizmo;

	float UpdateTime = 0;
	float RenderTime = 0;
};