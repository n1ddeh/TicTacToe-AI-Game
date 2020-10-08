#include <tictac_support.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <limits>
#include <utility>

bool isFull(const int board[3][3]);
int isWin(const int board[3][3]);
int getTurn(const int board[3][3]);
int min_max(int board[3][3], int depth, int maximizer);
int getOpenSpaces(const int board[3][3]);

int make_move(int board[3][3])
{
    if (isFull(board) || isWin(board)) return 0;
    std::vector<std::pair<int, int>> pos;
    int score = 0;
    int max_row = 0;
    int max_col = 0;
    int state = getTurn(board);
    int max_score = ((state == 1) ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max());

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) // If a play is available, call min-max on that play
            {
                board[i][j] = state;
                score = min_max(board, 0,  (state*-1));
                board[i][j] = 0;
                if ((state == 1 && score > max_score) || (state == -1 && score < max_score)) {
                    pos.clear();
                    max_score = score;
                }
                if (score == max_score) pos.push_back(std::make_pair(i, j));
            }
        }
    }
    srand(time(NULL));
    int max_pos = rand() % pos.size();
    max_row = std::get<0>(pos[max_pos]);
    max_col = std::get<1>(pos[max_pos]);
    int steps = 0;
    if (max_score < 0){
        steps = getOpenSpaces(board) - (max_score * -1);
    }
    else {
        steps = getOpenSpaces(board) - max_score;
    }
    board[max_row][max_col] = state;
    return steps;
}

int min_max(int board[3][3], int depth, int maximizer)
{
   // Check if end-game state reached.
   if (isWin(board) == 1) return 1*getOpenSpaces(board); // PLAYER 1 WIN.
   else if (isWin(board) == -1) return -1*getOpenSpaces(board); // PLAYER -1 WIN.
   if (isFull(board)) return 0; // TIE.

   if (maximizer == 1) { // Player 1 turn.
       int best_score = std::numeric_limits<int>::min(); // Initial best score set to absolute minimum int.
       int score = best_score;
       for (int i = 0; i < 3; i++) {
           for (int j = 0; j < 3; j++) {
               if (board[i][j] == 0) {
                   board[i][j] = 1;
                   score = min_max(board, depth + 1, -1);
                   best_score = std::max(best_score, score);
                   board[i][j] = 0; // Undo move.
               }
           }
       }
       return best_score;
   }
   else { // Player -1 turn.
       int best_score = std::numeric_limits<int>::max(); // Initial best score set to absolute maximum int.
       int score = best_score;
       for (int i = 0; i < 3; i++) {
           for (int j = 0; j < 3; j++) {
               if (board[i][j] == 0) {
                   board[i][j] = -1;
                   score = min_max(board, depth + 1, 1);
                   best_score = std::min(score, best_score);
                   board[i][j] = 0;
               }
           }
       }
       return best_score;
   }
}

bool isFull(const int board[3][3]) {
    for (int i = 0; i < 3; i++) { for (int j = 0; j < 3; j++) { if (board[i][j] == 0) { return false; }}}
    return true;
}

int isWin(const int board[3][3])
{
    for (int i = 0; i < 3; i++) { // Columns and Rows
        if (board[0][i] != 0 && board[0][i] == board[1][i] && board[0][i] == board[2][i]) return board[0][i];
        if (board[i][0] != 0 && board[i][0] == board[i][1] && board[i][0] == board[i][2]) return board[i][0];
    }
    // Diagonals
    if (board[0][0] != 0 && board[0][0] == board[1][1] && board[0][0] == board[2][2]) return board[0][0];
    if (board[2][0] != 0 && board[2][0] == board[1][1] && board[2][0] == board[0][2]) return board[2][0];
    return 0;
}

int getTurn(const int board[3][3]) {
    if (isFull(board)) return 0;
    int pos = 0, neg = 0;
    for (int i = 0; i < 3; i++) { for (int j = 0; j < 3; j++) {
            if (board[i][j] == 1) pos++;
            else if (board[i][j] == -1) neg++;
        }
    }
    if (pos > neg) return -1;
    return 1;
}

int getOpenSpaces(const int board[3][3]) {
    int x = 0;
    for (int i = 0; i < 3; i++) { for (int j = 0; j < 3; j++) { if (board[i][j] == 0) x++; }}
    return x;
}