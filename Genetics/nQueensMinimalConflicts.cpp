
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <string>
#include <algorithm>
#include <time.h>
#include <math.h>
#include <sstream>
#define N 16

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

/**
*   This is the minimal conflicts implementation
*    every iteration:
*       1: find worst queen (maximum conflicts)
*       2: find best queen (minimum conflicts)
*       3: swap them
*
**/
void solve(mc_struct& queens) {
    int moves = 0;
    vector<int> candidates;
    while (true) {
        int mx_conflicts = 0;
        candidates.clear();
        for (int c = 0; c < N; c++) {
            int conflicts = get_conflicts(queens,queens.board[c], c);
            if (conflicts == mx_conflicts) {
                candidates.push_back(c);
            } else if (conflicts > mx_conflicts) {
                mx_conflicts = conflicts;
                candidates.clear();
                candidates.push_back(c);
            }
        }
        if (mx_conflicts == 0) {
            return;
        }
        // get a queen from the most conflicted queens in order to swap it's position
        int worstQueenColumn = candidates[rand()%(candidates.size())];

        // find the minimal conflicted queens
        int min_conflicts = N;
        candidates.clear();
        for (int r = 0; r < N; r++) {
            int conflicts = get_conflicts(queens,r, worstQueenColumn);
            if (conflicts == min_conflicts) {
                candidates.push_back(r);
            } else if (conflicts < min_conflicts) {
                min_conflicts = conflicts;
                candidates.clear();
                candidates.push_back(r);
            }
        }
        if (!candidates.empty()) {
            //choose a random element from the minimal conflicted queens and swap it with the maximal conflicted
            queens.board[worstQueenColumn] =candidates[rand()%(candidates.size())];
        }
        moves++;
        if (moves == 2*N) {
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
        }
    }
    cout << "Done: \n" << result << endl;
}

void minimal_conflicts(mc_struct mcs){
    init(mcs);
    solve(mcs);
    print(mcs.board);
}

int main() {
    mc_struct board;
    minimal_conflicts(board);
}

