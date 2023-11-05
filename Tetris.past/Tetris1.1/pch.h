#pragma once

class Position
{
public:
	Position();
	Position(int ix, int iy);
	~Position();

	bool operator ==(const Position& other) const {
		if (other.x == x && other.y == y)
			return true;
		else
			return false;
	}

	Position operator +(const Position& other) const {
		Position result = Position(0, 0);
		result.x = x + other.x;
		result.y = y + other.y;

		return result;
	}

public:
	int x;
	int y;
};


enum class BlockType
{
	Box,
	Stick,
	T,
	L,
	ReverseL,
	Z,
	ReverseZ,
	none
};

enum class SpeenDir
{
	left,
	right
};

enum class CurDir
{
	base,
	left,
	upside,
	right
};