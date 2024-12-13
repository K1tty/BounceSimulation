#include "Rectangle.h"

SLine SRectangle::Top() const
{
  return SLine{ 0, 0, Width, 0 };
}

SLine SRectangle::Bottom() const
{
  return SLine{ 0, Height, Width, Height };
}

SLine SRectangle::Left() const
{
  return SLine{ 0, 0, 0, Height };
}

SLine SRectangle::Right() const
{
  return SLine{ Width, 0, Width, Height };
}
