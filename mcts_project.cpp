#include <iostream>
#include <cstdlib>
using namespace std;

#define SIZE 8      // Size of game board
#define MAX_RAND_PLAYOUT 10000  // Random playouts for MCTS

void display(char board[][SIZE]);
int validMoves(char board[][SIZE], int moves[][SIZE], char player);
void moveStone(char board[][SIZE], int row, int col, char player);
void pure_mcts_reversi(char board[][SIZE], int moves[][SIZE], char computer, int stonesOnBoard);
void heuristic_mcts_reversi(char board[][SIZE], int moves[][SIZE], char computer, int stonesOnBoard);
int* make_mcts_move(char board[][SIZE], int moves[][SIZE], char player);
int checkWinState(char board[][SIZE], int stonesOnBoard, char player, char computer);


int main() {
    char board[SIZE][SIZE] = {0};
    int moves[SIZE][SIZE] = {0};

    int stonesOnBoard;
    char currentPlayer = 'X';
    char player;
    char pure_mcts;
    char heur_mcts;
    int currMoveRow;
    char currMoveColChar;
    int currMoveCol;
    int winState;
    int passCounter = 0;
    int gameMode;

    char player1;
    char player2;

    cout << "Welcome to Riversie!" << endl;
    cout << "Choose game mode: " << endl;
    cout << "       1. Player vs Pure MCTS" << endl;
    cout << "       2. Player vs Heuristic MCTS" << endl;
    cout << "       3. Pure MCTS vs Heuristic MCTS" << endl;
    cout << "(Enter 1, 2 or 3 to pick your choice): ";      // User input for game mode
    cin >> gameMode;

    if(gameMode == 1 || gameMode == 2) {
        cout << "Choose your stone! (X or O): ";    // User input for character
        cin >> player;
    }
    else {
        cout << "Choose stone for Pure MCTS! (X or O): ";   // User input for character
        cin >> pure_mcts;
    }

//    if(player != 'X' || player != 'O') {
//        while(player != 'X' || player != 'O') {
//            cout << "Please enter a valid character (X or O): ";
//            cin >> player;
//        }
//    }

    // Assigning characters to the other players
    if(gameMode == 1) {
        if (player == 'X')
            pure_mcts = 'O';
        else
            pure_mcts = 'X';
        player1 = player;
        player2 = pure_mcts;
    }
    else if(gameMode == 2) {
        if (player == 'X')
            heur_mcts = 'O';
        else
            heur_mcts = 'X';
        player1 = player;
        player2 = heur_mcts;
    }
    else {
        if (pure_mcts == 'X')
            heur_mcts = 'O';
        else
            heur_mcts = 'X';
        player1 = pure_mcts;
        player2 = heur_mcts;
    }

    // Placing initial 4 stones on the board
    stonesOnBoard = 4;
    board[SIZE/2 - 1][SIZE/2 - 1] = board[SIZE/2][SIZE/2] = 'O';
    board[SIZE/2 - 1][SIZE/2] = board[SIZE/2][SIZE/2 - 1] = 'X';

    while(true) {
        display(board);

        // Players move
        if(currentPlayer == player) {
            if(validMoves(board, moves, currentPlayer) > 0) {
                if(passCounter >= 1)
                    passCounter = 0;

                cout << endl;
                bool validMove = false;
                while (!validMove) {
                    cout << "Please Enter your move (row column): ";
                    cin >> currMoveRow >> currMoveColChar;

                    currMoveCol = (int) currMoveColChar - 97;
                    currMoveRow--;

                    // Placing stone if it is a valid move
                    if (currMoveRow >= 0 && currMoveCol >= 0 && currMoveRow < SIZE && currMoveCol < SIZE &&
                        moves[currMoveRow][currMoveCol] == 1) {
                        moveStone(board, currMoveRow, currMoveCol, currentPlayer);
                        stonesOnBoard++;
                        validMove = true;
                    }
                }
            }
            else {
                passCounter += 1;   // Incrementing passCounter if a move cant be made
            }
        }

        // Pure MCTS's move
        else if(currentPlayer == pure_mcts) {
            if(validMoves(board, moves, currentPlayer) > 0) {
                if(passCounter >= 1)     // Resetting the passCounter
                    passCounter = 0;
                cout << endl << "Pure MCTS's turn!" << endl;
                // Calling the MCTS function
                pure_mcts_reversi(board, moves, pure_mcts, stonesOnBoard);
                stonesOnBoard++;
            }
            else {
                passCounter += 1;   // Incrementing passCounter if a move cant be made
            }
        }

        // Heuristic MCTS's move
        else if(currentPlayer == heur_mcts) {
            if(validMoves(board, moves, currentPlayer) > 0) {
                if(passCounter >= 1)    // Resetting the passCounter
                    passCounter = 0;
                cout << endl << "Heuristic MCTS's turn!" << endl;
                // Calling the MCTS function
                heuristic_mcts_reversi(board, moves, heur_mcts, stonesOnBoard);
                stonesOnBoard++;
            }
            else {
                passCounter += 1;   // Incrementing passCounter if a move cant be made
            }
        }

        // Checking winState based on selected game mode
        if(gameMode == 1)
            winState = checkWinState(board, stonesOnBoard, player, pure_mcts);

        else if(gameMode == 2)
            winState = checkWinState(board, stonesOnBoard, player, heur_mcts);

        else
            winState = checkWinState(board, stonesOnBoard, pure_mcts, heur_mcts);

        // If board is full or a player's stones no longer exist on the board
        if(winState != -1) {
            int player1Points = 0;
            int player2Points = 0;

            for(int x = 0; x < SIZE; x++) {
                for(int y = 0; y < SIZE; y++) {
                    if(board[x][y] == player1)
                        player1Points++;
                    else if(board[x][y] == player2)
                        player2Points++;
                }
            }

            display(board);
            if(gameMode == 1 || gameMode == 2) {
                cout << endl;
                if (winState == 0) {
                    cout << "It is a draw." << endl;
                } else if (winState == 1) {
                    cout << "You scored: " << player1Points << " points" << endl;
                    cout << "Computer scored: " << player2Points << " points" << endl;
                    cout << "You win. Congratulations!" << endl;
                } else if (winState == 2) {
                    cout << "You scored: " << player1Points << " points" << endl;
                    cout << "Computer scored: " << player2Points << " points" << endl;
                    cout << "Computer wins. Better luck next time!" << endl;
                }
                return 0;
            }
            else {
                cout << endl;
                if (winState == 0) {
                    cout << "It is a draw." << endl;
                } else if (winState == 1) {
                    cout << "Pure MCTS scored: " << player1Points << " points" << endl;
                    cout << "Heuristics MCTS scored: " << player2Points << " points" << endl;
                    cout << "Pure MCTS wins!" << endl;
                } else if (winState == 2) {
                    cout << "Pure MCTS scored: " << player1Points << " points" << endl;
                    cout << "Heuristics MCTS scored: " << player2Points << " points" << endl;
                    cout << "Heuristics MCTS wins!" << endl;
                }
                return 0;
            }
        }

        // If no more moves can be made by either players
        if(passCounter >= 2) {
            int player1Points = 0;
            int player2Points = 0;

            for(int x = 0; x < SIZE; x++) {
                for(int y = 0; y < SIZE; y++) {
                    if(board[x][y] == player1)
                        player1Points++;
                    else if(board[x][y] == player2)
                        player2Points++;
                }
            }

            if(gameMode == 1 || gameMode == 2) {
                if (player1Points > player2Points) {
                    cout << "You scored: " << player1Points << " points" << endl;
                    cout << "Computer scored: " << player2Points << " points" << endl;
                    cout << "You win. Congratulations!" << endl;
                } else if (player1Points < player2Points) {
                    cout << "You scored: " << player1Points << " points" << endl;
                    cout << "Computer scored: " << player2Points << " points" << endl;
                    cout << "Computer wins. Better luck next time!" << endl;
                } else {
                    cout << "You scored: " << player1Points << " points" << endl;
                    cout << "Computer scored: " << player2Points << " points" << endl;
                    cout << "It is a draw." << endl;
                }
            }
            else {
                if (player1Points > player2Points) {
                    cout << "Pure MCTS scored: " << player1Points << " points" << endl;
                    cout << "Heuristics MCTS scored: " << player2Points << " points" << endl;
                    cout << "Pure MCTS wins!" << endl;
                } else if (player1Points < player2Points) {
                    cout << "Pure MCTS scored: " << player1Points << " points" << endl;
                    cout << "Heuristics MCTS scored: " << player2Points << " points" << endl;
                    cout << "Heuristics MCTS wins!" << endl;
                } else {
                    cout << "Pure MCTS scored: " << player1Points << " points" << endl;
                    cout << "Heuristics MCTS scored: " << player2Points << " points" << endl;
                    cout << "It is a draw." << endl;
                }
            }
        }

        // Changing players
        if(currentPlayer == player1) {
            currentPlayer = player2;
        } else {
            currentPlayer = player1;
        }
    }
}




/** Function to display the board **/
void display(char board[][SIZE]) {
    int row;
    int col;
    char col_label = 'a';

    cout << "\n ";
    for(col = 0 ; col<SIZE ;col++)
        printf("   %c", col_label+col);
    cout << "\n";

    // Display the intermediate rows
    for(row = 0; row < SIZE; row++)
    {
        cout << "  +";
        for(col = 0; col<SIZE; col++)
            cout << "---+";
        printf("\n%2d|",row + 1);

        for(col = 0; col<SIZE; col++)
            printf(" %c |", board[row][col]);
        cout << "\n";
    }

    cout << "  +";
    for(col = 0 ; col<SIZE ;col++)
        cout << "---+";
    cout << "\n";
}




/** Function to list all valid moves **/
int validMoves(char board[][SIZE], int moves[][SIZE], char player) {
    int rowInc;     // Row increment around a square
    int colInc;     // Column increment around a square
    int row;
    int col;
    int rowTemp;            // Row index when searching
    int colTemp;            // Column index when searching
    int numOfMoves = 0;  // Number of valid moves

    // Set the opponent
    char opponent;
    if(player == 'O') {
        opponent = 'X';
    } else {
        opponent = 'O';
    }

    for(row = 0; row < SIZE; row++) {     // Initialize moves array to zero
        for (col = 0; col < SIZE; col++) {
            moves[row][col] = 0;
        }
    }

    /* Finding valid cells.
       A valid move must be on a blank square and must enclose
       at least one opponent square between two player squares */
    for(row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {

            if (board[row][col] != 0) {  // Checking if it is a blank cell
                continue;
            }
            else {
                for (rowInc = -1; rowInc <= 1; rowInc++) {   // Checking the cells around the current cell
                    for (colInc = -1; colInc <= 1; colInc++) {
                        // Checking to not to go out of bounds or check the current cell
                        if (row + rowInc < 0 || row + rowInc >= SIZE || col + colInc < 0 || col + colInc >= SIZE ||
                            (rowInc == 0 && colInc == 0))
                            continue;

                        // If the neighbouring cell is an opponent cell then move in the direction of the opponent cell
                        if (board[row + rowInc][col + colInc] == opponent) {
                            rowTemp = row + rowInc;
                            colTemp = col + colInc;

                            // Looking for another player's cell in the direction of the opponent's cell
                            for (;;) {
                                rowTemp += rowInc;
                                colTemp += colInc;

                                // Checking for out of bounds
                                if (rowTemp < 0 || rowTemp >= SIZE || colTemp < 0 || colTemp >= SIZE)
                                    break;

                                // break if cell is empty
                                if (board[rowTemp][colTemp] == 0)
                                    break;

                                else if (board[rowTemp][colTemp] == opponent)
                                    continue;

                                    // If cell contains player's stone, make move valid in moves array
                                    // then move to next cell
                                else if (board[rowTemp][colTemp] == player) {
                                    moves[row][col] = 1;
                                    numOfMoves++;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return numOfMoves;
}




/** Function to place stone on the passed index **/
void moveStone(char board[][SIZE], int row, int col, char player) {
    int rowInc;
    int colInc;
    int rowTemp;
    int colTemp;

    // Set the opponent
    char opponent;
    if(player == 'O') {
        opponent = 'X';
    } else {
        opponent = 'O';
    }

    board[row][col] = player;

    for(rowInc = -1; rowInc <= 1; rowInc++) {
        for(colInc = -1; colInc <= 1; colInc++) {

            // Checking to not to go out of bounds or check the current cell
            if(row + rowInc < 0 || row + rowInc >= SIZE || col + colInc < 0 || col + colInc >= SIZE || (rowInc == 0 && colInc == 0))
                continue;

            if(board[row + rowInc][col + colInc] == opponent) {
                rowTemp = row + rowInc;
                colTemp = col + colInc;

                for(;;) {
                    rowTemp += rowInc;
                    colTemp += colInc;

                    if(rowTemp < 0 || rowTemp >= SIZE || colTemp < 0 || colTemp >= SIZE)
                        break;

                    if(board[rowTemp][colTemp] == ' ')
                        break;

                    if(board[rowTemp][colTemp] == player) {
                        while(board[rowTemp - rowInc][colTemp - colInc] == opponent) {
                            rowTemp -= rowInc;
                            colTemp -= colInc;
                            board[rowTemp][colTemp] = player;
                        }
                        break;
                    }
                }
            }
        }
    }
}




/** Function to make initial playout move by the MCTS functions **/
int* make_mcts_move(char board[][SIZE], int moves[][SIZE], char player) {
    int* bestMoveIndex = new int[2];
    for(int x = 0; x < SIZE; x++) {
        for(int y = 0; y < SIZE; y++) {
            if(moves[x][y] == 1) {
                moveStone(board, x, y, player);
                moves[x][y] = 0;
                bestMoveIndex[0] = x;
                bestMoveIndex[1] = y;
                return bestMoveIndex;
            }
        }
    }
    return nullptr;
}




/** Function to check win states of the game at an instance **/
int checkWinState(char board[][SIZE], int stonesOnBoard, char player, char computer) {
    int playerPoints = 0;
    int computerPoints = 0;

    // Counting each player's points
    for(int x = 0; x < SIZE; x++) {
        for(int y = 0; y < SIZE; y++) {
            if(board[x][y] == player)
                playerPoints++;
            else if(board[x][y] == computer)
                computerPoints++;
        }
    }

    // If board is full
    if(stonesOnBoard == SIZE*SIZE) {
        if(playerPoints > computerPoints)
            return 1;
        else if(computerPoints > playerPoints)
            return 2;
        else
            return 0;
    }

    // If a player's stones no longer exist on the board
    if(playerPoints == 0)
        return 2;
    else if(computerPoints == 0)
        return 1;
    else
        return -1;
}




/** Function for pure MCTS algorithm **/
void pure_mcts_reversi(char board[][SIZE], int moves[][SIZE], char computer, int stonesOnBoard) {
    char sample_board[SIZE][SIZE] = {0};          /* Local copy of board     */
    int sample_moves[SIZE][SIZE] = {0};           /* Local valid moves array */
    char temp_board[SIZE][SIZE] = {0};          /* Local copy of board     */
    int random_playout_moves[SIZE][SIZE] = {0};           /* Local valid moves array */
    int move_scores[SIZE][SIZE] = {0};
    int bestRow;
    int bestCol;
    char currentPlayer = computer;
    char opponent;
    int sampleStonesOnBoard = stonesOnBoard;
    int winState;
    int availableMoves = 0;
    int passCounter = 0;

    // Listing all valid moves for the AI
    validMoves(board, moves, computer);

    for(int a = 0; a < SIZE; a++) {
        for(int b = 0; b < SIZE; b++) {
            if (moves[a][b] == 1)
                availableMoves++;
        }
    }

    // Setting opponent's character
    if(computer == 'X')
        opponent = 'O';
    else
        opponent = 'X';

    // Making random playouts for each available move for the AI
    if(availableMoves > 0) {
        for(int x = 0; x < SIZE; x++) {
            for(int y = 0; y < SIZE; y++) {
                sample_moves[x][y] = moves[x][y];
            }
        }

        for(int i = 0; i < availableMoves; i++) {
            int res[3] = {0, 0, 0};

            // Copying game board
            for(int x = 0; x < SIZE; x++) {
                for(int y = 0; y < SIZE; y++) {
                    sample_board[x][y] = board[x][y];
                }
            }

            // Setting initial move by AI before the random playouts for that move
            int* randomMoveIndex = make_mcts_move(sample_board, sample_moves, currentPlayer);
            bestRow = randomMoveIndex[0];
            bestCol = randomMoveIndex[1];

            sampleStonesOnBoard++;
            winState = checkWinState(sample_board, sampleStonesOnBoard, opponent, computer);

            // Checking winStates
            if(winState != -1) {
                res[winState]++;
            } else {
                for (int t = 0; t < MAX_RAND_PLAYOUT; t++) {    // Playing each random playout
                    for (int x = 0; x < SIZE; x++) {
                        for (int y = 0; y < SIZE; y++) {
                            temp_board[x][y] = sample_board[x][y];
                        }
                    }

                    bool gameOver = false;
                    int tempStonesOnBoard = sampleStonesOnBoard;
                    while (!gameOver) {     // Looping until game ends
                        if (currentPlayer == computer)      // Changing current character
                            currentPlayer = opponent;
                        else
                            currentPlayer = computer;

                        int random_valid_moves = 0;
                        validMoves(temp_board, random_playout_moves, currentPlayer);    // Listing all valid moves

                        for(int a = 0; a < SIZE; a++) {
                            for(int b = 0; b < SIZE; b++) {
                                if (random_playout_moves[a][b] == 1)
                                    random_valid_moves++;
                            }
                        }

                        if (random_valid_moves > 0) {   // If valid moves are available
                            if(passCounter >= 1)
                                passCounter = 0;
                            int randomNumber = (rand() % random_valid_moves) + 1;   // Generating random numbers
                            int randMoveRow;
                            int randMoveCol;
                            int c = 0;
                            while (c < SIZE && randomNumber > 0) {  // Making a random move based on the random number that has been generated
                                for (int d = 0; d < SIZE; d++) {
                                    if (random_playout_moves[c][d] == 1) {
                                        randomNumber--;
                                    }
                                    if (randomNumber == 0) {
                                        randMoveRow = c;
                                        randMoveCol = d;
                                        break;
                                    }
                                }
                                c++;
                            }
                            moveStone(temp_board, randMoveRow, randMoveCol, currentPlayer);
                            tempStonesOnBoard++;

                            winState = checkWinState(temp_board, tempStonesOnBoard, opponent, computer);

                            if (winState != -1) {
                                res[winState]++;
                                gameOver = true;
                            }
                        } else {    // If no valid moves are available
                            passCounter += 1;
                            if(passCounter >= 2) {  // End playout if neither players can make a move
                                gameOver = true;
                                int playerPoints = 0;
                                int computerPoints = 0;

                                for(int x = 0; x < SIZE; x++) {     // Counting points to decide who won the playout
                                    for(int y = 0; y < SIZE; y++) {
                                        if(board[x][y] == opponent)
                                            playerPoints++;
                                        else if(board[x][y] == computer)
                                            computerPoints++;
                                    }
                                }

                                if(playerPoints > computerPoints)
                                    res[1]++;
                                else if(playerPoints < computerPoints)
                                    res[2]++;
                                else
                                    res[0]++;
                            }
                            continue;
                        }
                    }
                }
            }
            move_scores[bestRow][bestCol] += 4*res[2];      // Adding playout points to choose optimal move
            move_scores[bestRow][bestCol] += 2*res[0];
            move_scores[bestRow][bestCol] += res[1];
        }
    }
    int maxWin = 0;
    int maxWinRow;
    int maxWinCol;
    for(int s = 0; s < SIZE; s++) {
        for(int u = 0; u < SIZE; u++) {
            if (move_scores[s][u] >= maxWin) {  // Choosing move that gave maximum points in playouts
                maxWin = move_scores[s][u];
                maxWinRow = s;
                maxWinCol = u;
            }
        }
    }

    moveStone(board, maxWinRow, maxWinCol, computer);   // Making the move by the AI on the optimal position
}




/** Function for heuristic MCTS algorithm **/
void heuristic_mcts_reversi(char board[][SIZE], int moves[][SIZE], char computer, int stonesOnBoard) {
    char sample_board[SIZE][SIZE] = {0};          /* Local copy of board     */
    int sample_moves[SIZE][SIZE] = {0};           /* Local valid moves array */
    char temp_board[SIZE][SIZE] = {0};          /* Local copy of board     */
    int random_playout_moves[SIZE][SIZE] = {0};           /* Local valid moves array */
    int move_scores[SIZE][SIZE] = {0};
    int bestRow;
    int bestCol;
    char currentPlayer = computer;
    char opponent;
    int sampleStonesOnBoard = stonesOnBoard;
    int winState;
    int availableMoves = 0;
    int passCounter = 0;

    // Listing all valid moves for the AI
    validMoves(board, moves, computer);

    for(int a = 0; a < SIZE; a++)
        for(int b = 0; b < SIZE; b++) {
            if(moves[a][b] == 1)
                availableMoves++;
        }

    // Setting opponent's character
    if(computer == 'X')
        opponent = 'O';
    else
        opponent = 'X';

    // Making random playouts for each available move for the AI
    if(availableMoves > 0) {
        for(int x = 0; x < SIZE; x++) {
            for(int y = 0; y < SIZE; y++) {
                sample_moves[x][y] = moves[x][y];
            }
        }

        for(int i = 0; i < availableMoves; i++) {
            int res[3] = {0, 0, 0};

            for(int x = 0; x < SIZE; x++) {
                for(int y = 0; y < SIZE; y++) {
                    sample_board[x][y] = board[x][y];
                }
            }

            // Heuristics algorithm to move to the corner piece is any are available
            if(sample_moves[0][0] == 1) {
                moveStone(sample_board, 0, 0, computer);
                sample_moves[0][0] = 0;
                bestRow = 0;
                bestCol = 0;
            } else if(sample_moves[0][SIZE-1] == 1) {
                moveStone(sample_board, 0, SIZE-1, computer);
                sample_moves[0][SIZE-1] = 0;
                bestRow = 0;
                bestCol = SIZE-1;
            } else if(sample_moves[SIZE-1][0] == 1) {
                moveStone(sample_board, SIZE-1, 0, computer);
                sample_moves[SIZE-1][0] = 0;
                bestRow = SIZE-1;
                bestCol = 0;
            } else if(sample_moves[SIZE-1][SIZE-1] == 1) {
                moveStone(sample_board, SIZE-1, SIZE-1, computer);
                sample_moves[SIZE-1][SIZE-1] = 0;
                bestRow = SIZE-1;
                bestCol = SIZE-1;
            } else {
                // Setting initial move by AI before the random playouts for that move if no corner moves area available
                int* randomMoveIndex = make_mcts_move(sample_board, sample_moves, currentPlayer);
                bestRow = randomMoveIndex[0];
                bestCol = randomMoveIndex[1];
            }

            sampleStonesOnBoard++;
            winState = checkWinState(sample_board, sampleStonesOnBoard, opponent, computer);

            // Checking winStates
            if(winState != -1) {
                res[winState]++;
            } else {
                for (int t = 0; t < MAX_RAND_PLAYOUT; t++) {    // Playing each random playout
                    for (int x = 0; x < SIZE; x++) {
                        for (int y = 0; y < SIZE; y++) {
                            temp_board[x][y] = sample_board[x][y];
                        }
                    }

                    bool gameOver = false;
                    int tempStonesOnBoard = sampleStonesOnBoard;
                    while (!gameOver) {     // Looping until game ends
                        if (currentPlayer == computer)      // Changing current character
                            currentPlayer = opponent;
                        else
                            currentPlayer = computer;

                        int random_valid_moves = 0;
                        validMoves(temp_board, random_playout_moves, currentPlayer);    // Listing all valid moves

                        for(int a = 0; a < SIZE; a++) {
                            for(int b = 0; b < SIZE; b++) {
                                if (random_playout_moves[a][b] == 1)
                                    random_valid_moves++;
                            }
                        }

                        if (random_valid_moves > 0) {   // If valid moves are available
                            if(passCounter >= 1)
                                passCounter = 0;
                            int randomNumber = (rand() % random_valid_moves) + 1;   // Generating random numbers
                            int randMoveRow;
                            int randMoveCol;
                            int c = 0;
                            while (c < SIZE && randomNumber > 0) {  // Making a random move based on the random number that has been generated
                                for (int d = 0; d < SIZE; d++) {
                                    if (random_playout_moves[c][d] == 1) {
                                        randomNumber--;
                                    }
                                    if (randomNumber == 0) {
                                        randMoveRow = c;
                                        randMoveCol = d;
                                        break;
                                    }
                                }
                                c++;
                            }
                            moveStone(temp_board, randMoveRow, randMoveCol, currentPlayer);
                            tempStonesOnBoard++;

                            winState = checkWinState(temp_board, tempStonesOnBoard, opponent, computer);

                            if (winState != -1) {
                                res[winState]++;
                                gameOver = true;
                            }
                        } else {    // If no valid moves are available
                            passCounter += 1;
                            if(passCounter >= 2) {  // End playout if neither players can make a move
                                gameOver = true;
                                int playerPoints = 0;
                                int computerPoints = 0;

                                for(int x = 0; x < SIZE; x++) {     // Counting points to decide who won the playout
                                    for(int y = 0; y < SIZE; y++) {
                                        if(board[x][y] == opponent)
                                            playerPoints++;
                                        else if(board[x][y] == computer)
                                            computerPoints++;
                                    }
                                }

                                if(playerPoints > computerPoints)
                                    res[1]++;
                                else if(playerPoints < computerPoints)
                                    res[2]++;
                                else
                                    res[0]++;
                            }
                            continue;
                        }
                    }
                }
            }
            move_scores[bestRow][bestCol] += 4*res[2];      // Adding playout points to choose optimal move
            move_scores[bestRow][bestCol] += 2*res[0];
            move_scores[bestRow][bestCol] += res[1];
        }
    }
    int maxWin = 0;
    int maxWinRow;
    int maxWinCol;
    for(int s = 0; s < SIZE; s++) {
        for(int u = 0; u < SIZE; u++) {
            if (move_scores[s][u] >= maxWin) {  // Choosing move that gave maximum points in playouts
                maxWin = move_scores[s][u];
                maxWinRow = s;
                maxWinCol = u;
            }
        }
    }

    moveStone(board, maxWinRow, maxWinCol, computer);   // Making the move by the AI on the optimal position
}