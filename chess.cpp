#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
using namespace std;

class Piece;

class Pair // Class used to represent a position on the Chessboard
{
public:
	int x;
	int y; 
	Pair(int a, int b) : x(a), y(b) {}
};

class Path // Class to hold the Path in which a Piece is being moved
{
public:
	vector<Pair> route; // Stores the Path as a list of positions
	int status;			// Indicates whether the Path is valid (1) or empty (0)
	Path() { status = 0; }

	// Function to add positions to the Path
	void build(int a, int b)
	{
		Pair temp(a, b);
		route.push_back(temp);
	}

	// Reset the Path, clearing previously added positions
	void reset()
	{
		route.clear();
		status = 0;
	}
};

class UndoObj // Format used when bacKing up a move for undo operation
{
public:
	int x, y;	// Initial position
	int fx, fy; // Final position
	Piece *loc; // Pointer to the Piece being moved
	UndoObj(Piece *temp, int a, int b, int c, int d) : loc(temp), x(a), y(b), fx(c), fy(d) {}
};

class Piece // Base class containing data common to all chess Pieces
{
public:
	int x, y, points, color;			   // 1=>white, 2=>black  // 0->white, 1->black
	int dir, org_x, org_y, has_been_moved; // dir->direction
	void (*disp)(int, int, int);

	/* checkmove is different for every class as every Piece moves differently (Chess rules).
	   It is declared as virtual so that it should be redefined in every derived class.
	   It returns the Path that the Piece will take in moving from (x, y) --> (fx, fy).
	*/

	virtual Path checkmove(int, int, Piece *[8][8]);

	// Used to clear the Piece in the UI
	void clear();

	// Function pointer for clearing a specific chess box
	void (*clearbox)(int, int);

	// Used to display the Piece in the UI
	void display();

	// Constructor for the Piece class
	Piece(int ix, int iy, int p, int c, int d, void (*displ)(int, int, int), void (*clearb)(int, int));

	// Function to move the Piece from (x, y) to (fx, fy)
	Piece *move(int fx, int fy, Piece *[8][8]);

	virtual ~Piece() {}
};

Piece::Piece(int ix, int iy, int p, int c, int d, void (*displ)(int, int, int), void (*clearb)(int, int))
{
	x = org_x = ix;
	y = org_y = iy;
	dir = d;
	points = p;
	color = c;
	has_been_moved = 0;
	disp = displ;
	clearbox = clearb;

	if (disp != NULL)
		disp(x, y, color);
}

Piece *Piece::move(int fx, int fy, Piece *board[8][8])
{
	Piece *ret = board[fx][fy];

	if (board[fx][fy] != NULL)
		board[fx][fy]->clear(); // Clear the Piece in the display

	board[fx][fy] = board[x][y];

	if (!(x == fx && y == fy))
		board[x][y] = NULL; // Clear the Piece in the chess 2D array

	(*clearbox)(x, y);

	board[fx][fy]->x = fx;
	board[fx][fy]->y = fy;
	board[fx][fy]->has_been_moved = 1;
	board[fx][fy]->display(); // Display the Piece in its final position

	return ret;
}

void Piece::clear()
{
	if (clearbox != NULL)
		(*clearbox)(x, y);
}

void Piece::display()
{
	if (disp != NULL)
		(*disp)(x, y, color);
}

Path Piece::checkmove(int fx, int fy, Piece *board[8][8])
{
	Path ret0, ret;	 // ret0 corresponds to an empty Path, ret corresponds to a non-empty Path
	ret.build(x, y); // Build the Path starting from the current position

	if (fx >= 8 || fx < 0 || fy >= 8 || fy < 0)
		return ret0; // Return an empty Path if the destination is out of bounds

	if (fx == x && fy == y)
		return ret0; // Return an empty Path if the destination is the same as the current position

	if (board[fx][fy] != NULL)
	{
		if (board[fx][fy]->color == color)
			return ret0; // Return an empty Path if there's a Piece of the same color at the destination
	}

	ret.status = 1; // Set the Path status to non-empty

	return ret;
}

class Rook : virtual public Piece
{
public:
	Rook(int ix, int iy, int p, int c, int d, void (*displ)(int, int, int), void (*clearb)(int, int)) : Piece(ix, iy, p, c, d, displ, clearb) {}
	Path checkmove(int, int, Piece *[8][8]);
	~Rook() {}
};

Path Rook::checkmove(int fx, int fy, Piece *board[8][8])
{
	Path ret0, ret = Piece::checkmove(fx, fy, board);
	if (ret.status == 0)
		return ret0;
	if (fx != x && fy != y)
		return ret0;
	// Check all horizontal and vertical positions and build Path if destination is found in correct direction
	for (int i = fy + 1; i < y; i++)
		if (board[x][i] == NULL)
			ret.build(x, i);
		else
			return ret0;
	for (int i = y + 1; i < fy; i++)
		if (board[x][i] == NULL)
			ret.build(x, i);
		else
			return ret0;
	for (int i = fx + 1; i < x; i++)
		if (board[i][y] == NULL)
			ret.build(i, y);
		else
			return ret0;
	for (int i = x + 1; i < fx; i++)
		if (board[i][y] == NULL)
			ret.build(i, y);
		else
			return ret0;
	return ret;
}

class Bishop : virtual public Piece
{
public:
	Bishop(int ix, int iy, int p, int c, int d, void (*displ)(int, int, int), void (*clearb)(int, int)) : Piece(ix, iy, p, c, d, displ, clearb) {}
	Path checkmove(int, int, Piece *[8][8]);
	~Bishop() {}
};

Path Bishop::checkmove(int fx, int fy, Piece *board[8][8])
{
	Path ret0, ret = Piece::checkmove(fx, fy, board);
	if (ret.status == 0)
		return ret0;
	if (!(x + y == fx + fy || x - y == fx - fy))
		return ret0;
	// Check all primary and secondary diagonal positions and build Path if destination is found in correct direction
	for (int i = x + 1, j = y + 1; i < fx && j < fy; i++, j++)
		if (board[i][j] == NULL)
			ret.build(i, j);
		else
			return ret0;
	for (int i = x + 1, j = y - 1; i < fx && j > fy; i++, j--)
		if (board[i][j] == NULL)
			ret.build(i, j);
		else
			return ret0;
	for (int i = x - 1, j = y + 1; i > fx && j < fy; i--, j++)
		if (board[i][j] == NULL)
			ret.build(i, j);
		else
			return ret0;
	for (int i = x - 1, j = y - 1; i > fx && j > fy; i--, j--)
		if (board[i][j] == NULL)
			ret.build(i, j);
		else
			return ret0;
	return ret;
}

class Queen : public Rook, public Bishop
{
public:
	Queen(int ix, int iy, int p, int c, int d, void (*displ)(int, int, int), void (*clearb)(int, int)) : Rook(ix, iy, p, c, d, displ, clearb), Bishop(ix, iy, p, c, d, displ, clearb), Piece(ix, iy, p, c, d, displ, clearb) {}
	Path checkmove(int, int, Piece *[8][8]);
	~Queen() {}
};

Path Queen::checkmove(int fx, int fy, Piece *board[8][8])
{
	/*Queen can move like a Rook and a Bishop.So checKing if move is legal wrt Bishop or Rook
	if legal, the Path obtained is returned as the final Path*/
	Path ret1 = Rook::checkmove(fx, fy, board);
	Path ret2 = Bishop::checkmove(fx, fy, board);
	if (ret1.status == 1)
		return ret1;
	else
		return ret2;
}
class Knight : virtual public Piece
{
public:
	Knight(int ix, int iy, int p, int c, int d, void (*displ)(int, int, int), void (*clearb)(int, int)) : Piece(ix, iy, p, c, d, displ, clearb) {}
	Path checkmove(int, int, Piece *[8][8]);
};

Path Knight::checkmove(int fx, int fy, Piece *board[8][8])
{
	// Knight can move in total 8 possible postions. All are checked. Path consists of only one position
	int pos[8][2] = {{x - 1, y + 2}, {x - 2, y + 1}, {x + 1, y + 2}, {x + 2, y + 1}, {x - 1, y - 2}, {x - 2, y - 1}, {x + 1, y - 2}, {x + 2, y - 1}};
	Path ret0, ret = Piece::checkmove(fx, fy, board);
	if (ret.status == 0)
		return ret0;
	for (int i = 0; i < 8; i++)
		if (fx == pos[i][0] && fy == pos[i][1])
		{
			return ret;
		}
	return ret0;
}

class Pawn : public Piece
{
public:
	Pawn(int ix, int iy, int p, int c, int d, void (*displ)(int, int, int), void (*clearb)(int, int)) : Piece(ix, iy, p, c, d, displ, clearb) {}
	Path checkmove(int, int, Piece *[8][8]);
	void check_promo(int, int, Piece *[8][8]);
};

Path Pawn::checkmove(int fx, int fy, Piece *board[8][8])
{
	/*Pawn can perform capture diagonally and move forward wrt it's side.Double move is allowed if the Pawn
	is moving for the first time */
	Path ret0, ret = Piece::checkmove(fx, fy, board);
	if (ret.status == 0)
		return ret0;
	if (x == org_x && y == org_y) // two step move
		if (abs(fy - y) == 2 && fx == x && board[fx][fy] == NULL && board[fx][y + dir] == NULL)
		{
			ret.build(fx, y + dir);
			return ret;
		}
	if (board[fx][fy] != NULL) // capture diagnol
		if (board[fx][fy]->color != color)
			if ((fx == x + 1 || fx == x - 1) && fy == y + dir)
			{
				return ret;
			}
	if (board[fx][fy] == NULL && fx == x && (fy == y + dir))
	{
		return ret;
	}
	return ret0;
}

class King : public Piece
{
public:
	King(int ix, int iy, int p, int c, int d, void (*displ)(int, int, int), void (*clearb)(int, int)) : Piece(ix, iy, p, c, d, displ, clearb) {}
	Path checkmove(int, int, Piece *[8][8]);
};

Path King::checkmove(int fx, int fy, Piece *board[8][8])
{
	// King can move within a radius of one chess box
	Path ret0, ret = Piece::checkmove(fx, fy, board);
	if (ret.status == 0)
		return ret0;
	if (abs(fx - x) <= 1 && abs(fy - y) <= 1)
	{
		return ret;
	}
	return ret0;
}

class Chessboard
{
public:
	int prev_x, prev_y, select_p, turn;
	vector<UndoObj> prev_list;
	Piece *player[2][16];
	Piece *board[8][8];
	Chessboard(void (*skeleton_b)(int, int), void (*clearb)(int, int), void (*highlightb)(int, int), void (*disp)(), void (*msg)(char *));
	void select(int, int);
	void (*skeleton_box)(int, int);
	void (*clearbox)(int, int);
	void (*highlight)(int, int);
	void setKing(void (*displ)(int, int, int));
	void setQueen(void (*displ)(int, int, int));
	void setBishop(void (*displ)(int, int, int));
	void setRook(void (*displ)(int, int, int));
	void setKnight(void (*displ)(int, int, int));
	void setPawn(void (*displ)(int, int, int));
	void (*dQueen)(int, int, int);
	void (*dKing)(int, int, int);
	void (*dRook)(int, int, int);
	void (*dBishop)(int, int, int);
	void (*dKnight)(int, int, int);
	void (*dPawn)(int, int, int);
	void (*message)(char *);
	void redisplay();
	void remove_Piece(Piece *val, int, int);
	Path check(int);
	void show_list();
	void undo();
	void undo_move();
	int move(int, int);
	int checkmate(int, Path);
	void (*display)();
	void check_promo(int, int);
	void remove(Piece *);
	void add_Piece(Piece *);
};

void Chessboard::undo_move() // move is reversed completely. Invoked for undoing a move on user request.
{
	if (prev_list.size() == 0)
		return;
	undo(); // undoing move in the chess engine.
	skeleton_box(prev_x, prev_y);
	display();
	prev_x = prev_y = 9;
	select_p = 0;
	turn = !turn; // changing turn;
}

void Chessboard::undo() // move is reversed without changing the turn.Used by the chess engine.
{
	if (prev_list.size() == 0)
		return;
	UndoObj temp = prev_list.back();
	if (temp.x == temp.fx && temp.y == temp.fy)
	{
		remove(board[temp.fx][temp.fy]);
		delete board[temp.fx][temp.fy];
		board[temp.fx][temp.fy] = temp.loc;
		add_Piece(temp.loc);
		prev_list.pop_back();
		temp = prev_list.back();
	}
	board[temp.fx][temp.fy]->move(temp.x, temp.y, board);
	board[temp.fx][temp.fy] = temp.loc;
	if (temp.loc != NULL)
	{
		for (int i = 0; i < 16; i++)
			if (player[(temp.loc)->color][i] == NULL)
			{
				player[(temp.loc)->color][i] = temp.loc;
				break;
			}
		board[temp.fx][temp.fy]->display();
	}
	prev_list.pop_back();
}

void Chessboard::add_Piece(Piece *p) // add Piece to player's collection of active Pieces on the chess board
{
	for (int i = 0; i < 16; i++)
		if (player[p->color][i] == NULL)
			player[p->color][i] = p;
}

void Chessboard::check_promo(int fx, int fy) // check for Pawn promotion
{
	if (board[fx][fy]->points == 1 && ((turn == 0 && fy == 7) || (turn == 1 && fy == 0)))
	{
		cout << "CHECK PROMO" << endl;
		int temp1 = prev_x, temp2 = prev_y;
		prev_x = fx;
		prev_y = fy;
		remove_Piece(board[fx][fy], fx, fy);
		prev_x = temp1;
		prev_y = temp2;
		board[fx][fy] = new Queen(fx, fy, 5, turn, turn == 0 ? 1 : -1, dQueen, clearbox);
		board[fx][fy]->clear();
		board[fx][fy]->display();
		add_Piece(board[fx][fy]);
	}
}

void Chessboard::show_list()
{
	UndoObj temp = prev_list.back();
}

void Chessboard::remove(Piece *val) // remove Piece from player's collection of active Pieces on the chess board
{
	if (val != NULL)
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 16; j++)
				if (player[i][j] == val)
				{
					player[i][j] = NULL;
					break;
				}
}

void Chessboard::remove_Piece(Piece *val, int fx, int fy)
{
	UndoObj temp(val, prev_x, prev_y, fx, fy);
	remove(val);

	prev_list.push_back(temp);
}

// Display all Pieces in the board in their respective positions
void Chessboard::redisplay()
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (board[i][j] != 0)
				board[i][j]->display();
}

Chessboard::Chessboard(void (*skeleton_b)(int, int), void (*clearb)(int, int), void (*highlightb)(int, int), void (*disp)(), void (*msg)(char *))
{
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 16; j++)
			player[i][j] = NULL;
	skeleton_box = skeleton_b;
	clearbox = clearb;
	message = msg;
	display = disp;
	select_p = 0;
	turn = 0;
	highlight = highlightb;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			board[i][j] = NULL;
	prev_x = prev_y = 9;
}

Path Chessboard::check(int turnt) // Used to check if the King has been given a check
{
	// Note : only 2 players are there. If turn = 0, then !turn = 1
	/*	This is evaluated by checKing if any Piece of "(!turn)" player can move to the position in which
		the King of the "turn" player is present. If such a move is possible. A check for "turn" player is
		declared. Path of the check is returned
	*/
	Path ret0, ret;
	if (player[turnt][0] != NULL)
	{
		for (int i = 0; i < 16; i++)
		{
			if (player[!turnt][i] != NULL)
			{
				ret = player[!turnt][i]->checkmove(player[turnt][0]->x, player[turnt][0]->y, board);
				if (ret.status == 1)
				{
					char p[] = "CHECK!!";
					message(p);
					return ret;
				}
			}
		}
	}
	return ret0;
}

int Chessboard::move(int x, int y) // used to perform the move in the chess engine,backup move,display message
{
	Path ret;
	if (prev_x == x && prev_y == y)
		return 0;
	ret = board[prev_x][prev_y]->checkmove(x, y, board);
	if (ret.status)
	{
		Piece *tt = board[prev_x][prev_y]->move(x, y, board);
		remove_Piece(tt, x, y);
		// move should be declared invalid if move results in check to own King
		if (check(turn).status)
		{
			undo();
			char p[] = "SELF CHECK!!ILLEGAL MOVE";
			message(p);
			return 0;
		}
		check_promo(x, y);
		ret = check(!turn); // get the Path in which check is occuring. Empty if no check has occured.
		if (ret.status)
		{

			char p[] = "CHECK!!";
			message(p);
			if (checkmate(!turn, ret)) // check if the opponent has been checkmated
			{
				char p[] = "CHECKMATE";
				message(p);
				return 1;
			}
			else
			{
				char p[] = "CHECK!!";
				message(p);
			}
		}
	}
	else
		return 0;
	return 1;
}

int Chessboard::checkmate(int turnt, Path ret) // used to check if a checkmate has occured for "turnt" player
{
	/*
		Checkmate is declared if 3 conditions are fullfilled.
		1) The King under check cannot move to any other position(legally) and still not be under check.
		2) No Piece belonging to the same player as the King can block the check by going on the Path between the
			Piece causing the check and the King.
		3) No Piece belonging to the same player as the King can capture the Piece belonging to the other player
			causing check to the King.

			The second and third points are combined into a single condition by including the position of the
			Piece causing the check into the Path of the check. If  the King can move or if any Piece can be
			moved to this Path, no checkmate has occured.
	*/

	int temp_turn = turn; // bacKing up current  turn
	int x = player[turnt][0]->x;
	int y = player[turnt][0]->y;
	prev_x = x;
	prev_y = y;
	turn = turnt;

	// checKing the first condition in the below code
	int King_move[8][2] = {{x + 1, y}, {x + 1, y + 1}, {x, y + 1}, {x - 1, y + 1}, {x - 1, y}, {x - 1, y - 1}, {x, y - 1}, {x + 1, y - 1}};
	for (int i = 0; i < 8; i++)
		if (move(King_move[i][0], King_move[i][1]) == 1)
		{
			undo();
			turn = temp_turn;
			return 0;
		}

	// ChecKing second and third conditions in the below code
	for (int i = 1; i < 16; i++)
	{
		if (player[turnt][i] == NULL)
			continue;
		for (int j = 0; j < ret.route.size(); j++)
		{
			Path val;
			Pair pos = ret.route.at(j);
			prev_x = player[turnt][i]->x;
			prev_y = player[turnt][i]->y;
			if (move(pos.x, pos.y) == 1)
			{
				undo();
				turn = temp_turn;
				return 0;
			}

			turn = turnt;
		}
	}
	turn = temp_turn;
	return 1;
}

void Chessboard::select(int x, int y)
// input from the UI is fed here. invokes move function and flushes the changes made in the chess engine to the UI
{
	if (select_p == 0)
	{
		if (prev_x == x && prev_y == y || board[x][y] == NULL)
			return;
		if (board[x][y]->color != turn)
			return;
		if (highlight != NULL)
		{
			highlight(x, y);
			display();
		}
		prev_x = x;
		prev_y = y;
		select_p = 1;
		char p[] = "";
		message(p);
	}
	else if (select_p == 1)
	{
		if (prev_x == 9 || prev_y == 9)
			return;
		if (move(x, y) == 0)
		{
			char p[] = "INVALID MOVE!! ";
			message(p);
		}
		else
		{
			turn = !turn;
			// message("");
			display();
		}
		skeleton_box(prev_x, prev_y);
		display();
		prev_x = prev_y = 9;
		select_p = 0;
	}
}

void Chessboard::setKing(void (*displ)(int, int, int))
// Initialise King by using the funtion pointer to display King
{
	dKing = displ;
	board[4][0] = player[0][0] = new King(4, 0, 99, 0, 1, displ, clearbox);
	board[4][7] = player[1][0] = new King(4, 7, 99, 1, -1, displ, clearbox);
}

void Chessboard::setQueen(void (*displ)(int, int, int))
// Initialise Queen by using the funtion pointer to display Queen
{
	dQueen = displ;
	board[3][0] = player[0][1] = new Queen(3, 0, 5, 0, 1, displ, clearbox);
	board[3][7] = player[1][1] = new Queen(3, 7, 5, 1, -1, displ, clearbox);
}

void Chessboard::setBishop(void (*displ)(int, int, int))
// Initialise Bishop by using the funtion pointer to display Bishop
{
	dBishop = displ;
	board[2][0] = player[0][2] = new Bishop(2, 0, 4, 0, 1, displ, clearbox);
	board[5][0] = player[0][3] = new Bishop(5, 0, 4, 0, 1, displ, clearbox);
	board[2][7] = player[1][2] = new Bishop(2, 7, 4, 1, -1, displ, clearbox);
	board[5][7] = player[1][3] = new Bishop(5, 7, 4, 1, -1, displ, clearbox);
}

void Chessboard::setRook(void (*displ)(int, int, int))
// Initialise Rook by using the funtion pointer to display Rook
{
	dRook = displ;
	board[0][0] = player[0][4] = new Rook(0, 0, 2, 0, 1, displ, clearbox);
	board[7][0] = player[0][5] = new Rook(7, 0, 2, 0, 1, displ, clearbox);
	board[0][7] = player[1][4] = new Rook(0, 7, 2, 1, -1, displ, clearbox);
	board[7][7] = player[1][5] = new Rook(7, 7, 2, 1, -1, displ, clearbox);
}

void Chessboard::setKnight(void (*displ)(int, int, int))
{
	// Initialise Knight by using function pointer to display Knight
	dKnight = displ;
	board[1][0] = player[0][6] = new Knight(1, 0, 3, 0, 1, displ, clearbox);
	board[6][0] = player[0][7] = new Knight(6, 0, 3, 0, 1, displ, clearbox);
	board[1][7] = player[1][6] = new Knight(1, 7, 3, 1, -1, displ, clearbox);
	board[6][7] = player[1][7] = new Knight(6, 7, 3, 1, -1, displ, clearbox);
}

void Chessboard::setPawn(void (*displ)(int, int, int))
{
	// Initialise Pawn by using funtion pointer to display Pawn
	dPawn = displ;
	for (int i = 0; i < 8; i++)
		board[i][1] = player[0][i + 8] = new Pawn(i, 1, 1, 0, 1, displ, clearbox);
	for (int i = 0; i < 8; i++)
		board[i][6] = player[1][i + 8] = new Pawn(i, 6, 1, 1, -1, displ, clearbox);
}