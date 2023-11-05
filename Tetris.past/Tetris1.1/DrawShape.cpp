#include "DrawShape.h"
#include <time.h>

DrawShape* DrawShape::inst = nullptr;

DrawShape::~DrawShape()
{
	if (CurBlock)
		CurBlock = nullptr;

	for (int i = 0; i < 50; i++)
	{
		if (blocks[i] != nullptr)
		{
			delete blocks[i];
		}
		blocks[i] = nullptr;
	}
}

void DrawShape::DrawBlock(HWND hwnd, HDC hdc)
{
	HBRUSH hbr;
	hbr = CreateSolidBrush(RGB(0, 0, 255));

	for (int i = 0; i < 50; i++)
	{
		if(blocks[i] != nullptr)
		{
			blocks[i]->Render(hdc, hbr);
		}
	}
}

void DrawShape::SpawnBlock()
{
	KillTimer(m_hwnd, 2);

	

	if(CurBlock == nullptr)
	{
		for (int i = 0; i < 50; i++)
		{
			if (blocks[i] == nullptr)
			{
				blocks[i] = new Block;
				CurBlock = blocks[i];
				if (CurBlock->GetType() == BlockType::Box)
				{
					CurBlock->SetPos(132, 36);
				}
				else
				{
					CurBlock->SetPos(148, 52);
				}
				break;
			}
			else if (blocks[49] != nullptr)
			{
				BlockManage();
			}
		}
	}
}

void DrawShape::SpawnBlock(BlockType type)
{
	KillTimer(m_hwnd, 2);
	CurBlock = nullptr;

	for (int i = 0; i < 50; i++)
	{
		if (blocks[i] == nullptr)
		{
			blocks[i] = new Block(type);
			CurBlock = blocks[i];

			if (CurBlock->GetType() == BlockType::Box)
			{
				CurBlock->SetPos(68, 68);
			}
			else
			{
				CurBlock->SetPos(84, 84);
			}
			break;
		}
	}
}

void DrawShape::Run(HWND hwnd, HDC hdc)
{
	if (CurBlock == nullptr)
		SpawnBlock();

	DrawBlock(hwnd, hdc);

	m_hwnd = hwnd;
}

void DrawShape::MoveCurBlock(int i)
{
	if(CurBlock != nullptr)
	{
		switch (i)
		{
		case 1:
			CurBlock->SetPos(CurBlock->GetPos().x - 32, CurBlock->GetPos().y);
			break;

		case 2:
			CurBlock->SetPos(CurBlock->GetPos().x + 32, CurBlock->GetPos().y);
			break;

		case 3:
			CurBlock->SetPos(CurBlock->GetPos().x, CurBlock->GetPos().y + 32);
			break;
		}
	}

	CurBlockHeightCheck();
	LineCheck();
}

void DrawShape::BlockManage()
{
	for (int block = 0; block < 50; block++)
	{
		bool shouldDelete = true;
		for (int square = 0; square < 4; square++)
		{
			if (blocks[block] != nullptr && blocks[block]->squares[square] != nullptr)
			{
				shouldDelete = false;
			}
		}

		if (shouldDelete)
		{
			delete blocks[block];
			blocks[block] = nullptr;
		}
	}
}

void DrawShape::CurBlockHeightCheck()
{
	bool isCollided = false;

	if(CurBlock != nullptr)
	{
		for (int i = 0; i < 50; i++)
		{
			if (blocks[i] != nullptr && blocks[i] != CurBlock)
			{
				for (int j = 0; j < 4; j++)
				{
					if (blocks[i]->squares[j] != nullptr && (blocks[i]->squares[j]->GetPos() == CurBlock->squares[0]->GetPos() + Position(0, 32) ||
						blocks[i]->squares[j]->GetPos() == CurBlock->squares[1]->GetPos() + Position(0, 32) ||
						blocks[i]->squares[j]->GetPos() == CurBlock->squares[2]->GetPos() + Position(0, 32) ||
						blocks[i]->squares[j]->GetPos() == CurBlock->squares[3]->GetPos() + Position(0, 32))
						)
					{
						isCollided = true;
						break;
					}
				}
			}
			else
			{
				for (int k = 0; k < 4; k++)
				{
					if (CurBlock->squares[k]->GetPos().y >= 628)
					{
						isCollided = true;
						break;
					}
				}
			}
		}
	}

	if (isCollided)
	{
		PutBlockCheck();
	}
	else
		KillTimer(m_hwnd, 2);
}

void DrawShape::LineCheck() // 라인 완성되면 라인 지우기
{
	Block* b[10] = { nullptr };
	Square* s[10] = { nullptr };

	for (int check = 0; check < 18; check++) // 확인할 높이
	{
		for (int block = 0; block < 50; block++) // 블럭배열의 블럭 위치
		{
			if (blocks[block] != nullptr && blocks[block] != CurBlock)
			{
				for (int square = 0; square < 4; square++) // 블럭이 가지고있는 네모의 위치
				{
					if (blocks[block]->squares[square] != nullptr && blocks[block]->squares[square]->GetPos().y == Lines[check]) // 만약 네모의 높이가 확인할 높이와 같다면 추가
					{
						for (int l = 0; l < 10; l++)
						{
							if (b[l] == nullptr)
							{
								bool can = true;
								for (int v = 0; v < 10; v++)
								{
									if (b[v] == blocks[block])
									{
										can = false;
									}
									else
										break;
								}

								if (can)
								{
									b[l] = blocks[block];
								}
							}
							if (s[l] == nullptr)
							{
								bool can = true;
								for(int z = 0; z < 10; z++)
								{
									if (s[z] == blocks[block]->squares[square])
									{
										can = false;
										break;
									}
								}

								if(can)
								{
									s[l] = blocks[block]->squares[square];
								}

								break;
							}
						}
					}
				}
			}
		}

		// 아래는 줄 삭제 코드 ( 만약 한 줄에 10개의 네모가 있다면 삭제
		bool shouldDelete = true;

		for (int i = 0; i < 10; i++)
		{
			if (s[i] == nullptr)
			{
				shouldDelete = false;
				break;
			}
		}

		if (shouldDelete) 
		{
			for (int i = 0; i < 10; i++)
			{
				for (int v = 0; v < 10; v++)
				{
					for (int j = 0; j < 4; j++)
					{
						if (b[i]->squares[j] == s[v])
						{
							b[i]->DeleteSquare(j);
						}
					}
				}
			}
			score += 100;
			BlocksFloorCheck(check);
		}

		for (int i = 0; i < 10; i++)
		{
			b[i] = nullptr;
			s[i] = nullptr;
		}
	}
}

void DrawShape::BlockSpeen(SpeenDir dir)
{
	bool CanSpeen = true;

	if(CurBlock != nullptr && CurBlock->GetType() == BlockType::Stick)
	{
		switch (dir)
		{
		case SpeenDir::left:
			for (int block = 0; block < 50; block++)
			{
				for (int square = 0; square < 4; square++)
				{
					if (blocks[block] != nullptr && blocks[block] != CurBlock && blocks[block]->squares[square] != nullptr &&
						(blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(0, 32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(0, -32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, 32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, -32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, 32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, -32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, 0) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, 0) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-64, 0) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-64, -32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-64, 32)))
					{
						CanSpeen = false;
						break;
					}

					for (int square = 0; square < 4; square++)
					{
						if (blocks[block] != nullptr && blocks[block] != CurBlock && blocks[block]->squares[square] != nullptr &&
							(blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(0, 32) ||
								blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(0, -32) ||
								blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, 32) ||
								blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, -32) ||
								blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, 32) ||
								blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, -32) ||
								blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, 0) ||
								blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, 0) ||
								blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(0, -64) ||
								blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, -64) ||
								blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, -64)))
						{
							CanSpeen = false;
							break;
						}
					}
				}
			}
			break;
		case SpeenDir::right:
			for (int block = 0; block < 50; block++)
			{
				for (int square = 0; square < 4; square++)
				{
					if (blocks[block] != nullptr && blocks[block] != CurBlock && blocks[block]->squares[square] != nullptr &&
						(blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(0, 32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(0, -32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, 32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, -32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, 32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, -32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, 0) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, 0) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(64, 0) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(64, -32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(64, 32)))
					{
						CanSpeen = false;
						break;
					}
				}

				for (int square = 0; square < 4; square++)
				{
					if (blocks[block] != nullptr && blocks[block] != CurBlock && blocks[block]->squares[square] != nullptr &&
						(blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(0, 32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(0, -32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, 32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, -32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, 32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, -32) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, 0) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, 0) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(0, -64) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, -64) ||
							blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, -64)))
					{
						CanSpeen = false;
						break;
					}
				}
			}
			break;
		}
	
	}
	else if (CurBlock != nullptr && CurBlock->GetType() == BlockType::T)
	{
		for (int block = 0; block < 50; block++)
		{
			for (int square = 0; square < 4; square++)
			{
				if (blocks[block] != nullptr && blocks[block] != CurBlock && blocks[block]->squares[square] != nullptr &&
					(blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(0, 32) ||
						blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(0, -32) ||
						blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, 0) ||
						blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, 0)))
				{
					CanSpeen = false;
					break;
				}
			}
		}
	}
	else if (CurBlock != nullptr)
	{
		for (int block = 0; block < 50; block++)
		{
			for (int square = 0; square < 4; square++)
			{
				if (blocks[block] != nullptr && blocks[block] != CurBlock && blocks[block]->squares[square] != nullptr &&
					(blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(0, 32) ||
						blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(0, -32) ||
						blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, 32) ||
						blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, -32) ||
						blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, 32) ||
						blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, -32) ||
						blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(32, 0) ||
						blocks[block]->squares[square]->GetPos() == CurBlock->GetPos() + Position(-32, 0)))
				{
					CanSpeen = false;
					break;
				}
			}
		}
	}

	if(CanSpeen)
	{
		CurBlock->Speen(dir);
	}
}


void DrawShape::PutBlockCheck()
{
	SetTimer(m_hwnd, 2, 1000, NULL);
}

void DrawShape::BlocksFloorCheck(int line)
{
	for (int block = 0; block < 50; block++)
	{
		for(int square = 0; square < 4; square++)
		{
			if (blocks[block] != nullptr && blocks[block]->squares[square] != nullptr && blocks[block]->squares[square]->GetPos().y < Lines[line])
			{
				blocks[block]->LineDown(Lines[line]);
				break;
			}
		}
	}
}

bool DrawShape::CurBlockLeftCheck()
{
	bool isCollided = false;
	
	if(CurBlock != nullptr)
	{
		for (int i = 0; i < 50; i++)
		{
			if (blocks[i] != nullptr && blocks[i] != CurBlock)
			{
				for (int j = 0; j < 4; j++)
				{
					if (blocks[i]->squares[j] != nullptr && (blocks[i]->squares[j]->GetPos() == CurBlock->squares[0]->GetPos() + Position(-32, 0) ||
						blocks[i]->squares[j]->GetPos() == CurBlock->squares[1]->GetPos() + Position(-32, 0) ||
						blocks[i]->squares[j]->GetPos() == CurBlock->squares[2]->GetPos() + Position(-32, 0) ||
						blocks[i]->squares[j]->GetPos() == CurBlock->squares[3]->GetPos() + Position(-32, 0))
						)
					{
						isCollided = true;
						break;
					}
				}
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					if (CurBlock->squares[i]->GetPos().x == 20)
					{
						isCollided = true;
						break;
					}
				}
			}
		}
	}

	return isCollided;
	LineCheck();
}

bool DrawShape::CurBlockRightCheck()
{
	bool isCollided = false;
	
	if(CurBlock != nullptr)
	{
		for (int i = 0; i < 50; i++)
		{
			if (blocks[i] != nullptr && blocks[i] != CurBlock)
			{
				for (int j = 0; j < 4; j++)
				{
					if (blocks[i]->squares[j] != nullptr && (blocks[i]->squares[j]->GetPos() == CurBlock->squares[0]->GetPos() + Position(32, 0) ||
						blocks[i]->squares[j]->GetPos() == CurBlock->squares[1]->GetPos() + Position(32, 0) ||
						blocks[i]->squares[j]->GetPos() == CurBlock->squares[2]->GetPos() + Position(32, 0) ||
						blocks[i]->squares[j]->GetPos() == CurBlock->squares[3]->GetPos() + Position(32, 0))
						)
					{
						isCollided = true;
						break;
					}
				}
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					if (CurBlock->squares[i]->GetPos().x == 308)
					{
						isCollided = true;
						break;
					}
				}
			}
		}
	}

	return isCollided;
	LineCheck();
}

void DrawShape::BlockChange()
{
	KillTimer(m_hwnd, 2);
	if (SavedBlock == BlockType::none)
	{
		SavedBlock = CurBlock->GetType();

		for (int i = 0; i < 50; i++)
		{
			if (blocks[i] != nullptr && blocks[i] == CurBlock)
			{
				delete blocks[i];
				blocks[i] = nullptr;
			}
		}

		CurBlock = nullptr;
	}
	else
	{
		BlockType tmp = SavedBlock;
		SavedBlock = CurBlock->GetType();

		for (int i = 0; i < 50; i++)
		{
			if (blocks[i] != nullptr && blocks[i] == CurBlock)
			{
				delete blocks[i];
				blocks[i] = nullptr;
			}
		}

		SpawnBlock(tmp);
	}
}

void DrawShape::DownToBottom()
{
	if (CurBlock != nullptr)
	{
		while(!CurBlockDownCheck())
		{
			MoveCurBlock(3);
		}
		CurBlock = nullptr;
		KillTimer(m_hwnd, 2);
		LineCheck();
	}
}

bool DrawShape::CurBlockDownCheck()
{
	bool isCollided = false;

	if (CurBlock != nullptr)
	{
		for (int i = 0; i < 50; i++)
		{
			if (blocks[i] != nullptr && blocks[i] != CurBlock)
			{
				for (int j = 0; j < 4; j++)
				{
					for(int s = 0; s < 4; s++)
					{
						if (blocks[i]->squares[j] != nullptr && CurBlock->squares[s] != nullptr && (blocks[i]->squares[j]->GetPos() == CurBlock->squares[s]->GetPos() + Position(0, 32))
							)
						{
							isCollided = true;
							break;
						}
					}
				}
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					if (CurBlock->squares[i]->GetPos().y == 628)
					{
						isCollided = true;
						break;
					}
				}
			}
		}
	}

	return isCollided;
	LineCheck();
}

void DrawShape::Init(HWND hwnd)
{
	GetClientRect(hwnd, &WindowSize);
}