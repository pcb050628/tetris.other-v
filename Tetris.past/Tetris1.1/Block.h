#pragma once
#include "Square.h"
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>


class Block
{
public:
	Block();
	Block(BlockType t);
	~Block();

	Square* squares[4] = { nullptr };

private:
	Position Pos = Position(100, 100);
	BlockType type;
	CurDir dir;

	void Init();

public:
	void SetPos(int x, int y);
	Position GetPos() { return Pos; }
	BlockType GetType() { return type; }
	CurDir GetDir() { return dir; }

	void DeleteSquare(int i);
	void LineDown(int line);

	void Speen(SpeenDir nextDir);

	void Render(HDC hdc, HBRUSH hbr);
};

