# Hidato Solver

A graphical puzzle solver for Hidato puzzles built with C++ and FLTK.

## What is Hidato?

Hidato is a logic puzzle where you fill a grid with consecutive numbers that connect horizontally, vertically, or diagonally. Some numbers are given as clues, and you must find a path from 1 to the highest number using all cells.

**Example:**
```
Given:          Solution:
[1][ ][ ]      [1][3][4]
[ ][ ][5]  →   [2][6][5]
[9][ ][ ]      [9][8][7]
```

## Features

- **Interactive Grid Editor**: Click cells to select and type numbers
- **Visual Feedback**: 
  - Fixed numbers (clues) shown in bold with blue background
  - Selected cell highlighted in yellow
  - Blocked cells shown in dark gray
- **Backtracking Solver**: Automatically finds solutions using depth-first search
- **Flexible Grid Size**: Supports grids from 1×1 up to 20×20
- **Keyboard Navigation**: Use arrow keys to move between cells
- **Block/Unblock Cells**: Right-click to mark cells as blocked (for irregular puzzles)

## Requirements

- FLTK 1.3.x or 1.4.x
- C++ compiler

Compile the Program with the make file

## Usage

Run the program with the desired grid dimensions:

```bash
./hidato_solver <rows> <cols>
```

**Examples:**
```bash
./hidato_solver 5 5    # 5×5 grid
./hidato_solver 8 8    # 8×8 grid
./hidato_solver 10 6   # 10×6 grid
```

### Controls

#### Mouse Controls
- **Left Click**: Select a cell
- **Right Click**: Toggle cell between normal and blocked (dark gray)

#### Keyboard Controls
- **0-9**: Type numbers into the selected cell
- **Enter**: Confirm the number entry
- **Backspace**: 
  - If typing: delete last digit
  - If not typing: clear the cell
- **Arrow Keys**: Navigate between cells

#### Buttons
- **Solve**: Find a solution for the puzzle
- **Clear**: Reset the entire grid

## How to Solve a Puzzle

1. **Launch the program** with your desired grid size
2. **Place the starting number**: Click a cell and type `1`, then press Enter
3. **Add clue numbers**: Click other cells and enter any given numbers
4. **Block cells** (optional): Right-click cells that should not be used
5. **Click "Solve"**: The program will fill in the remaining numbers
6. If no solution exists, you'll see an alert message

## Algorithm

The solver uses a **backtracking algorithm** with the following approach:

1. **Start from 1**: The algorithm begins at the cell containing `1`
2. **Depth-First Search**: For each number, it tries all 8 adjacent cells.
3. **Constraintss**: 
   - Respects fixed numbers
   - Only places numbers in empty cells
   - Ensures consecutive numbers are adjacent
4. **Backtracking**: If a path leads to a dead end, it backtracks and tries another route
5. **Solution Found**: When all cells are filled with consecutive numbers

## Limitations

- Maximum grid size: 20×20 (400 cells)
- No save/load functionality
- Large grids may take significant time to solve
- No honeycomb grid support

## Planned improvements:
~~- [ ] Solver optimizations for larger grids~~

~~- [ ] Honeycomb grid support~~

~~- [ ] Timer and statistics~~

I ain't adding anything more to this, c++ make my mind go boom.


Created as an educational project to demonstrate:
- GUI programming with FLTK
- Backtracking algorithms
- C++ features (structured bindings, std::optional, std::array)
