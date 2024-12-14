#pragma once
#include "Timer.h"
#include "Renderer.h"
#include "Simulation.h"


class CApp
{
public:
	void Run();

private:
	bool Init();
	void Update();
	void Render();

	void DrawFPS();

	void OnKeyDown(SDL_Keycode Key);

private:
	CTimer Timer;
	CRenderer Renderer;
	CSimulation Simulation;

	float UpdateTime = 0;
	float RenderTime = 0;
};