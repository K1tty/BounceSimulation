#include "Rectangle.h"

SLine SRectangle::Top() const
{
  return SLine{ X, Y, X + Width, Y };
}

SLine SRectangle::Bottom() const
{
  return SLine{ X, Y + Height, X + Width, Y + Height };
}

SLine SRectangle::Left() const
{
  return SLine{ X, Y, X, Y + Height };
}

SLine SRectangle::Right() const
{
  return SLine{ X + Width, Y, X + Width, Y + Height };
}
