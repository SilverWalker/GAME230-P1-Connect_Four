#include<iostream>

using namespace std;

int** board;
int rowSize = 7;
int columnSize = 6;
int winRequired = 4;
int step = 0;
char discType[3] = { '.', 'O', 'X' };	//[0]: empty, [1]: P1, [2]: p2

bool gameEnd = false;
int winner = 0;							//1: P1, 2: P2, 3: Tie
bool p1Turn = true;
bool quitGame = false;

bool setWrapMode = false;
bool setPopMode = false;

void resetGame();
void displayBoard();
int getValidInput(int min, int max);
void insertDisc();
void checkGameEnd(int discRow, int discCol);
bool checkLine(int discRow, int discCol, char direction);
bool checkCanPop();
void popDisc();
void checkPopGameEnd(int popedRow);

int main()
{
	while (!quitGame) {
		resetGame();

		while (!gameEnd) {
			displayBoard();
			if (setPopMode && checkCanPop()) {
				if (step == rowSize * columnSize) {
					popDisc();
				}
				else {
					cout << "Player " << (p1Turn ? discType[1] : discType[2]) << "'s turn! Do you want to insert disc or remove disc from the bottom? [1]insert [2]remove: ";
					(getValidInput(1, 2) == 1) ? insertDisc() : popDisc();
				}
			}
			else {
				insertDisc();
			}
		}

		switch (winner) {
		case 1:
			cout << "Player O has won the game!" << endl;
			break;
		case 2:
			cout << "Player X has won the game!" << endl;
			break;
		case 3:
			cout << "Tie game!" << endl;
			break;
		}
		cout << "Game over! Do you want to play again? [1]Yes [2]No: ";
		quitGame = ((getValidInput(1, 2) == 2) ? true : false);
	}
	return 0;
}

void resetGame()
{
	step = 0;
	gameEnd = false;
	p1Turn = true;

	cout << "Insert row number(4~20): ";
	rowSize = getValidInput(4, 20);
	cout << "Insert column number(4~20): ";
	columnSize = getValidInput(4, 20);
	int maxWinRequired = rowSize >= columnSize ? columnSize : rowSize;
	cout << "Insert number of winning requirement(3~" << maxWinRequired << "): ";
	winRequired = getValidInput(3, maxWinRequired);
	board = new int* [rowSize];
	for (int i = 0; i < rowSize; i++) {
		board[i] = new int[columnSize];
		for (int j = 0; j < columnSize; j++) {
			board[i][j] = 0;
		}
	}
	cout << "Enable wrap around mode? [1]Yes [2]No: ";
	setWrapMode = ((getValidInput(1, 2) == 1) ? true : false);
	cout << "Enable pop out mode? [1]Yes [2]No: ";
	setPopMode = ((getValidInput(1, 2) == 1) ? true : false);
	cout << "Game start with wrap around mode " << (setWrapMode ? "ON" : "OFF") << ", pop out mode " << (setPopMode ? "ON" : "OFF") << endl;
	cout << "Winning requirement is " << winRequired << " in a row!" << endl;
}

void displayBoard()
{
	for (int num = 0; num < rowSize; num++) {
		cout << num + 1;
	}
	cout << endl;
	for (int displayRow = columnSize - 1; displayRow >= 0; displayRow--) {
		for (int displayCol = 0; displayCol < rowSize; displayCol++) {
			cout << discType[board[displayCol][displayRow]];
		}
		cout << endl;
	}
	/*cout << endl;
	for (int i = 0; i < rowSize;i++) {
		for (int j = 0; j < columnSize;j++) {
			cout << board[i][j];
		}
		cout << endl;
	}*/
}

int getValidInput(int min, int max)
{
	int num = -1;
	while (!(cin >> num) || num < min || num > max) {
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "Invalid input. Please try again: ";
	}
	return num;
}

void insertDisc()
{
	bool isRowFull = true;
	int selectedRow = -1;
	int insertedCol = -1;
	cout << "Player " << (p1Turn ? discType[1] : discType[2]) << "'s turn! Type the column number to insert a piece: ";
	while (isRowFull) {
		selectedRow = getValidInput(1, rowSize) - 1;
		if (board[selectedRow][columnSize - 1] != 0) {
			cout << "That column is full. Please try a different column: ";
		}
		else {
			isRowFull = false;
		}
	}
	for (int i = 0; i < columnSize; i++) {
		if (board[selectedRow][i] == 0) {
			board[selectedRow][i] = (p1Turn ? 1 : 2);
			insertedCol = i;
			break;
		}
	}
	step++;
	checkGameEnd(selectedRow, insertedCol);
}

void checkGameEnd(int discRow, int discCol)
{
	if (step == rowSize * columnSize && !setPopMode) {
		gameEnd = true;
		winner = 3;
		return;
	}
	if (checkLine(discRow, discCol, 'v')|| checkLine(discRow, discCol, 'h')|| checkLine(discRow, discCol, 'p')|| checkLine(discRow, discCol, 'm')) {
		gameEnd = true;
		winner = (p1Turn ? 1 : 2);
		return;
	}
	else {
		p1Turn = !p1Turn;
	}
}

bool checkLine(int discRow, int discCol, char direction)
{
	int currentPlayer = board[discRow][discCol];
	if (currentPlayer == 0) return false;
	int lineCount = 0;
	bool breakFlag1 = false;
	bool breakFlag2 = false;
	if (!setWrapMode) {
		for (int i = 1; i < winRequired; i++) {
			switch (direction) {
				//vertical
			case 'v':
				(discCol - i < 0 || board[discRow][discCol - i] != currentPlayer) ? breakFlag1 = true : lineCount++;
				break;
				//horizontal
			case 'h':
				(discRow - i < 0 || board[discRow - i][discCol] != currentPlayer) ? breakFlag1 = true : lineCount++;
				break;
				//diagonal(+)
			case 'p':
				(discRow - i < 0 || discCol - i < 0 || board[discRow - i][discCol - i] != currentPlayer) ? breakFlag1 = true : lineCount++;
				break;
				//diagonal(-)
			case 'm':
				(discRow - i < 0 || discCol + i > columnSize || board[discRow - i][discCol + i] != currentPlayer) ? breakFlag1 = true : lineCount++;
				break;
			}
			if (breakFlag1) break;
		}
		for (int i = 1; i < winRequired; i++) {
			switch (direction) {
			case 'v':
				(discCol + i >= columnSize || board[discRow][discCol + i] != currentPlayer) ? breakFlag2 = true : lineCount++;
				break;
			case 'h':
				(discRow + i >= rowSize || board[discRow + i][discCol] != currentPlayer) ? breakFlag2 = true : lineCount++;
				break;
			case 'p':
				(discRow + i >= rowSize || discCol + i > columnSize || board[discRow + i][discCol + i] != currentPlayer) ? breakFlag2 = true : lineCount++;
				break;
			case 'm':
				(discRow + i >= rowSize || discCol - i < 0 || board[discRow + i][discCol - i] != currentPlayer) ? breakFlag2 = true : lineCount++;
				break;
			}
			if (breakFlag2) break;
		}
		//cout << (p1Turn ? 1 : 2) << "'s turn C2, direction: " << direction << " count: " << lineCount << endl;
	}
	else {
		for (int i = 1; i < winRequired; i++) {
			switch (direction) {
			case 'v':
				(discCol - i < 0 || board[discRow][discCol - i] != currentPlayer) ? breakFlag1 = true : lineCount++;
				break;
			case 'h':
				(board[(discRow + rowSize - i) % rowSize][discCol] != currentPlayer) ? breakFlag1 = true : lineCount++;
				break;
			case 'p':
				(discCol - i < 0 || board[(discRow + rowSize - i) % rowSize][discCol - i] != currentPlayer) ? breakFlag1 = true : lineCount++;
				break;
			case 'm':
				(discCol + i > columnSize || board[(discRow + rowSize - i) % rowSize][discCol + i] != currentPlayer) ? breakFlag1 = true : lineCount++;
				break;
			}
			if (breakFlag1) break;
		}
		for (int i = 1; i < winRequired; i++) {
			switch (direction) {
			case 'v':
				(discCol + i >= columnSize || board[discRow][discCol + i] != currentPlayer) ? breakFlag2 = true : lineCount++;
				break;
			case 'h':
				(board[(discRow + i) % rowSize][discCol] != currentPlayer) ? breakFlag2 = true : lineCount++;
				break;
			case 'p':
				(discCol + i > columnSize || board[(discRow + i) % rowSize][discCol + i] != currentPlayer) ? breakFlag2 = true : lineCount++;
				break;
			case 'm':
				(discCol - i < 0 || board[(discRow + i) % rowSize][discCol - i] != currentPlayer) ? breakFlag2 = true : lineCount++;
				break;
			}
			if (breakFlag2) break;
		}
		//cout << (p1Turn ? 1 : 2) << "'s turn C2, direction: " << direction << " count: " << lineCount << endl;
	}
	return (lineCount >= winRequired - 1) ? true : false;
}

bool checkCanPop() {
	int currentPlayer = (p1Turn ? 1 : 2);
	for (int i = 0; i < rowSize; i++) {
		if (board[i][0] == currentPlayer) return true;
	}
	return false;
}

void popDisc() {
	int currentPlayer = (p1Turn ? 1 : 2);
	int selectedRow = -1;
	bool canPop = false;
	while (!canPop) {
		cout << "Player " << (p1Turn ? discType[1] : discType[2]) << "'s turn! Type the column number to remove a piece of yours: ";
		selectedRow = getValidInput(1, rowSize) - 1;
		if (board[selectedRow][0] != currentPlayer) {
			cout << "The bottom piece isn't yours. Please try a different column." << endl;
		}
		else {
			canPop = true;
		}
	}
	for (int i = 0; i < columnSize-1; i++) {
		board[selectedRow][i] = board[selectedRow][i + 1];
	}
	board[selectedRow][columnSize - 1] = 0;
	step--;
	checkPopGameEnd(selectedRow);
}

void checkPopGameEnd(int popedRow){
	bool p1Win = false;
	bool p2Win = false;
	for (int i = 0; i < columnSize - 1; i++) {
		if (checkLine(popedRow, i, 'v') || checkLine(popedRow, i, 'h') || checkLine(popedRow, i, 'p') || checkLine(popedRow, i, 'm')) {
			board[popedRow][i] == 1 ? (p1Win = true) : (p2Win = true);
		}
	}
	if (p1Win||p2Win) {
		gameEnd = true;
		winner = p1Win && p2Win ? 3 : (p1Win ? 1 : 2);
	}
	else {
		p1Turn = !p1Turn;
	}
}