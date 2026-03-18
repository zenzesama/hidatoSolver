#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>

// possible directions, rows, columns, fixed points map and max number in grid declarations
int dr[8] = {-1,-1,-1,0,0,1,1,1};
int dc[8] = {-1,0,1,-1,1,-1,0,1};
int n, m;
std::unordered_map<int, std::pair<int,int>> fixed;
int maxNum = 0;

// helper function to determine if pointers are in grid
bool inside(int r, int c){
    return r >= 0 && r < n && c >= 0 && c < m;
}

// in the backtrack function we pass the grid, current row & column, and the last number placed
// k = current number placed at (r, c); try to place k+1 next
bool solve(std::vector<std::vector<int>> &grid, int r, int c, int k){

    // base case, all numbers filled
    if (k == maxNum) return true;

    int target = k + 1;

    // 1. Check if target is a pre-placed number
    int targetR = -1, targetC = -1;
    auto it = fixed.find(target);
    if(it != fixed.end()){
        targetR = it->second.first;
        targetC = it->second.second;
    }

    // 2. If target is pre placed, it MUST be adjacent to (r, c), if not, Prune this branch
    if(targetR != -1 && targetC != -1){
        if(std::abs(r - targetR) <= 1 && std::abs(c - targetC) <= 1){
            return solve(grid, targetR, targetC, target);
        }
        return false;
    }

    // 3. Otherwise try placing target in all adjacent empty cells
    for(int d = 0; d < 8; d++){
        int nr = r + dr[d];
        int nc = c + dc[d];

        if(inside(nr, nc) && grid[nr][nc] == 0){
            grid[nr][nc] = target;
            if (solve(grid, nr, nc, target)) return true;
            grid[nr][nc] = 0;
        }
    }

    return false;
}

int main(){
    std::vector<std::vector<int>> grid = {
        {0, 0, 6},
        {0, -1, 0},
        {1, 0, 0}
    };

    n = grid.size();
    m = grid[0].size();

    std::pair<int,int> start = {-1, -1};

    // do these:
    // 1. figure out the max number on the board
    // 2. figure out the start indexes
    // 3. map all the pre placed numbers to their indexes
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(grid[i][j] != -1) maxNum++;
            if(grid[i][j] == 1) start = {i, j};
            if(grid[i][j] > 0) fixed[grid[i][j]] = {i, j};
        }
    }

    // invalid state. No starting cell.
    if(start.first == -1){
        std::cout << "No starting cell (1) found\n";
        return 1;
    }

    // backtracking function
    if(solve(grid, start.first, start.second, 1)){
        std::cout << "Solved Hidato:\n";
        for(auto &row : grid){
            for(auto x : row){
                std::cout << x << " ";
            }
            std::cout << "\n";
        }
    }
    else{
        std::cout << "No solution\n";
    }

    return 0;
}