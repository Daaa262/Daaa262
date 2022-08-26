#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

enum PIECE
{
	EMPTY,
	WHITE_PAWN,
	WHITE_KNIGHT,
	WHITE_BISHOP,
	WHITE_ROOK,
	WHITE_QUEEN,
	WHITE_KING,
	BLACK_PAWN,
	BLACK_KNIGHT,
	BLACK_BISHOP,
	BLACK_ROOK,
	BLACK_QUEEN,
	BLACK_KING
};

enum COLOR
{
	WHITE,
	BLACK,
	NONE
};

char pieceColor(char piece)
{
	if (piece == WHITE_PAWN || piece == WHITE_KNIGHT || piece == WHITE_BISHOP || piece == WHITE_ROOK || piece == WHITE_QUEEN || piece == WHITE_KING)
		return WHITE;
	else if (piece == BLACK_PAWN || piece == BLACK_KNIGHT || piece == BLACK_BISHOP || piece == BLACK_ROOK || piece == BLACK_QUEEN || piece == BLACK_KING)
		return BLACK;
	else return NONE;
}

class Board
{
private:
	char enPassant;
	bool whiteKingMoved;
	bool whiteArookMoved;
	bool whiteHrookMoved;
	bool blackKingMoved;
	bool blackArookMoved;
	bool blackHrookMoved;
public:
	char square[64];

	void reset();
	bool isKingUnderCheck(char playerColor);
	bool isMoveLegal(char start, char end, char playerColor);
	void move(char start, char end, char promotionTo = 0);
	char getSquare(char number);

	Board()
	{
		reset();
	}
};

short stringMoveToMove(string stringMove)
{
	return 0;
}

int main()
{
	RenderWindow window(VideoMode(1000, 800), "Szachy", Style::Close);

	Board board;

	string move;
	char start, end, playerColor = 0;
	char moveState = 0;

	Mouse mouse;

	Texture boardTexture[2];
	boardTexture[0].loadFromFile("boardWhite.png");
	boardTexture[1].loadFromFile("boardBlack.png");
	Sprite boardSprite[2];
	boardSprite[0].setTexture(boardTexture[0]);
	boardSprite[1].setTexture(boardTexture[1]);

	Texture whitePawnTexture;
	whitePawnTexture.loadFromFile("whitePawn.png");
	Sprite whitePawnSprite(whitePawnTexture);
	Texture whiteKnightTexture;
	whiteKnightTexture.loadFromFile("whiteKnight.png");
	Sprite whiteKnightSprite(whiteKnightTexture);
	Texture whiteBishopTexture;
	whiteBishopTexture.loadFromFile("whiteBishop.png");
	Sprite whiteBishopSprite(whiteBishopTexture);
	Texture whiteRookTexture;
	whiteRookTexture.loadFromFile("whiteRook.png");
	Sprite whiteRookSprite(whiteRookTexture);
	Texture whiteQueenTexture;
	whiteQueenTexture.loadFromFile("whiteQueen.png");
	Sprite whiteQueenSprite(whiteQueenTexture);
	Texture whiteKingTexture;
	whiteKingTexture.loadFromFile("whiteKing.png");
	Sprite whiteKingSprite(whiteKingTexture);
	Texture blackPawnTexture;
	blackPawnTexture.loadFromFile("blackPawn.png");
	Sprite blackPawnSprite(blackPawnTexture);
	Texture blackKnightTexture;
	blackKnightTexture.loadFromFile("blackKnight.png");
	Sprite blackKnightSprite(blackKnightTexture);
	Texture blackBishopTexture;
	blackBishopTexture.loadFromFile("blackBishop.png");
	Sprite blackBishopSprite(blackBishopTexture);
	Texture blackRookTexture;
	blackRookTexture.loadFromFile("blackRook.png");
	Sprite blackRookSprite(blackRookTexture);
	Texture blackQueenTexture;
	blackQueenTexture.loadFromFile("blackQueen.png");
	Sprite blackQueenSprite(blackQueenTexture);
	Texture blackKingTexture;
	blackKingTexture.loadFromFile("blackKing.png");
	Sprite blackKingSprite(blackKingTexture);

	RectangleShape marked(Vector2f(100, 100));
	marked.setFillColor(Color(255, 255, 0, 70));
	Texture legalMoveTexture;
	legalMoveTexture.loadFromFile("legalMove.png");
	Sprite legalMoveSprite(legalMoveTexture);

	Texture promotion1Texture;
	promotion1Texture.loadFromFile("promotion1.png");
	Sprite promotion1Sprite(promotion1Texture);
	Texture promotion2Texture;
	promotion2Texture.loadFromFile("promotion2.png");
	Sprite promotion2Sprite(promotion2Texture);

	while (window.isOpen())
	{
		window.clear(Color(200, 200, 200));

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed && mouse.isButtonPressed(Mouse::Left) && mouse.getPosition(window).x >= 0 && mouse.getPosition(window).x <= 800 && mouse.getPosition(window).y >= 0 && mouse.getPosition(window).y <= 800 && moveState == 0)
			{
				start = mouse.getPosition(window).x / 100 + 8 * (7 - mouse.getPosition(window).y / 100);

				if (pieceColor(board.getSquare(start)) == playerColor)
					moveState = 1;
			}
			else if (event.type == Event::MouseButtonPressed && mouse.isButtonPressed(Mouse::Left) && mouse.getPosition(window).x >= 0 && mouse.getPosition(window).x <= 800 && mouse.getPosition(window).y >= 0 && mouse.getPosition(window).y <= 800 && moveState == 1)
			{
				end = mouse.getPosition(window).x / 100 + 8 * (7 - mouse.getPosition(window).y / 100);

				if (end == start)
					moveState = 0;
				else if (pieceColor(board.getSquare(end)) == playerColor)
					start = end;
				else if (board.isMoveLegal(start, end, playerColor))
					if (board.square[start] == WHITE_PAWN && end >= 56 || board.square[start] == BLACK_PAWN && end <= 7)
						moveState = 2;
					else
					{
						board.move(start, end);
						playerColor = (playerColor + 1) % 2;
						moveState = 0;
					}
			}
			else if (event.type == Event::MouseButtonPressed && mouse.isButtonPressed(Mouse::Left) && mouse.getPosition(window).x >= 0 && mouse.getPosition(window).x <= 800 && mouse.getPosition(window).y >= 0 && mouse.getPosition(window).y <= 800 && moveState == 2)
				if (playerColor == WHITE)
				{
					if (mouse.getPosition(window).x >= end % 8 * 100 && mouse.getPosition(window).x <= end % 8 * 100 + 100 && mouse.getPosition(window).y <= 100)
					{
						board.move(start, end, 0);
						playerColor = (playerColor + 1) % 2;
						moveState = 0;
					}
					else if (mouse.getPosition(window).x >= end % 8 * 100 && mouse.getPosition(window).x <= end % 8 * 100 + 100 && mouse.getPosition(window).y <= 200)
					{
						board.move(start, end, 1);
						playerColor = (playerColor + 1) % 2;
						moveState = 0;
					}
					else if (mouse.getPosition(window).x >= end % 8 * 100 && mouse.getPosition(window).x <= end % 8 * 100 + 100 && mouse.getPosition(window).y <= 300)
					{
						board.move(start, end, 2);
						playerColor = (playerColor + 1) % 2;
						moveState = 0;
					}
					else if (mouse.getPosition(window).x >= end % 8 * 100 && mouse.getPosition(window).x <= end % 8 * 100 + 100 && mouse.getPosition(window).y <= 400)
					{
						board.move(start, end, 3);
						playerColor = (playerColor + 1) % 2;
						moveState = 0;
					}
					else moveState = 1;
				}
				else
				{
					if (mouse.getPosition(window).x >= end % 8 * 100 && mouse.getPosition(window).x <= end % 8 * 100 + 100 && mouse.getPosition(window).y >= 700)
					{
						board.move(start, end, 0);
						playerColor = (playerColor + 1) % 2;
						moveState = 0;
					}
					else if (mouse.getPosition(window).x >= end % 8 * 100 && mouse.getPosition(window).x <= end % 8 * 100 + 100 && mouse.getPosition(window).y >= 600)
					{
						board.move(start, end, 1);
						playerColor = (playerColor + 1) % 2;
						moveState = 0;
					}
					else if (mouse.getPosition(window).x >= end % 8 * 100 && mouse.getPosition(window).x <= end % 8 * 100 + 100 && mouse.getPosition(window).y >= 500)
					{
						board.move(start, end, 2);
						playerColor = (playerColor + 1) % 2;
						moveState = 0;
					}
					else if (mouse.getPosition(window).x >= end % 8 * 100 && mouse.getPosition(window).x <= end % 8 * 100 + 100 && mouse.getPosition(window).y >= 400)
					{
						board.move(start, end, 3);
						playerColor = (playerColor + 1) % 2;
						moveState = 0;
					}
					else moveState = 1;
				}
		}

		for (int y = 0; y < 8; y++)
			for (int x = 0; x < 8; x++)
			{
				boardSprite[(x + y) % 2].setPosition(static_cast<float>(x * 100), 700 - static_cast<float>(y * 100));
				window.draw(boardSprite[(x + y) % 2]);

				if (moveState != 0 && start == y * 8 + x)
				{
					marked.setPosition(static_cast<float>(x * 100), static_cast<float>(700 - y * 100));
					window.draw(marked);
				}

				switch (board.getSquare(y * 8 + x))
				{
				case WHITE_PAWN:
					whitePawnSprite.setPosition(static_cast<float>(x * 100), 700 - static_cast<float>(y * 100));
					window.draw(whitePawnSprite);
					break;
				case WHITE_KNIGHT:
					whiteKnightSprite.setPosition(static_cast<float>(x * 100), 700 - static_cast<float>(y * 100));
					window.draw(whiteKnightSprite);
					break;
				case WHITE_BISHOP:
					whiteBishopSprite.setPosition(static_cast<float>(x * 100), 700 - static_cast<float>(y * 100));
					window.draw(whiteBishopSprite);
					break;
				case WHITE_ROOK:
					whiteRookSprite.setPosition(static_cast<float>(x * 100), 700 - static_cast<float>(y * 100));
					window.draw(whiteRookSprite);
					break;
				case WHITE_QUEEN:
					whiteQueenSprite.setPosition(static_cast<float>(x * 100), 700 - static_cast<float>(y * 100));
					window.draw(whiteQueenSprite);
					break;
				case WHITE_KING:
					whiteKingSprite.setPosition(static_cast<float>(x * 100), 700 - static_cast<float>(y * 100));
					window.draw(whiteKingSprite);
					break;
				case BLACK_PAWN:
					blackPawnSprite.setPosition(static_cast<float>(x * 100), 700 - static_cast<float>(y * 100));
					window.draw(blackPawnSprite);
					break;
				case BLACK_KNIGHT:
					blackKnightSprite.setPosition(static_cast<float>(x * 100), 700 - static_cast<float>(y * 100));
					window.draw(blackKnightSprite);
					break;
				case BLACK_BISHOP:
					blackBishopSprite.setPosition(static_cast<float>(x * 100), 700 - static_cast<float>(y * 100));
					window.draw(blackBishopSprite);
					break;
				case BLACK_ROOK:
					blackRookSprite.setPosition(static_cast<float>(x * 100), 700 - static_cast<float>(y * 100));
					window.draw(blackRookSprite);
					break;
				case BLACK_QUEEN:
					blackQueenSprite.setPosition(static_cast<float>(x * 100), 700 - static_cast<float>(y * 100));
					window.draw(blackQueenSprite);
					break;
				case BLACK_KING:
					blackKingSprite.setPosition(static_cast<float>(x * 100), 700 - static_cast<float>(y * 100));
					window.draw(blackKingSprite);
					break;
				}
			}

		if (moveState == 1)
		{
			for (int i = 0; i < 64; i++)
				if (board.isMoveLegal(start, i, playerColor))
				{
					legalMoveSprite.setPosition(static_cast<float>(i % 8 * 100), static_cast<float>(700 - i / 8 * 100));
					window.draw(legalMoveSprite);
				}
		}
		else if (moveState == 2)
		{
			if (playerColor == WHITE)
			{
				promotion1Sprite.setPosition(static_cast<float>(end % 8 * 100), 0);
				window.draw(promotion1Sprite);
				whiteQueenSprite.setPosition(static_cast<float>(end % 8 * 100), 0);
				window.draw(whiteQueenSprite);
				promotion2Sprite.setPosition(static_cast<float>(end % 8 * 100), 100);
				window.draw(promotion2Sprite);
				whiteRookSprite.setPosition(static_cast<float>(end % 8 * 100), 100);
				window.draw(whiteRookSprite);
				promotion1Sprite.setPosition(static_cast<float>(end % 8 * 100), 200);
				window.draw(promotion1Sprite);
				whiteBishopSprite.setPosition(static_cast<float>(end % 8 * 100), 200);
				window.draw(whiteBishopSprite);
				promotion2Sprite.setPosition(static_cast<float>(end % 8 * 100), 300);
				window.draw(promotion2Sprite);
				whiteKnightSprite.setPosition(static_cast<float>(end % 8 * 100), 300);
				window.draw(whiteKnightSprite);
			}
			else
			{
				promotion1Sprite.setPosition(static_cast<float>(end % 8 * 100), 700);
				window.draw(promotion1Sprite);
				blackQueenSprite.setPosition(static_cast<float>(end % 8 * 100), 700);
				window.draw(blackQueenSprite);
				promotion2Sprite.setPosition(static_cast<float>(end % 8 * 100), 600);
				window.draw(promotion2Sprite);
				blackRookSprite.setPosition(static_cast<float>(end % 8 * 100), 600);
				window.draw(blackRookSprite);
				promotion1Sprite.setPosition(static_cast<float>(end % 8 * 100), 500);
				window.draw(promotion1Sprite);
				blackBishopSprite.setPosition(static_cast<float>(end % 8 * 100), 500);
				window.draw(blackBishopSprite);
				promotion2Sprite.setPosition(static_cast<float>(end % 8 * 100), 400);
				window.draw(promotion2Sprite);
				blackKnightSprite.setPosition(static_cast<float>(end % 8 * 100), 400);
				window.draw(blackKnightSprite);
			}
		}

		window.display();
	}

	return 0;
}

void Board::reset()
{
	enPassant = -1;
	whiteKingMoved = false;
	whiteArookMoved = false;
	whiteHrookMoved = false;
	blackKingMoved = false;
	blackArookMoved = false;
	blackHrookMoved = false;

	square[0] = WHITE_ROOK;
	square[1] = WHITE_KNIGHT;
	square[2] = WHITE_BISHOP;
	square[3] = WHITE_QUEEN;
	square[4] = WHITE_KING;
	square[5] = WHITE_BISHOP;
	square[6] = WHITE_KNIGHT;
	square[7] = WHITE_ROOK;
	for (int i = 8; i < 16; i++)
	{
		square[i] = WHITE_PAWN;
		square[i + 40] = BLACK_PAWN;
	}
	for (int i = 16; i < 48; i++)
		square[i] = EMPTY;
	square[56] = BLACK_ROOK;
	square[57] = BLACK_KNIGHT;
	square[58] = BLACK_BISHOP;
	square[59] = BLACK_QUEEN;
	square[60] = BLACK_KING;
	square[61] = BLACK_BISHOP;
	square[62] = BLACK_KNIGHT;
	square[63] = BLACK_ROOK;
}

bool Board::isKingUnderCheck(char playerColor)
{
	if (playerColor == WHITE)
	{
		for (int i = 0; i < 64; i++)
			if (square[i] == WHITE_KING)
			{
				if (i % 8 != 0 && i / 8 < 6)
				{
					if (square[i + 7] == BLACK_PAWN)
						return true;
				}
				if (i % 8 != 7 && i / 8 < 6)
				{
					if (square[i + 9] == BLACK_PAWN)
						return true;
				}

				if (i % 8 > 1)
				{
					if (i / 8 != 7)
						if (square[i + 6] == BLACK_KNIGHT)
							return true;
					if (i / 8 != 0)
						if (square[i - 10] == BLACK_KNIGHT)
							return true;
				}
				if (i % 8 != 0)
				{
					if (i / 8 < 6)
						if (square[i + 15] == BLACK_KNIGHT)
							return true;
					if (i / 8 > 1)
						if (square[i - 17] == BLACK_KNIGHT)
							return true;
				}
				if (i % 8 != 7)
				{
					if (i / 8 < 6)
						if (square[i + 17] == BLACK_KNIGHT)
							return true;
					if (i / 8 > 1)
						if (square[i - 15] == BLACK_KNIGHT)
							return true;
				}
				if (i % 8 < 6)
				{
					if (i / 8 != 7)
						if (square[i + 10] == BLACK_KNIGHT)
							return true;
					if (i / 8 != 0)
						if (square[i - 6] == BLACK_KNIGHT)
							return true;
				}

				for (int j = 1; j <= min(i % 8, i / 8); j++)
					if (square[i - j * 9] == BLACK_BISHOP || square[i - j * 9] == BLACK_QUEEN)
						return true;
					else if (square[i - j * 9] != EMPTY)
						break;
				for (int j = 1; j <= min(i % 8, 7 - i / 8); j++)
					if (square[i + j * 7] == BLACK_BISHOP || square[i + j * 7] == BLACK_QUEEN)
						return true;
					else if (square[i + j * 7] != EMPTY)
						break;
				for (int j = 1; j <= min(7 - i % 8, i / 8); j++)
					if (square[i - j * 7] == BLACK_BISHOP || square[i - j * 7] == BLACK_QUEEN)
						return true;
					else if (square[i - j * 7] != EMPTY)
						break;
				for (int j = 1; j <= min(7 - i % 8, 7 - i / 8); j++)
					if (square[i + j * 9] == BLACK_BISHOP || square[i + j * 9] == BLACK_QUEEN)
						return true;
					else if (square[i + j * 9] != EMPTY)
						break;

				for (int j = 1; j <= i % 8; j++)
					if (square[i - j] == BLACK_ROOK || square[i - j] == BLACK_QUEEN)
						return true;
					else if (square[i - j] != EMPTY)
						break;
				for (int j = 1; j <= 7 - i % 8; j++)
					if (square[i + j] == BLACK_ROOK || square[i + j] == BLACK_QUEEN)
						return true;
					else if (square[i + j] != EMPTY)
						break;
				for (int j = 1; j <= i / 8; j++)
					if (square[i - j * 8] == BLACK_ROOK || square[i - j * 8] == BLACK_QUEEN)
						return true;
					else if (square[i - j * 8] != EMPTY)
						break;
				for (int j = 1; j <= 7 - i / 8; j++)
					if (square[i + j * 8] == BLACK_ROOK || square[i + j * 8] == BLACK_QUEEN)
						return true;
					else if (square[i + j * 8] != EMPTY)
						break;

				if (i % 8 != 0)
				{
					if (square[i - 1] == BLACK_KING)
						return true;
					if (i / 8 != 0)
						if (square[i - 9] == BLACK_KING)
							return true;
					if (i / 8 != 7)
						if (square[i + 7] == BLACK_KING)
							return true;
				}
				if (i % 8 != 7)
				{
					if (square[i + 1] == BLACK_KING)
						return true;
					if (i / 8 != 0)
						if (square[i - 7] == BLACK_KING)
							return true;
					if (i / 8 != 7)
						if (square[i + 9] == BLACK_KING)
							return true;
				}
				if (i / 8 != 0)
				{
					if (square[i - 8] == BLACK_KING)
						return true;
				}
				if (i / 8 != 7)
				{
					if (square[i + 8] == BLACK_KING)
						return true;
				}

				break;
			}
	}
	else if(playerColor == BLACK)
		for (int i = 0; i < 64; i++)
			if (square[i] == BLACK_KING)
			{
				if (i % 8 != 0 && i / 8 > 1)
				{
					if (square[i - 9] == WHITE_PAWN)
						return true;
				}
				if (i % 8 != 7 && i / 8 > 1)
				{
					if (square[i - 7] == WHITE_PAWN)
						return true;
				}

				if (i % 8 > 1)
				{
					if (i / 8 != 7)
						if (square[i + 6] == WHITE_KNIGHT)
							return true;
					if (i / 8 != 0)
						if (square[i - 10] == WHITE_KNIGHT)
							return true;
				}
				if (i % 8 != 0)
				{
					if (i / 8 < 6)
						if (square[i + 15] == WHITE_KNIGHT)
							return true;
					if (i / 8 > 1)
						if (square[i - 17] == WHITE_KNIGHT)
							return true;
				}
				if (i % 8 != 7)
				{
					if (i / 8 < 6)
						if (square[i + 17] == WHITE_KNIGHT)
							return true;
					if (i / 8 > 1)
						if (square[i - 15] == WHITE_KNIGHT)
							return true;
				}
				if (i % 8 < 6)
				{
					if (i / 8 != 7)
						if (square[i + 10] == WHITE_KNIGHT)
							return true;
					if (i / 8 != 0)
						if (square[i - 6] == WHITE_KNIGHT)
							return true;
				}

				for (int j = 1; j <= min(i % 8, i / 8); j++)
					if (square[i - j * 9] == WHITE_BISHOP || square[i - j * 9] == WHITE_QUEEN)
						return true;
					else if (square[i - j * 9] != EMPTY)
						break;
				for (int j = 1; j <= min(i % 8, 7 - i / 8); j++)
					if (square[i + j * 7] == WHITE_BISHOP || square[i + j * 7] == WHITE_QUEEN)
						return true;
					else if (square[i + j * 7] != EMPTY)
						break;
				for (int j = 1; j <= min(7 - i % 8, i / 8); j++)
					if (square[i - j * 7] == WHITE_BISHOP || square[i - j * 7] == WHITE_QUEEN)
						return true;
					else if (square[i - j * 7] != EMPTY)
						break;
				for (int j = 1; j <= min(7 - i % 8, 7 - i / 8); j++)
					if (square[i + j * 9] == WHITE_BISHOP || square[i + j * 9] == WHITE_QUEEN)
						return true;
					else if (square[i + j * 9] != EMPTY)
						break;

				for (int j = 1; j <= i % 8; j++)
					if (square[i - j] == WHITE_ROOK || square[i - j] == WHITE_QUEEN)
						return true;
					else if (square[i - j] != EMPTY)
						break;
				for (int j = 1; j <= 7 - i % 8; j++)
					if (square[i + j] == WHITE_ROOK || square[i + j] == WHITE_QUEEN)
						return true;
					else if (square[i + j] != EMPTY)
						break;
				for (int j = 1; j <= i / 8; j++)
					if (square[i - j * 8] == WHITE_ROOK || square[i - j * 8] == WHITE_QUEEN)
						return true;
					else if (square[i - j * 8] != EMPTY)
						break;
				for (int j = 1; j <= 7 - i / 8; j++)
					if (square[i + j * 8] == WHITE_ROOK || square[i + j * 8] == WHITE_QUEEN)
						return true;
					else if (square[i + j * 8] != EMPTY)
						break;

				if (i % 8 != 0)
				{
					if (square[i - 1] == WHITE_KING)
						return true;
					if (i / 8 != 0)
						if (square[i - 9] == WHITE_KING)
							return true;
					if (i / 8 != 7)
						if (square[i + 7] == WHITE_KING)
							return true;
				}
				if (i % 8 != 7)
				{
					if (square[i + 1] == WHITE_KING)
						return true;
					if (i / 8 != 0)
						if (square[i - 7] == WHITE_KING)
							return true;
					if (i / 8 != 7)
						if (square[i + 9] == WHITE_KING)
							return true;
				}
				if (i / 8 != 0)
				{
					if (square[i - 8] == WHITE_KING)
						return true;
				}
				if (i / 8 != 7)
				{
					if (square[i + 8] == WHITE_KING)
						return true;
				}

				break;
			}

	return false;
}

bool Board::isMoveLegal(char start, char end, char playerColor)
{
	if (pieceColor(square[start]) == static_cast<char>(playerColor) && pieceColor(square[end]) != playerColor)
	{
		char enPassantBuffer = enPassant;
		bool whiteKingMovedBuffer = whiteKingMoved;
		bool whiteArookMovedBuffer = whiteArookMoved;
		bool whiteHrookMovedBuffer = whiteHrookMoved;
		bool blackKingMovedBuffer = blackKingMoved;
		bool blackArookMovedBuffer = blackArookMoved;
		bool blackHrookMovedBuffer = blackHrookMoved;
		int buffer[64];
		for (int i = 0; i < 64; i++)
			buffer[i] = square[i];

		switch (square[start])
		{
		case WHITE_PAWN:
			if (start + 7 == end && start % 8 != 0)
			{
				if (pieceColor(square[end]) != BLACK && (enPassant != start % 8 - 1 || start / 8 != 4))
					return false;
			}
			else if (start + 9 == end && start % 8 != 7)
			{
				if (pieceColor(square[end]) != BLACK && (enPassant != start % 8 + 1 || start / 8 != 4))
					return false;
			}
			else if (start % 8 != end % 8)
				return false;
			else if (end < start)
				return false;
			else if (end / 8 - start / 8 > 2)
				return false;
			else if (end / 8 - start / 8 == 2 && (start / 8 != 1 || square[end] != EMPTY || square[end - 8] != EMPTY))
				return false;
			else if (square[end] != EMPTY)
				return false;
			break;
		case WHITE_KNIGHT:
			if ((abs(start % 8 - end % 8) != 2 || abs(start / 8 - end / 8) != 1) && (abs(start % 8 - end % 8) != 1 || abs(start / 8 - end / 8) != 2))
				return false;
			break;
		case WHITE_BISHOP:
			if (abs(start % 8 - end % 8) != abs(start / 8 - end / 8))
				return false;
			if (start % 8 < end % 8 && start / 8 < end / 8)
			{
				for (int i = 1; i < end % 8 - start % 8; i++)
					if (square[start + i * 9] != EMPTY)
						return false;
			}
			else if (start % 8 > end % 8 && start / 8 < end / 8)
			{
				for (int i = 1; i < start % 8 - end % 8; i++)
					if (square[start + i * 7] != EMPTY)
						return false;
			}
			else if (start % 8 < end % 8 && start / 8 > end / 8)
			{
				for (int i = 1; i < end % 8 - start % 8; i++)
					if (square[start - i * 7] != EMPTY)
						return false;
			}
			else if (start % 8 > end % 8 && start / 8 > end / 8)
			{
				for (int i = 1; i < start % 8 - end % 8; i++)
					if (square[start - i * 9] != EMPTY)
						return false;
			}
			break;
		case WHITE_ROOK:
			if (start % 8 != end % 8 && start / 8 != end / 8)
				return false;
			if (start % 8 < end % 8)
			{
				for (int i = 1; i < end - start; i++)
					if (square[start + i] != EMPTY)
						return false;
			}
			else if (start % 8 > end % 8)
			{
				for (int i = 1; i < start - end; i++)
					if (square[start - i] != EMPTY)
						return false;
			}
			else if (start / 8 < end / 8)
			{
				for (int i = 1; i < (end - start) / 8; i++)
					if (square[start + 8 * i] != EMPTY)
						return false;
			}
			else if (start / 8 > end / 8)
			{
				for (int i = 1; i < (start - end) / 8; i++)
					if (square[start - 8 * i] != EMPTY)
						return false;
			}
			break;
		case WHITE_QUEEN:
			if (start % 8 != end % 8 && start / 8 != end / 8 && abs(start % 8 - end % 8) != abs(start / 8 - end / 8))
				return false;
			if (start % 8 < end % 8 && start / 8 < end / 8)
			{
				for (int i = 1; i < end % 8 - start % 8; i++)
					if (square[start + i * 9] != EMPTY)
						return false;
			}
			else if (start % 8 > end % 8 && start / 8 < end / 8)
			{
				for (int i = 1; i < start % 8 - end % 8; i++)
					if (square[start + i * 7] != EMPTY)
						return false;
			}
			else if (start % 8 < end % 8 && start / 8 > end / 8)
			{
				for (int i = 1; i < end % 8 - start % 8; i++)
					if (square[start - i * 7] != EMPTY)
						return false;
			}
			else if (start % 8 > end % 8 && start / 8 > end / 8)
			{
				for (int i = 1; i < start % 8 - end % 8; i++)
					if (square[start - i * 9] != EMPTY)
						return false;
			}
			else if (start % 8 < end % 8)
			{
				for (int i = 1; i < end - start; i++)
					if (square[start + i] != EMPTY)
						return false;
			}
			else if (start % 8 > end % 8)
			{
				for (int i = 1; i < start - end; i++)
					if (square[start - i] != EMPTY)
						return false;
			}
			else if (start / 8 < end / 8)
			{
				for (int i = 1; i < (end - start) / 8; i++)
					if (square[start + 8 * i] != EMPTY)
						return false;
			}
			else if (start / 8 > end / 8)
			{
				for (int i = 1; i < (start - end) / 8; i++)
					if (square[start - 8 * i] != EMPTY)
						return false;
			}
			break;
		case WHITE_KING:
			if (start == 4 && end == 2 && !whiteKingMoved && !whiteArookMoved && square[3] == EMPTY && square[1] == EMPTY && !isKingUnderCheck(playerColor))
			{
				move(4, 3);
				if (isKingUnderCheck(playerColor))
				{
					enPassant = enPassantBuffer;
					whiteKingMoved = whiteKingMovedBuffer;
					whiteArookMoved = whiteArookMovedBuffer;
					whiteHrookMoved = whiteHrookMovedBuffer;
					blackKingMoved = blackKingMovedBuffer;
					blackArookMoved = blackArookMovedBuffer;
					blackHrookMoved = blackHrookMovedBuffer;
					for (int i = 0; i < 64; i++)
						square[i] = buffer[i];

					return false;
				}
				else
				{
					enPassant = enPassantBuffer;
					whiteKingMoved = whiteKingMovedBuffer;
					whiteArookMoved = whiteArookMovedBuffer;
					whiteHrookMoved = whiteHrookMovedBuffer;
					blackKingMoved = blackKingMovedBuffer;
					blackArookMoved = blackArookMovedBuffer;
					blackHrookMoved = blackHrookMovedBuffer;
					for (int i = 0; i < 64; i++)
						square[i] = buffer[i];
				}

				break;
			}
			else if (start == 4 && end == 6 && !whiteKingMoved && !whiteHrookMoved && square[5] == EMPTY && !isKingUnderCheck(playerColor))
			{
				move(4, 6);
				if (isKingUnderCheck(playerColor))
				{
					enPassant = enPassantBuffer;
					whiteKingMoved = whiteKingMovedBuffer;
					whiteArookMoved = whiteArookMovedBuffer;
					whiteHrookMoved = whiteHrookMovedBuffer;
					blackKingMoved = blackKingMovedBuffer;
					blackArookMoved = blackArookMovedBuffer;
					blackHrookMoved = blackHrookMovedBuffer;
					for (int i = 0; i < 64; i++)
						square[i] = buffer[i];

					return false;
				}
				else
				{
					enPassant = enPassantBuffer;
					whiteKingMoved = whiteKingMovedBuffer;
					whiteArookMoved = whiteArookMovedBuffer;
					whiteHrookMoved = whiteHrookMovedBuffer;
					blackKingMoved = blackKingMovedBuffer;
					blackArookMoved = blackArookMovedBuffer;
					blackHrookMoved = blackHrookMovedBuffer;
					for (int i = 0; i < 64; i++)
						square[i] = buffer[i];
				}

				break;
			}
			if (start - end != -9 && start - end != -8 && start - end != -7 && start - end != -1 && start - end != 1 && start - end != 7 && start - end != 8 && start - end != 9)
				return false;
			break;
		case BLACK_PAWN:
			if (start - 7 == end && start % 8 != 7)
			{
				if (pieceColor(square[end]) != WHITE && (enPassant != start % 8 + 1 || start / 8 != 3))
					return false;
			}
			else if (start - 9 == end && start % 8 != 0)
			{
				if (pieceColor(square[end]) != WHITE && (enPassant != start % 8 - 1 || start / 8 != 3))
					return false;
			}
			else if (start % 8 != end % 8)
				return false;
			else if (end > start)
				return false;
			else if (start / 8 - end / 8 > 2)
				return false;
			else if (start / 8 - end / 8 == 2 && (start / 8 != 6 || square[end] != EMPTY || square[end + 8] != EMPTY))
				return false;
			else if (square[end] != EMPTY)
				return false;
			break;
		case BLACK_KNIGHT:
			if ((abs(start % 8 - end % 8) != 2 || abs(start / 8 - end / 8) != 1) && (abs(start % 8 - end % 8) != 1 || abs(start / 8 - end / 8) != 2))
				return false;
			break;
		case BLACK_BISHOP:
			if (abs(start % 8 - end % 8) != abs(start / 8 - end / 8))
				return false;
			if (start % 8 < end % 8 && start / 8 < end / 8)
			{
				for (int i = 1; i < end % 8 - start % 8; i++)
					if (square[start + i * 9] != EMPTY)
						return false;
			}
			else if (start % 8 > end % 8 && start / 8 < end / 8)
			{
				for (int i = 1; i < start % 8 - end % 8; i++)
					if (square[start + i * 7] != EMPTY)
						return false;
			}
			else if (start % 8 < end % 8 && start / 8 > end / 8)
			{
				for (int i = 1; i < end % 8 - start % 8; i++)
					if (square[start - i * 7] != EMPTY)
						return false;
			}
			else if (start % 8 > end % 8 && start / 8 > end / 8)
			{
				for (int i = 1; i < start % 8 - end % 8; i++)
					if (square[start - i * 9] != EMPTY)
						return false;
			}
			break;
		case BLACK_ROOK:
			if (start % 8 != end % 8 && start / 8 != end / 8)
				return false;
			if (start % 8 < end % 8)
			{
				for (int i = 1; i < end - start; i++)
					if (square[start + i] != EMPTY)
						return false;
			}
			else if (start % 8 > end % 8)
			{
				for (int i = 1; i < start - end; i++)
					if (square[start - i] != EMPTY)
						return false;
			}
			else if (start / 8 < end / 8)
			{
				for (int i = 1; i < (end - start) / 8; i++)
					if (square[start + 8 * i] != EMPTY)
						return false;
			}
			else if (start / 8 > end / 8)
			{
				for (int i = 1; i < (start - end) / 8; i++)
					if (square[start - 8 * i] != EMPTY)
						return false;
			}
			break;
		case BLACK_QUEEN:
			if (start % 8 != end % 8 && start / 8 != end / 8 && abs(start % 8 - end % 8) != abs(start / 8 - end / 8))
				return false;
			if (start % 8 < end % 8 && start / 8 < end / 8)
			{
				for (int i = 1; i < end % 8 - start % 8; i++)
					if (square[start + i * 9] != EMPTY)
						return false;
			}
			else if (start % 8 > end % 8 && start / 8 < end / 8)
			{
				for (int i = 1; i < start % 8 - end % 8; i++)
					if (square[start + i * 7] != EMPTY)
						return false;
			}
			else if (start % 8 < end % 8 && start / 8 > end / 8)
			{
				for (int i = 1; i < end % 8 - start % 8; i++)
					if (square[start - i * 7] != EMPTY)
						return false;
			}
			else if (start % 8 > end % 8 && start / 8 > end / 8)
			{
				for (int i = 1; i < start % 8 - end % 8; i++)
					if (square[start - i * 9] != EMPTY)
						return false;
			}
			else if (start % 8 < end % 8)
			{
				for (int i = 1; i < end - start; i++)
					if (square[start + i] != EMPTY)
						return false;
			}
			else if (start % 8 > end % 8)
			{
				for (int i = 1; i < start - end; i++)
					if (square[start - i] != EMPTY)
						return false;
			}
			else if (start / 8 < end / 8)
			{
				for (int i = 1; i < (end - start) / 8; i++)
					if (square[start + 8 * i] != EMPTY)
						return false;
			}
			else if (start / 8 > end / 8)
			{
				for (int i = 1; i < (start - end) / 8; i++)
					if (square[start - 8 * i] != EMPTY)
						return false;
			}
			break;
		case BLACK_KING:
			if (start == 60 && end == 58 && !blackKingMoved && !blackArookMoved && square[59] == EMPTY && square[57] == EMPTY && !isKingUnderCheck(playerColor))
			{
				move(60, 58);
				if (isKingUnderCheck(playerColor))
				{
					enPassant = enPassantBuffer;
					whiteKingMoved = whiteKingMovedBuffer;
					whiteArookMoved = whiteArookMovedBuffer;
					whiteHrookMoved = whiteHrookMovedBuffer;
					blackKingMoved = blackKingMovedBuffer;
					blackArookMoved = blackArookMovedBuffer;
					blackHrookMoved = blackHrookMovedBuffer;
					for (int i = 0; i < 64; i++)
						square[i] = buffer[i];

					return false;
				}
				else
				{
					enPassant = enPassantBuffer;
					whiteKingMoved = whiteKingMovedBuffer;
					whiteArookMoved = whiteArookMovedBuffer;
					whiteHrookMoved = whiteHrookMovedBuffer;
					blackKingMoved = blackKingMovedBuffer;
					blackArookMoved = blackArookMovedBuffer;
					blackHrookMoved = blackHrookMovedBuffer;
					for (int i = 0; i < 64; i++)
						square[i] = buffer[i];
				}

				break;
			}
			else if (start == 60 && end == 62 && !blackKingMoved && !blackHrookMoved && square[61] == EMPTY && !isKingUnderCheck(playerColor))
			{
				move(60, 62);
				if (isKingUnderCheck(playerColor))
				{
					enPassant = enPassantBuffer;
					whiteKingMoved = whiteKingMovedBuffer;
					whiteArookMoved = whiteArookMovedBuffer;
					whiteHrookMoved = whiteHrookMovedBuffer;
					blackKingMoved = blackKingMovedBuffer;
					blackArookMoved = blackArookMovedBuffer;
					blackHrookMoved = blackHrookMovedBuffer;
					for (int i = 0; i < 64; i++)
						square[i] = buffer[i];

					return false;
				}
				else
				{
					enPassant = enPassantBuffer;
					whiteKingMoved = whiteKingMovedBuffer;
					whiteArookMoved = whiteArookMovedBuffer;
					whiteHrookMoved = whiteHrookMovedBuffer;
					blackKingMoved = blackKingMovedBuffer;
					blackArookMoved = blackArookMovedBuffer;
					blackHrookMoved = blackHrookMovedBuffer;
					for (int i = 0; i < 64; i++)
						square[i] = buffer[i];
				}

				break;
			}
			if (start - end != -9 && start - end != -8 && start - end != -7 && start - end != -1 && start - end != 1 && start - end != 7 && start - end != 8 && start - end != 9)
				return false;
			break;
		}

		move(start, end);
		if (isKingUnderCheck(playerColor))
		{
			enPassant = enPassantBuffer;
			whiteKingMoved = whiteKingMovedBuffer;
			whiteArookMoved = whiteArookMovedBuffer;
			whiteHrookMoved = whiteHrookMovedBuffer;
			blackKingMoved = blackKingMovedBuffer;
			blackArookMoved = blackArookMovedBuffer;
			blackHrookMoved = blackHrookMovedBuffer;
			for (int i = 0; i < 64; i++)
				square[i] = buffer[i];

			return false;
		}
		else
		{
			enPassant = enPassantBuffer;
			whiteKingMoved = whiteKingMovedBuffer;
			whiteArookMoved = whiteArookMovedBuffer;
			whiteHrookMoved = whiteHrookMovedBuffer;
			blackKingMoved = blackKingMovedBuffer;
			blackArookMoved = blackArookMovedBuffer;
			blackHrookMoved = blackHrookMovedBuffer;
			for (int i = 0; i < 64; i++)
				square[i] = buffer[i];

			return true;
		}

		return true;
	}
	else return false;
}

void Board::move(char start, char end, char promotionTo)
{
	if (start % 8 != 0 && enPassant == start % 8 - 1 && (square[start] == WHITE_PAWN && start / 8 == 4 && start + 7 == end || square[start] == BLACK_PAWN && start / 8 == 3 && start - 9 == end))
	{
		enPassant = -1;

		square[end] = square[start];
		square[start] = EMPTY;
		square[start - 1] = EMPTY;
	}
	else if (start % 8 != 7 && enPassant == start % 8 + 1 && (square[start] == WHITE_PAWN && start / 8 == 4 && start + 9 == end || square[start] == BLACK_PAWN && start / 8 == 3 && start - 7 == end))
	{
		enPassant = -1;

		square[end] = square[start];
		square[start] = EMPTY;
		square[start + 1] = EMPTY;
	}
	else if (square[start] == WHITE_KING && start == 4 && end == 2)
	{
		square[4] = EMPTY;
		square[2] = WHITE_KING;
		square[0] = EMPTY;
		square[3] = WHITE_ROOK;
		whiteKingMoved = true;

		if (enPassant != -1)
			enPassant = -1;
	}
	else if (square[start] == WHITE_KING && start == 4 && end == 6)
	{
		square[4] = EMPTY;
		square[6] = WHITE_KING;
		square[7] = EMPTY;
		square[5] = WHITE_ROOK;
		whiteKingMoved = true;

		if (enPassant != -1)
			enPassant = -1;
	}
	else if (square[start] == BLACK_KING && start == 60 && end == 58)
	{
		square[60] = EMPTY;
		square[58] = BLACK_KING;
		square[56] = EMPTY;
		square[59] = BLACK_ROOK;
		blackKingMoved = true;

		if (enPassant != -1)
			enPassant = -1;
	}
	else if (square[start] == BLACK_KING && start == 60 && end == 62)
	{
		square[60] = EMPTY;
		square[62] = BLACK_KING;
		square[63] = EMPTY;
		square[61] = BLACK_ROOK;
		blackKingMoved = true;

		if (enPassant != -1)
			enPassant = -1;
	}
	else
	{
		if ((square[start] == WHITE_PAWN || square[start] == BLACK_PAWN) && abs(start / 8 - end / 8) == 2)
			enPassant = start % 8;
		else if (enPassant != -1)
			enPassant = -1;

		if (!whiteArookMoved && (start == 0 || end == 0))
			whiteArookMoved = true;
		else if (!whiteHrookMoved && (start == 7 || end == 7))
			whiteHrookMoved = true;
		else if (!blackArookMoved && (start == 56 || end == 56))
			blackArookMoved = true;
		else if (!blackHrookMoved && (start == 63 || end == 63))
			blackHrookMoved = true;
		else if (!whiteKingMoved && start == 4)
			whiteKingMoved = true;
		else if (!blackKingMoved && start == 60)
			blackKingMoved = true;

		if (square[start] == WHITE_PAWN && end / 8 == 7)
		{
			if (promotionTo == 0)
				square[end] = WHITE_QUEEN;
			else if (promotionTo == 1)
				square[end] = WHITE_ROOK;
			else if (promotionTo == 2)
				square[end] = WHITE_BISHOP;
			else if (promotionTo == 3)
				square[end] = WHITE_KNIGHT;
		}
		else if (square[start] == BLACK_PAWN && end / 8 == 0)
		{
			if (promotionTo == 0)
				square[end] = BLACK_QUEEN;
			else if (promotionTo == 1)
				square[end] = BLACK_ROOK;
			else if (promotionTo == 2)
				square[end] = BLACK_BISHOP;
			else if (promotionTo == 3)
				square[end] = BLACK_KNIGHT;
		}
		else square[end] = square[start];
		square[start] = EMPTY;
	}
}

char Board::getSquare(char number)
{
	return square[number];
}