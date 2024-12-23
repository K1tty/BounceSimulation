#include "Renderer.h"


CRenderer* GRenderer = nullptr;

CRenderer::CRenderer()
{
  GRenderer = this;
}

CRenderer::~CRenderer()
{
  DestroyFont();
  DestroyRenderer();

  GRenderer = nullptr;
}

bool CRenderer::Init(uint16_t WindowWidth, uint16_t WindowHeight)
{
  if (!CreateRenderer(WindowWidth, WindowHeight))
    return false;

  if (!CreateFont())
    return false;
 
  return true;
}

bool CRenderer::CreateRenderer(uint16_t WindowWidth, uint16_t WindowHeight)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    return false;

  Window = SDL_CreateWindow("Bounce Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(WindowWidth), static_cast<int>(WindowHeight), SDL_WINDOW_SHOWN);
  if (!Window)
    return false;

  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
  SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);

  Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
  if (!Renderer)
    return false;

  return true;
}

bool CRenderer::CreateFont()
{
  Font = FC_CreateFont();
  if (!Font)
    return false;

  if (!FC_LoadFont(Font, Renderer, "Fonts/consolas.ttf", 16, FC_MakeColor(0xff, 0xff, 0xff, 0xff), TTF_STYLE_NORMAL))
    return false;

  return true;
}

void CRenderer::DestroyRenderer()
{
  SDL_DestroyRenderer(Renderer);
  SDL_DestroyWindow(Window);
  SDL_Quit();
}

void CRenderer::DestroyFont()
{
  FC_FreeFont(Font);
}

void CRenderer::DrawText(SVector Position, SColor Color, const char* Text) const
{
  FC_DrawColor(Font, Renderer, Position.X, Position.Y, SDL_Color(Color.R, Color.G, Color.B, Color.A), Text);
}

void CRenderer::DrawLines(const TStrideView<SLine>& Lines, SColor Color) const
{
  static std::vector<SDL_Vertex> VertexBuffer;
  VertexBuffer.resize(Lines.GetSize() * 4);

  static std::vector<int> IndexBuffer;
  IndexBuffer.resize(Lines.GetSize() * 6);

  const SDL_Color SDLColor = { Color.R, Color.G, Color.B, Color.A };
  const float Thickness = 1.0f / 2;

  for (size_t i = 0; i < Lines.GetSize(); ++i)
  {
    const SLine& Line = Lines[i];
    const size_t VertexOffset = i * 4;

    const SVector Direction = Normalize({ Line.P2.X - Line.P1.X, Line.P2.Y - Line.P1.Y });
    const SVector Perp = Normal(Direction);

    const float XOffset = Perp.X * Thickness;
    const float YOffset = Perp.Y * Thickness;

    VertexBuffer[VertexOffset + 0].position.x = Line.P1.X + XOffset;
    VertexBuffer[VertexOffset + 0].position.y = Line.P1.Y + YOffset;
    VertexBuffer[VertexOffset + 0].color = SDLColor;

    VertexBuffer[VertexOffset + 1].position.x = Line.P1.X - XOffset;
    VertexBuffer[VertexOffset + 1].position.y = Line.P1.Y - YOffset;
    VertexBuffer[VertexOffset + 1].color = SDLColor;

    VertexBuffer[VertexOffset + 2].position.x = Line.P2.X + XOffset;
    VertexBuffer[VertexOffset + 2].position.y = Line.P2.Y + YOffset;
    VertexBuffer[VertexOffset + 2].color = SDLColor;

    VertexBuffer[VertexOffset + 3].position.x = Line.P2.X - XOffset;
    VertexBuffer[VertexOffset + 3].position.y = Line.P2.Y - YOffset;
    VertexBuffer[VertexOffset + 3].color = SDLColor;

    const int IndexBase = static_cast<int>(i * 4);
    const size_t IndexOffset = i * 6;

    IndexBuffer[IndexOffset + 0] = IndexBase + 0;
    IndexBuffer[IndexOffset + 1] = IndexBase + 1;
    IndexBuffer[IndexOffset + 2] = IndexBase + 2;
    IndexBuffer[IndexOffset + 3] = IndexBase + 2;
    IndexBuffer[IndexOffset + 4] = IndexBase + 1;
    IndexBuffer[IndexOffset + 5] = IndexBase + 3;
  }

  SDL_RenderGeometry(Renderer, nullptr, &VertexBuffer[0], static_cast<int>(VertexBuffer.size()), &IndexBuffer[0], static_cast<int>(IndexBuffer.size()));
}

void CRenderer::DrawPoints(const TStrideView<SVector>& Points, SColor Color) const
{
  const float PointSize = 3.0f;
  const float PointHalfSize = PointSize / 2;

  static std::vector<SDL_FRect> RectangleBuffer;
  RectangleBuffer.resize(Points.GetSize());

  for (size_t i = 0; i < RectangleBuffer.size(); ++i)
  {
    const SVector& Point = Points[i];

    SDL_FRect& Rectangle = RectangleBuffer[i];

    Rectangle.x = Point.X - PointHalfSize;
    Rectangle.y = Point.Y - PointHalfSize;
    Rectangle.h = PointSize;
    Rectangle.w = PointSize;
  }

  SDL_SetRenderDrawColor(Renderer, Color.R, Color.G, Color.B, Color.A);
  SDL_RenderFillRectsF(Renderer, &RectangleBuffer[0], static_cast<int>(RectangleBuffer.size()));
}

void CRenderer::PreRender()
{
  Clear(Colors::Black);
}

void CRenderer::Clear(SColor Color)
{
  SDL_SetRenderDrawColor(Renderer, Color.R, Color.G, Color.B, Color.A);
  SDL_RenderClear(Renderer);
}

void CRenderer::Present()
{
  SDL_RenderPresent(Renderer);
}

void CRenderer::PostRender()
{
  Present();
}