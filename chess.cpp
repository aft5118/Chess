#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

const int NUM_ROWS = 8;
const int NUM_COLS = 8;


// **************** CLASS: CELL *******************
class Cell {
	char piece;
	char color;
public:
	Cell();
	void place(char color, char piece);
	string take();
	string getPiece();
	string look();
};

Cell::Cell() {
	piece = ' ';
	color = ' ';
}

string Cell::look() {
	string result = "";
	result = result.append(1, color);
	result = result.append(1, piece);
	return result;
}

string Cell::take() {
	string result = look();
	piece = ' ';
	color = ' ';
	return result;
}

void Cell::place(char newColor, char newPiece) {
	assert((newColor == 'W') || (newColor == 'B') || (newColor == ' '));
	color = newColor;
	assert((newPiece == 'R') || (newPiece == 'K') || (newPiece == 'B') || (newPiece == 'Q') || (newPiece == 'K') || (newPiece == 'N') || (newPiece == 'P'));
	piece = newPiece;

}

string Cell::getPiece() {
	string result = "";
	result = result.append(1, color);
	result = result.append(1, piece);
	return result;
}

// **************** CLASS: BOARD *******************
class Board {
	Cell board[NUM_ROWS][NUM_COLS];
	bool debug = true;
	void displayLine();
	bool moveRook(int row, int col, string direction, int steps);
	bool movePawn(int row, int col, string direction, int steps);
	bool moveBishop(int row, int col, string direction, int steps);
	bool moveQueen(int row, int col, string direction, int steps);
	bool moveKing(int row, int col, string direction, int steps);
	bool moveKnight(int row, int col, string direction, int steps);
	bool canTake(int r, int c, char kingColor);
	bool kingOnBoard(char color);
	bool place(int, int, char, char);
	bool movePieceOneStep(string, string, int&, int&, bool &);
	void buffer(string);


public:
	Board(string = "WITHPAWNS");
	Board(string, int, int, char, char); // Constructor for board with only one piece
	void displayBoard();
	string take(int, int);
	bool cellEmpty(int, int);
	string look(int, int);
	bool movePiece(int row, int col, string direction, int steps);
	bool turn(int row, int col, string direction, int numSpaces);
	bool checkForCheck(char color);
	bool makeMove(char color);

};
bool Board::cellEmpty(int row, int col) {
	if (board[row][col].getPiece() == "  ")
		return true;
	return false;
}

bool Board::place(int row, int col, char color, char peice) {
	if (((row >= 0) && (row < NUM_ROWS)) && ((col >= 0) && (col < NUM_COLS))) {
		board[row][col].place(color, peice);
		return true;
	}
	return false;

}

string Board::look(int row, int col) {
	if (((row >= 0) && (row < NUM_ROWS)) && ((col >= 0) && (col < NUM_COLS)))
		return board[row][col].look();
	return "  ";
}
string Board::take(int row, int col) {
	assert((row >= 0) && (row < NUM_ROWS));
	assert((col >= 0) && (col < NUM_COLS));
	return board[row][col].take();

}
Board::Board(string command, int row, int col, char color, char piece) {
	if (command == "ONEPIECE") {
		board[row][col].place(color, piece);
		return;
	}
}
Board::Board(string command) {
	board[0][0].place('B', 'R');
	board[0][1].place('B', 'N');
	board[0][2].place('B', 'B');
	board[0][3].place('B', 'Q');
	board[0][4].place('B', 'K');
	board[0][5].place('B', 'B');
	board[0][6].place('B', 'N');
	board[0][7].place('B', 'R');
	if (command != "NOPAWNS") {
		for (int c = 0; c < NUM_COLS; c++) {
			board[1][c].place('B', 'P');
		}
	}

	board[NUM_ROWS - 1][0].place('W', 'R');
	board[NUM_ROWS - 1][1].place('W', 'N');
	board[NUM_ROWS - 1][2].place('W', 'B');
	board[NUM_ROWS - 1][4].place('W', 'K');
	board[NUM_ROWS - 1][3].place('W', 'Q');
	board[NUM_ROWS - 1][5].place('W', 'B');
	board[NUM_ROWS - 1][6].place('W', 'N');
	board[NUM_ROWS - 1][7].place('W', 'R');
	if (command != "NOPAWNS") {
		for (int c = 0; c < NUM_COLS; c++) {
			board[NUM_COLS - 2][c].place('W', 'P');
		}
	}

}
void Board::displayLine() {
	cout << endl;
	for (int x = 0; x <= NUM_COLS; x++) {
		cout << "    | ";
	}
	cout << endl;
	for (int x = 0; x <= NUM_COLS; x++) {
		cout << "----| ";
	}
	cout << endl;

}

void Board::displayBoard() {
	cout << endl << "CURRENT BOARD:" << endl << endl;
	cout << "    | ";
	for (int c = 0; c < NUM_COLS; c++) {
		cout << c << "   | ";
	}
	displayLine();

	char translate[NUM_ROWS] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
	for (int r = 0; r < NUM_ROWS; r++) {
		cout << translate[r] << "   | ";
		for (int c = 0; c < NUM_COLS; c++) {
			cout << " " << board[r][c].getPiece() << " | ";
		}
		displayLine();
	}
	cout << endl << endl;
}

bool Board::movePieceOneStep(string piece, string direction, int &row, int &col, bool &tookOne) {
	assert((row >= 0) && (row < NUM_ROWS));
	assert((col >= 0) && (col < NUM_COLS));
	int toRow = row;
	int toCol = col;
	if (direction == "S")
		toRow = row + 1;
	else if (direction == "N")
		toRow = row - 1;
	else if (direction == "E")
		toCol = col + 1;
	else if (direction == "W")
		toCol = col - 1;
	else if (direction == "NW") {
		toRow = row - 1;
		toCol = col - 1;
	}
	else if (direction == "NE") {
		toRow = row - 1;
		toCol = col + 1;
	}
	else if (direction == "SW") {
		toRow = row + 1;
		toCol = col - 1;
	}
	else if (direction == "SE") {
		toRow = row + 1;
		toCol = col + 1;
	}
	else {
		if (debug) cout << "INVALID DIRECTION!" << endl;
		return false;
	}

	if ((toRow < 0) || (toRow >= NUM_ROWS)) {
		if (debug) cout << "Out of bounds!" << endl;
		return false;
	}
	if ((toCol < 0) || (toCol >= NUM_COLS)) {
		if (debug) cout << "Out of bounds!" << endl;
		return false;
	}
	tookOne = false;
	if (!cellEmpty(toRow, toCol)) {
		string movePiece = look(row, col);
		string hitPiece = look(toRow, toCol);
		if (hitPiece.at(0) == movePiece.at(0)) {
			if (debug) cout << "You can not take your own piece! (" << hitPiece << ")" << endl;
			return false;
		}
		tookOne = true;
	}

	piece = take(row, col);
	place(toRow, toCol, piece.at(0), piece.at(1));
	row = toRow;
	col = toCol;

	return true;
}

bool Board::moveQueen(int row, int col, string direction, int steps) {
	string piece = look(row, col);
	assert(piece.at(1) == 'Q');
	bool tookOne = false;
	for (int x = 0; x < steps; x++) {
		if (!movePieceOneStep(piece, direction, row, col, tookOne))
			return false;
		if (tookOne)
			break;
	}
	return true;
}

bool Board::moveKing(int row, int col, string direction, int steps) {
	string piece = look(row, col);
	assert(piece.at(1) == 'K');

	if (steps > 1) {
		if (debug) cout << "Kings can not move " << steps << " steps at a time!" << endl;
		return false;
	}
	bool tookOne = false;
	if (!movePieceOneStep(piece, direction, row, col, tookOne))
		return false;

	return true;
}

bool Board::moveBishop(int row, int col, string direction, int steps) {
	string piece = look(row, col);
	assert(piece.at(1) == 'B');

	if (!((direction == "NE") || (direction == "SE") || (direction == "NW") || (direction == "SW"))) {
		if (debug) cout << "Bishops can not move " << direction << "!" << endl;
		return false;
	}
	bool tookOne = false;
	for (int x = 0; x < steps; x++) {
		if (!movePieceOneStep(piece, direction, row, col, tookOne))
			return false;
		if (tookOne)
			break;
	}
	return true;
}

bool Board::moveRook(int row, int col, string direction, int steps) {
	string piece = look(row, col);
	assert(piece.at(1) == 'R');

	if (!((direction == "N") || (direction == "S") || (direction == "W") || (direction == "E"))) {
		if (debug) cout << "Rooks can not move " << direction << "!" << endl;
		return false;
	}
	bool tookOne = false;
	for (int x = 0; x < steps; x++) {
		if (!movePieceOneStep(piece, direction, row, col, tookOne))
			return false;
		if (tookOne)
			break;
	}
	return true;
}
bool Board::moveKnight(int row, int col, string direction, int steps) {
	string piece = look(row, col);
	assert(piece.at(1) == 'N');

	if (steps > 1) {
		if (debug) cout << "Knights can not move " << steps << " steps at a time!" << endl;
		return false;
	}
	int toRow = row;
	int toCol = col;
	if (direction == "NNE") {
		toRow -= 2;
		toCol += 1;
	}
	else if (direction == "NEE") {
		toRow -= 1;
		toCol += 2;
	}
	else if (direction == "SEE") {
		toRow += 1;
		toCol += 2;
	}
	else if (direction == "SSE") {
		toRow += 2;
		toCol += 1;
	}
	else if (direction == "SSW") {
		toRow += 2;
		toCol -= 1;
	}
	else if (direction == "SWW") {
		toRow += 1;
		toCol -= 2;
	}
	else if (direction == "NWW") {
		toRow -= 1;
		toCol -= 2;
	}
	else if (direction == "NNW") {
		toRow -= 2;
		toCol -= 1;
	}
	else {
		if (debug) cout << "Knights can not move " << direction << "!" << endl;
		return false;
	}

	if (!cellEmpty(toRow, toCol)) {
		string movePiece = look(row, col);
		string hitPiece = look(toRow, toCol);
		if (hitPiece.at(0) == movePiece.at(0)) {
			if (debug) cout << "You can not take your own piece! (" << hitPiece << ")" << endl;
			return false;
		}
	}

	piece = take(row, col);
	place(toRow, toCol, piece.at(0), piece.at(1));

	return true;
}

bool Board::movePawn(int row, int col, string direction, int steps) {
	string piece = look(row, col);
	assert(piece.at(1) == 'P');

	if (piece.at(0) == 'W'){
		if ((direction == "NW") && (steps == 1)) {
			int newRow = row - 1;
			int newCol = col - 1;
			string takeMe = look(newRow, newCol);
			if (takeMe.at(0) != 'B') {
				if (debug) cout << "White pawns can only take black pieces on a diagonal!" << endl;
				return false;
			}
		}
		else if ((direction == "NE") && (steps == 1)) {
			int newRow = row - 1;
			int newCol = col + 1;
			string takeMe = look(newRow, newCol);
			//printf("Attempting to take %s at [%d,%d]\n", takeMe.c_str(), newRow, newCol);
			if (takeMe.at(0) != 'B') {
				if (debug) cout << "White pawns can only take black pieces on a diagonal!" << endl;
				return false;
			}
		}
		else if (direction == "N") {
			if (steps > 2) {
				if (debug) cout << "Pawns can not move " << steps << " steps at a time!" << endl;
				return false;
			}
			else if (steps == 2) {
				if (row != 6) {
					if (debug) cout << "Pawns can only move two steps on their first moves!" << endl;
					return false;
				}
				string takeMe = look(row - 2, col);
				if (takeMe.at(0) == 'B') {
					if (debug) cout << "Pawns can only take pieces on a diagonal!" << endl;
					return false;
				}
				if (takeMe.at(0) == 'W') {
					if (debug) cout << "You can not run into your own piece!" << endl;
					return false;
				}
			}
			else if (steps == 1) {
				string takeMe = look(row - 1, col);
				if (takeMe.at(0) == 'B') {
					if (debug) cout << "Pawns can only take pieces on a diagonal!" << endl;
					return false;
				}
				if (takeMe.at(0) == 'W') {
					if (debug) cout << "You can not run into your own piece!" << endl;
					return false;
				}
			}
			else {
				; // cout << "White pawns can not move that way!" << endl;
				; // return false;
			}
		}
		else {
			if (debug) cout << "White pawns can not move that way!" << endl;
			return false;
		}
	}
	if (piece.at(0) == 'B'){
		if ((direction == "SW") && (steps == 1)) {
			int newRow = row + 1;
			int newCol = col - 1;
			string takeMe = look(newRow, newCol);
			if (takeMe.at(0) != 'W') {
				if (debug) cout << "Black pawns can only take black pieces on a diagonal!" << endl;
				return false;
			}
		}
		else if ((direction == "SE") && (steps == 1)) {
			int newRow = row + 1;
			int newCol = col + 1;
			string takeMe = look(newRow, newCol);
			if (takeMe.at(0) != 'W') {
				if (debug) cout << "Black pawns can only take black pieces on a diagonal!" << endl;
				return false;
			}
		}
		else if (direction == "S") {
			if (steps > 2) {
				if (debug) cout << "Pawns can not move " << steps << " steps at a time!" << endl;
				return false;
			}
			else if (steps == 2) {
				if (row != 1) {
					if (debug) cout << "Pawns can only move two steps on their first moves!" << endl;
					return false;
				}
				string takeMe = look(row + 2, col);
				if (takeMe.at(0) == 'W') {
					if (debug) cout << "Pawns can only take pieces on a diagonal!" << endl;
					return false;
				}
				if (takeMe.at(0) == 'B') {
					if (debug) cout << "You can not run into your own piece!" << endl;
					return false;
				}
				else if (steps == 1) {
					string takeMe = look(row + 1, col);
					if (takeMe.at(0) == 'W') {
						if (debug) cout << "Pawns can only take pieces on a diagonal!" << endl;
						return false;
					}
					if (takeMe.at(0) == 'B') {
						if (debug) cout << "You can not run into your own piece!" << endl;
						return false;
					}
				}
				else {
					;// cout << "Black pawns can not move that way!" << endl;
					;// return false;
				}
			}
			else
				; // all good
		}
		else {
			if (debug) cout << "Black pawns can not move that way!" << endl;
			return false;
		}
	}
	bool tookOne = false;
	for (int x = 0; x < steps; x++) {
		if (!movePieceOneStep(piece, direction, row, col, tookOne))
			return false;
		if (tookOne)
			break;
	}
	return true;
}

bool Board::movePiece(int row, int col, string direction, int steps) {
	string piece = look(row, col);
	//cout << "*** MOVING " << piece << " FROM [" << row << ", " << col
	//	<< "] Direction:" << direction << " #steps: " << steps << endl;
	if (piece.at(1) == 'R')
		return moveRook(row, col, direction, steps);
	else if (piece.at(1) == 'P')
		return movePawn(row, col, direction, steps);
	else if (piece.at(1) == 'B')
		return moveBishop(row, col, direction, steps);
	else if (piece.at(1) == 'Q')
		return moveQueen(row, col, direction, steps);
	else if (piece.at(1) == 'K')
		return moveKing(row, col, direction, steps);
	else if (piece.at(1) == 'N')
		return moveKnight(row, col, direction, steps);
	else {
		if (debug) cout << "Invalid piece " << piece << " at position [" << row << ", " << col << "]" << endl;
		return false;
	}
	return true;
}

bool Board::turn(int row, int col, string direction, int numSpaces) {
	if ((row < 0) || (row >= NUM_ROWS)) {
		if (debug) cout << "OUT OF BOUNDS!" << endl;
		return false;
	}
	if ((col < 0) || (col >= NUM_COLS)) {
		if (debug) cout << "OUT OF BOUNDS!" << endl;
		return false;
	}
	if (!movePiece(row, col, direction, numSpaces)) {
		return false;
	}

	return true;
}

bool Board::canTake(int r, int c, char kingColor) {
	string direction[12] = { "N", "S", "W", "E", "NE", "NW", "SE", "SW", "NNE", "NNW", "SSE", "SSW" };
	string piece = look(r, c);
	bool canTakeIt;
	Cell boardCopy[NUM_ROWS][NUM_COLS];
	if ((piece.at(0) != ' ') && (piece.at(0) != kingColor)) {
		for (int dir = 0; dir < 12; dir++) {
			for (int steps = 0; steps < (NUM_ROWS > NUM_COLS ? NUM_ROWS : NUM_COLS); steps++) {
				canTakeIt = false;
				// make a copy of the board
				for (int rx = 0; rx < NUM_ROWS; rx++) {
					for (int cx = 0; cx < NUM_COLS; cx++) {
						boardCopy[rx][cx] = board[rx][cx];
					}
				}
				if (movePiece(r, c, direction[dir], steps)) {
					if (!kingOnBoard(kingColor)) {
						canTakeIt = true;
					}
				}
				// put the board back
				for (int rx = 0; rx < NUM_ROWS; rx++) {
					for (int cx = 0; cx < NUM_COLS; cx++) {
						board[rx][cx] = boardCopy[rx][cx];
					}
				}
				if (canTakeIt == true)
					return true;
			}
		}
	}
	return false;
}

bool Board::kingOnBoard(char color) {
	debug = false;
	for (int r = 0; r < NUM_ROWS; r++) {
		for (int c = 0; c < NUM_COLS; c++) {
			string piece = look(r, c);
			if ((piece.at(0) == color) && (piece.at(1) == 'K')) {
				return true;
			}
		}
	}
	debug = true;
	return false;
}



bool Board::checkForCheck(char color) {
	string piece;
	for (int r = 0; r < NUM_ROWS; r++) {
		for (int c = 0; c < NUM_COLS; c++) {
			piece = look(r, c);
			if ((piece.at(0) != color || (piece.at(0) != ' '))) {
				if (canTake(r, c, color)){
					cout << "King is in check" << endl;
					return true;
				}
			}
		}
	}
	return false;
}


void Board::buffer(string command) {
	static Cell boardCopy[NUM_ROWS][NUM_COLS];
	if (command == "copy") {
		// make a copy of the board
		for (int rx = 0; rx < NUM_ROWS; rx++) {
			for (int cx = 0; cx < NUM_COLS; cx++) {
				boardCopy[rx][cx] = board[rx][cx];
			}
		}
	}
	else if (command == "paste") {
		// put the copy back into the board
		for (int rx = 0; rx < NUM_ROWS; rx++) {
			for (int cx = 0; cx < NUM_COLS; cx++) {
				board[rx][cx] = boardCopy[rx][cx];
			}
		}
	}
	else
		assert(false); // only copy and paste are valid here 

}

bool Board::makeMove(char color) {
	int row, col, steps;
	string direction;
	string cell;

	buffer("copy");

	printf("\n%s's turn! \n", color == 'W' ? "White" : "Black");
	printf("Enter cell (row/col) to move: ");
	cin >> cell;
	row = cell.at(0) - 'A';
	col = cell.at(1) - '0';
	string piece = look(row, col);
	if (piece.at(0) != color) {
		cout << "Wrong color!";
		return false;
	}
	printf("Enter direction: ");
	cin >> direction;
	printf("Enter number of steps: ");
	cin >> steps;
	bool status = turn(row, col, direction, steps);
	if (checkForCheck(color)) {
		buffer("paste");
		printf("You can not move into or stay in check!\n");
		displayBoard();
		return false;
	}
	displayBoard();
	return status;
}

int main() {
	int counter;
	Board board("NOPAWNS");
	string goOn = "";
	cout << "BEGINNING BOARD:" << endl;
	board.displayBoard();
	while ((goOn != "N") && (goOn != "n")) {
		int step = 0;
		board.checkForCheck('W');
		counter = 0;
		while (!board.makeMove('W')) {
			if (counter++ > 3) {
				cout << "Out of tries! White loses!\n";
				goOn = "N";
				break;
			}
		}
		if (goOn != "N") {
			board.checkForCheck('B');
			counter = 0;
			while (!board.makeMove('B')) {
				if (counter++ > 3) {
					cout << "Out of tries! Black loses!\n";
					goOn = "N";
					break;
				}
			}
		}
		cout << "Continue? (Y/N) ";
		cin >> goOn;
	}

	cout << "ENDING BOARD:" << endl;
	board.displayBoard();
	return 0;
}
