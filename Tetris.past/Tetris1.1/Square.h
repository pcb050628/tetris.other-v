#pragma once
#include "pch.h"
#include <Windows.h>


class Square
{
public:
	Square();
	Square(Position pos);
	Square(Position pos, int xOff, int yOff);
	~Square();

private:
	RECT rect;

	Position Pos;
	int xOffset;
	int yOffset;

	int Width = 32;
	int Height = 32;

public:

	void SetPos(int x, int y);
	void SetOffset(int x, int y);

	Position GetPos() { return Pos; }
	Position GetOffset() { return Position(xOffset, yOffset); }

	void Render(HDC hdc, HBRUSH hbr);
};

