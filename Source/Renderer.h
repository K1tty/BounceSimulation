#pragma once
#include <vector>
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_FontCache.h"
#include "Vector.h"
#include "Color.h"
#include "Line.h"
#include "Vector.h"
#include "StrideView.h"
#include <functional>


class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	bool Init(uint16_t Width, uint16_t Height);

	void PreRender();
	void PostRender();

	void DrawText(SVector Position, SColor Color, const char* Text) const;

	void DrawLines(const TStrideView<SLine>& Lines, SColor Color) const;
	void DrawPoints(const TStrideView<SVector>& Points, SColor Color) const;

private:
	bool CreateRenderer(uint16_t Width, uint16_t Height);
	bool CreateFont();

	void DestroyRenderer();
	void DestroyFont();

	void Clear(SColor Color);
	void Present();

private:
	SDL_Window* Window = nullptr;
	SDL_Renderer* Renderer = nullptr;
	FC_Font* Font = nullptr;
};

extern CRenderer* GRenderer;