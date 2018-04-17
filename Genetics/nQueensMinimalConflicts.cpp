
#include <iostream>					// for cout etc.
#include <vector>
#include <string>
#include <array>					// for vector class
#include <string>					// for string class
#include <algorithm>				// for sort algorithm
#include <time.h>					// for random seed
#include <math.h>
#include <sstream>
#define N 500

using namespace std;


struct mc_struct
{
    int board[N];
};


void init(mc_struct& queens) {
    for (int i = 0, n = N; i < n; i++) {
        queens.board[i] = i;
    }
    for (int i = 0, n = N; i < n; i++) {
        int j = rand()%n;
        int rowToSwap = queens.board[i];
        queens.board[i] = queens.board[j];
        queens.board[j] = rowToSwap;
    }
}


int get_conflicts(mc_struct& queens,int row, int col) {
    int _count = 0;
    for (int c = 0; c < N; c++) {
        if (c == col) continue;
        int r = queens.board[c];
        if (r == row || abs(r-row) == abs(c-col)) _count++;
    }
    return _count;
}


void solve(mc_struct& queens) {
    int moves = 0;

    vector<int> candidates;

    while (true) {
        int maxConflicts = 0;
        candidates.clear();
        for (int c = 0; c < N; c++) {
            int conflicts = get_conflicts(queens,queens.board[c], c);
            if (conflicts == maxConflicts) {
                candidates.push_back(c);
            } else if (conflicts > maxConflicts) {
                maxConflicts = conflicts;
                candidates.clear();
                candidates.push_back(c);
            }
        }

        if (maxConflicts == 0) {
            return;
        }

        // Pick a random queen from those that had the most conflicts
        int worstQueenColumn =
                candidates[rand()%(candidates.size())];

        // Move her to the place with the least conflicts.
        int minConflicts = N;
        candidates.clear();
        for (int r = 0; r < N; r++) {
            int conflicts = get_conflicts(queens,r, worstQueenColumn);
            if (conflicts == minConflicts) {
                candidates.push_back(r);
            } else if (conflicts < minConflicts) {
                minConflicts = conflicts;
                candidates.clear();
                candidates.push_back(r);
            }
        }

        if (!candidates.empty()) {
            queens.board[worstQueenColumn] =
                candidates[rand()%(candidates.size())];
        }

        moves++;
        if (moves == N * 2) {
            init(queens);
            moves = 0;
        }
    }
}

void print(int rows[N]) {
    string result="";
    for (int r = 0; r < N; r++) {
            if(r%30 == 0)
                result += "\n";
        for (int c = 0; c < N; c++) {

            if(rows[c] == r){
                std::ostringstream ss;
                ss << c;
                result += (ss.str() + " ");
            }
           // result = (rows[c] == r ? (result + (""+(c+'0'))) : result);
        }
    }
    cout << "Done: \n" << result << endl;
}


/
int main() {
    mc_struct board;
    init(board);

    solve(board);
    print(board.board);

}

