#include "Block.h"
using namespace std;

Block::Block() :
	type(BlockType::none),
	dir(CurDir::base)
{
	Init();
}

Block::Block(BlockType t) :
	dir(CurDir::base)
{
	type = t;
	Init();
}

Block::~Block()
{
	for (int i = 0; i < 4; i++)
	{
		if(squares[i] != nullptr)
		{
			delete squares[i];
			squares[i] = nullptr;
		}
	}
}

void Block::Init()
{
	for (int i = 0; i < 4; i++)
	{
		squares[i] = new Square;
	}

	if(type == BlockType::none)
	{
		srand(time(NULL));

		int a(rand() % 100);

		if (a < 15)
		{
			type = BlockType::Box;
		}
		else if (14 < a && a < 29)
		{
			type = BlockType::L;
		}
		else if (28 < a && a < 43)
		{
			type = BlockType::ReverseL;
		}
		else if (42 < a && a < 57)
		{
			type = BlockType::ReverseZ;
		}
		else if (56 < a && a < 71)
		{
			type = BlockType::Stick;
		}
		else if (70 < a && a < 85)
		{
			type = BlockType::T;
		}
		else if (84 < a && a <= 100)
		{
			type = BlockType::Z;
		}
	}

	
	switch (type)
	{
	case BlockType::Box:
		squares[0]->SetOffset(-16, 16);
		squares[1]->SetOffset(16, 16);
		squares[2]->SetOffset(-16, -16);
		squares[3]->SetOffset(16, -16);
		
		break;

	case BlockType::L:
		squares[0]->SetOffset(-32, 0);
		squares[1]->SetOffset(0, 0);
		squares[2]->SetOffset(32, 0);
		squares[3]->SetOffset(32, -32);

		break;

	case BlockType::ReverseL:
		squares[0]->SetOffset(32, 0);
		squares[1]->SetOffset(0, 0);
		squares[2]->SetOffset(-32, 0);
		squares[3]->SetOffset(-32, -32);

		break;

	case BlockType::Z:
		squares[0]->SetOffset(-32, 0);
		squares[1]->SetOffset(0, 0);
		squares[2]->SetOffset(0, 32);
		squares[3]->SetOffset(32, 32);
		break;

	case BlockType::Stick:
		squares[0]->SetOffset(0, -64);
		squares[1]->SetOffset(0, -32);
		squares[2]->SetOffset(0, 0);
		squares[3]->SetOffset(0, 32);

		break;

	case BlockType::T:
		squares[0]->SetOffset(0, -32);
		squares[1]->SetOffset(-32, 0);
		squares[2]->SetOffset(0, 0);
		squares[3]->SetOffset(32, 0);

		break;

	case BlockType::ReverseZ:
		squares[0]->SetOffset(-32, 32);
		squares[1]->SetOffset(0, 32);
		squares[2]->SetOffset(0, 0);
		squares[3]->SetOffset(32, 0);

		break;
	}
}

void Block::SetPos(int x, int y)
{
	Pos = Position(x, y);

	for (int i = 0; i < 4; i++)
	{
		if(squares[i] != nullptr)
			squares[i]->SetPos(Pos.x, Pos.y);
	}
}

void Block::DeleteSquare(int i)
{
	if (squares[i] != nullptr)
	{
		delete squares[i];
		squares[i] = nullptr;
	}
}

void Block::LineDown(int line)
{
	bool check[4] = { false };

	for (int square = 0; square < 4; square++)
	{
		if (squares[square] != nullptr && squares[square]->GetPos().y < line)
		{
			check[square] = true;
		}
		else if (squares[square] == nullptr)
		{
			check[square] = true;
		}
	}

	if (check[0] && check[1] && check[2] && check[3])
	{
		SetPos(Pos.x, Pos.y + 32);
	}
	else
	{
		for (int square = 0; square < 4; square++)
		{
			if (squares[square] != nullptr && squares[square]->GetPos().y < line)
			{
				squares[square]->SetOffset(squares[square]->GetOffset().x, squares[square]->GetOffset().y + 32);

				SetPos(Pos.x, Pos.y);
			}
		}
	}
}

void Block::Speen(SpeenDir nextDir)
{
	switch (type)
	{
	case BlockType::L:
		switch (dir)
		{
		case CurDir::base:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(0, 32);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(0, -32);
				squares[3]->SetOffset(-32, -32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::left;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(0, -32);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(0, 32);
				squares[3]->SetOffset(32, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::right;
				break;
			}
			break;

		case CurDir::left:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(32, 0);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(-32, 0);
				squares[3]->SetOffset(-32, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::upside;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(-32, 0);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(32, 0);
				squares[3]->SetOffset(32, -32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::base;
				break;
			}
			break;

		case CurDir::right:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(-32, 0);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(32, 0);
				squares[3]->SetOffset(32, -32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::base;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(32, 0);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(-32, 0);
				squares[3]->SetOffset(-32, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::upside;
				break;
			}
			break;

		case CurDir::upside:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(0, -32);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(0, 32);
				squares[3]->SetOffset(32, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::right;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(0, 32);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(0, -32);
				squares[3]->SetOffset(-32, -32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::left;
				break;
			}
			break;
		}
		break;

	case BlockType::ReverseL:
		switch (dir)
		{
		case CurDir::base:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(0, -32);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(0, 32);
				squares[3]->SetOffset(-32, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::left;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(0, 32);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(0, -32);
				squares[3]->SetOffset(32, -32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::right;
				break;
			}
			break;

		case CurDir::left:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(-32, 0);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(32, 0);
				squares[3]->SetOffset(32, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::upside;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(32, 0);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(-32, 0);
				squares[3]->SetOffset(-32, -32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::base;
				break;
			}
			break;

		case CurDir::right:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(32, 0);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(-32, 0);
				squares[3]->SetOffset(-32, -32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::base;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(-32, 0);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(32, 0);
				squares[3]->SetOffset(32, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::upside;
				break;
			}
			break;

		case CurDir::upside:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(0, 32);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(0, -32);
				squares[3]->SetOffset(32, -32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::right;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(0, -32);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(0, 32);
				squares[3]->SetOffset(-32, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::left;
				break;
			}
			break;
		}
		break;

	case BlockType::ReverseZ:
		switch (dir)
		{
		case CurDir::base:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(32, 32);
				squares[1]->SetOffset(32, 0);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(0, -32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::left;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(-32, -32);
				squares[1]->SetOffset(-32, 0);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(0, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::right;
				break;
			}
			break;

		case CurDir::left:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(32, -32);
				squares[1]->SetOffset(0, -32);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(-32, 0);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::upside;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(-32, 32);
				squares[1]->SetOffset(0, 32);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(32, 0);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::base;
				break;
			}
			break;

		case CurDir::right:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(-32, 32);
				squares[1]->SetOffset(0, 32);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(32, 0);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::base;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(32, -32);
				squares[1]->SetOffset(0, -32);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(-32, 0);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::upside;
				break;
			}
			break;

		case CurDir::upside:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(-32, -32);
				squares[1]->SetOffset(-32, 0);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(0, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::right;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(32, 32);
				squares[1]->SetOffset(32, 0);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(0, -32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::left;
				break;
			}
			break;
		}
		break;

	case BlockType::Stick:
		switch (dir)
		{
		case CurDir::base:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(-64, 0);
				squares[1]->SetOffset(-32, 0);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(32, 0);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::left;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(64, 0);
				squares[1]->SetOffset(32, 0);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(-32, 0);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::right;
				break;
			}
			break;

		case CurDir::left:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(0, -64);
				squares[1]->SetOffset(0, -32);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(0, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::base;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(0, -64);
				squares[1]->SetOffset(0, -32);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(0, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::base;
				break;
			}
			break;

		case CurDir::right:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(0, -64);
				squares[1]->SetOffset(0, -32);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(0, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::base;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(0, -64);
				squares[1]->SetOffset(0, -32);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(0, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::base;
				break;
			}
			break;
		}
		break;

	case BlockType::T:
		switch (dir)
		{
		case CurDir::base:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(-32, 0);
				squares[1]->SetOffset(0, 32);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(0, -32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::left;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(32, 0);
				squares[1]->SetOffset(0, -32);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(0, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::right;
				break;
			}
			break;

		case CurDir::left:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(0, 32);
				squares[1]->SetOffset(32, 0);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(-32, 0);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::upside;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(0, -32);
				squares[1]->SetOffset(-32, 0);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(32, 0);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::base;
				break;
			}
			break;

		case CurDir::right:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(0, -32);
				squares[1]->SetOffset(-32, 0);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(32, 0);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::base;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(0, 32);
				squares[1]->SetOffset(32, 0);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(-32, 0);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::upside;
				break;
			}
			break;

		case CurDir::upside:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(32, 0);
				squares[1]->SetOffset(0, -32);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(0, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::right;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(-32, 0);
				squares[1]->SetOffset(0, 32);
				squares[2]->SetOffset(0, 0);
				squares[3]->SetOffset(0, -32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::left;
				break;
			}
			break;
		}
		break;

	case BlockType::Z:
		switch (dir)
		{
		case CurDir::base:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(0, 32);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(32, 0);
				squares[3]->SetOffset(32, -32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::left;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(0, -32);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(-32, 0);
				squares[3]->SetOffset(-32, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::right;
				break;
			}
			break;

		case CurDir::left:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(32, 0);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(0, -32);
				squares[3]->SetOffset(-32, -32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::upside;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(-32, 0);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(0, 32);
				squares[3]->SetOffset(32, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::base;
				break;
			}
			break;

		case CurDir::right:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(-32, 0);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(0, 32);
				squares[3]->SetOffset(32, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::base;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(32, 0);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(0, -32);
				squares[3]->SetOffset(-32, -32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::upside;
				break;
			}
			break;

		case CurDir::upside:
			switch (nextDir)
			{
			case SpeenDir::left:
				squares[0]->SetOffset(0, -32);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(-32, 0);
				squares[3]->SetOffset(-32, 32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::right;
				break;

			case SpeenDir::right:
				squares[0]->SetOffset(0, +32);
				squares[1]->SetOffset(0, 0);
				squares[2]->SetOffset(32, 0);
				squares[3]->SetOffset(32, -32);

				squares[0]->SetPos(Pos.x, Pos.y);
				squares[1]->SetPos(Pos.x, Pos.y);
				squares[2]->SetPos(Pos.x, Pos.y);
				squares[3]->SetPos(Pos.x, Pos.y);

				dir = CurDir::left;
				break;
			}
			break;
		}
		break;
	}
}

void Block::Render(HDC hdc , HBRUSH hbr)
{
	for (int i = 0; i < 4; i++)
	{
		if(squares[i] != nullptr)
			squares[i]->Render(hdc, hbr);
	}
}
