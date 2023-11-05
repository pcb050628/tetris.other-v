#pragma once
#include "pch.h"
#include <Windows.h>
#include "Block.h"
#include "Square.h"
#include <iostream>
#include <time.h>

class DrawShape
{
private:
	DrawShape() :
		CurBlock(nullptr),
		SavedBlock(BlockType::none)
	{
		blocks[49] = { nullptr };
	}
	DrawShape(const DrawShape&) = delete;
	DrawShape& operator=(const DrawShape&) = delete;
	~DrawShape();

	int score = 0;

	HWND m_hwnd;

	static DrawShape* inst;

	RECT WindowSize = { NULL, NULL, NULL, NULL };

	Block* blocks[50] = { nullptr };
	Block* CurBlock;
	BlockType SavedBlock;

	int Lines[18] = { 84, 116, 148, 180, 212, 244, 276, 308, 340, 372, 404, 436, 468, 500, 532, 564, 596, 628 };

public:
	static DrawShape* GetInst()
	{
		if (inst == nullptr)
			inst = new DrawShape;
		return inst;
	}

	static void ReleaseInstance()
	{
		if (inst != nullptr)
		{
			delete inst;
			inst = nullptr;
		}
	}

	void Init(HWND hwnd);

public:
	void Run(HWND hwnd, HDC hdc);
	void DrawBlock(HWND hwnd, HDC hdc);

	void BlockManage();

	void SpawnBlock();
	void SpawnBlock(BlockType type);
	void ChangeCurBlock() { CurBlock = nullptr; }
	void MoveCurBlock(int i);
	void BlockSpeen(SpeenDir dir);
	void PutBlockCheck();
	void DownToBottom();

	void BlockChange();

	void CurBlockHeightCheck();
	bool CurBlockLeftCheck();
	bool CurBlockRightCheck();
	bool CurBlockDownCheck();
	void LineCheck();
	void BlocksFloorCheck(int line);

	int ReturnScore() 
	{
		return score;
	}

	BlockType ReturnSavedBlockType() 
	{
		return SavedBlock;
	}
};

