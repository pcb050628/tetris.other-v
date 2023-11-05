#include "Square.h"
#include <iostream>

Square::Square() :
	xOffset(NULL),
	yOffset(NULL),
	Pos(Position(0, 0))
{
	rect = { NULL, NULL, NULL, NULL };
}

Square::Square(Position pos):
	xOffset(NULL),
	yOffset(NULL)
{
	Pos = pos;

	rect = { NULL, NULL, NULL, NULL };
}

Square::Square(Position pos, int xOff, int yOff)
{
	Pos = pos;
	xOffset = xOff;
	yOffset = yOff;

	rect = { NULL, NULL, NULL, NULL };
}

Square::~Square()
{
}

void Square::SetPos(int x, int y)
{
	Pos = Position(x + xOffset, y + yOffset);

	rect = { 0, 0, 0, 0 };
	rect.left = Pos.x - (Width / 2) + 2;
	rect.top = Pos.y - (Height / 2) + 2;
	rect.right = Pos.x + (Width / 2) - 2;
	rect.bottom = Pos.y + (Height / 2) - 2;
}

void Square::SetOffset(int x, int y)
{
	xOffset = x;
	yOffset = y;
}

void Square::Render(HDC hdc, HBRUSH hbr)
{
	Rectangle(hdc
		, Pos.x - (Width / 2)
		, Pos.y - (Height / 2)
		, Pos.x + (Width / 2)
		, Pos.y + (Height / 2));

	FillRect(hdc, &rect, hbr);
}
