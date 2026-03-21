#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/fl_draw.H>
#include <FL/fl_ask.H>

#include <algorithm>
#include <array>
#include <optional>
#include <string>

constexpr int MAX_ROWS = 20;
constexpr int MAX_COLS = 20;
constexpr int MAX_NUM = MAX_ROWS * MAX_COLS;
constexpr int CELL_SIZE = 80;

struct Vec2
{
    int r, c;
};

constexpr std::array<Vec2, 8> DIRS = {{{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};

using Grid = std::array<std::array<int, MAX_COLS>, MAX_ROWS>;

bool inside(int r, int c, int n, int m) {
    return r >= 0 && r < n && c >= 0 && c < m;
}

bool solve(Grid &grid,
           const std::array<std::optional<Vec2>, MAX_NUM + 1> &fixed,
           int r, int c, int k, int maxNum, int n, int m) {
    if (k == maxNum) {
        return true;
    }

    const int target = k + 1;

    if (fixed[target]) {
        auto [tr, tc] = *fixed[target];
        if (std::abs(r - tr) <= 1 && std::abs(c - tc) <= 1) {
            return solve(grid, fixed, tr, tc, target, maxNum, n, m);
        } else {
            return false;
        }
    }

    for (auto [dr, dc] : DIRS) {
        int nr = r + dr, nc = c + dc;
        if (inside(nr, nc, n, m) && grid[nr][nc] == 0) {
            grid[nr][nc] = target;
            if (solve(grid, fixed, nr, nc, target, maxNum, n, m)) {
                return true;
            } else {
                grid[nr][nc] = 0;
            }
        }
    }

    return false;
}

class HidatoGrid : public Fl_Widget {
public:
    Grid grid{};
    int rows, cols;
    int selected_r = -1, selected_c = -1;
    std::string input_buf;
    std::array<bool, MAX_NUM + 1> is_fixed{};

    HidatoGrid(int x, int y, int w, int h, int r, int c)
        : Fl_Widget(x, y, w, h), rows(r), cols(c) {
        grid = {};
        is_fixed = {};
    }

    void clear_grid() {
        grid = {};
        is_fixed = {};
        selected_r = selected_c = -1;
        input_buf.clear();
    }

    void commit_input() {
        if (input_buf.empty() || selected_r == -1) {
            return;
        }
        int val = std::stoi(input_buf);
        input_buf.clear();
        if (val <= 0) {
            return;
        }

        int old = grid[selected_r][selected_c];
        if (old > 0) {
            is_fixed[old] = false;
        }

        grid[selected_r][selected_c] = val;
        is_fixed[val] = true;
    }

private:
    void draw() override {
        int cw = w() / cols;
        int ch = h() / rows;

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int cx = x() + c * cw;
                int cy = y() + r * ch;
                int val = grid[r][c];

                // background
                if (val == -1) {
                    fl_color(FL_DARK3);
                } else if (r == selected_r && c == selected_c) {
                    fl_color(FL_YELLOW);
                } else if (val > 0 && is_fixed[val]) {
                    fl_color(fl_rgb_color(200, 220, 255));
                } else {
                    fl_color(FL_WHITE);
                }
                fl_rectf(cx, cy, cw, ch);

                fl_color(FL_BLACK);
                fl_rect(cx, cy, cw, ch);

                if (val > 0) {
                    fl_font(is_fixed[val] ? FL_BOLD : FL_HELVETICA, 16);
                    fl_color(FL_BLACK);
                    fl_draw(std::to_string(val).c_str(), cx, cy, cw, ch, FL_ALIGN_CENTER);
                }

                if (r == selected_r && c == selected_c && !input_buf.empty()) {
                    fl_font(FL_HELVETICA, 16);
                    fl_color(FL_RED);
                    fl_draw(input_buf.c_str(), cx, cy, cw, ch, FL_ALIGN_CENTER);
                }
            }
        }
    }

    int handle(int event) override {
        int cw = w() / cols;
        int ch = h() / rows;

        switch (event) {
            case FL_PUSH: {
                int c = (Fl::event_x() - x()) / cw;
                int r = (Fl::event_y() - y()) / ch;
                if (r < 0 || r >= rows || c < 0 || c >= cols) {
                    return 1;
                }

                commit_input();

                if (Fl::event_button() == FL_RIGHT_MOUSE) {
                    int old = grid[r][c];
                    if (old > 0) {
                        is_fixed[old] = false;
                    }
                    grid[r][c] = (grid[r][c] == -1) ? 0 : -1;
                } else {
                    selected_r = r;
                    selected_c = c;
                    Fl::focus(this);
                }
                redraw();
                return 1;
            }

            case FL_KEYDOWN: {
                if (selected_r == -1) {
                    return 0;
                }
                int key = Fl::event_key();

                if (key >= '0' && key <= '9') {
                    if (input_buf.size() < 3) {
                        input_buf += static_cast<char>(key);
                    }
                    redraw();
                    return 1;
                }

                if (key == FL_Enter || key == FL_KP_Enter) {
                    commit_input();
                    redraw();
                    return 1;
                }

                if (key == FL_BackSpace) {
                    if (!input_buf.empty()) {
                        input_buf.pop_back();
                    } else {
                        int v = grid[selected_r][selected_c];
                        if (v > 0) {
                            is_fixed[v] = false;
                        }
                        grid[selected_r][selected_c] = 0;
                    }
                    redraw();
                    return 1;
                }

                if (key == FL_Up && selected_r > 0) {
                    commit_input();
                    --selected_r;
                }

                if (key == FL_Down && selected_r < rows - 1) {
                    commit_input();
                    ++selected_r;
                }

                if (key == FL_Left && selected_c > 0) {
                    commit_input();
                    --selected_c;
                }

                if (key == FL_Right && selected_c < cols - 1) {
                    commit_input();
                    ++selected_c;
                }

                redraw();
                return 1;
            }

            case FL_FOCUS:
            case FL_UNFOCUS: {
                return 1;
            }
        }

        return Fl_Widget::handle(event);
    }
};

static HidatoGrid *g_grid = nullptr;

void solve_cb(Fl_Widget *, void *) {
    g_grid->commit_input();

    const int n = g_grid->rows;
    const int m = g_grid->cols;

    Grid grid = g_grid->grid;

    std::array<std::optional<Vec2>, MAX_NUM + 1> fixed{};
    int maxNum = 0;
    std::optional<Vec2> start;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] != -1) {
                ++maxNum;
            }
            if (grid[i][j] > 0) {
                int v = grid[i][j];
                fixed[v] = Vec2{i, j};
                if (v == 1) {
                    start = Vec2{i, j};
                }
            }
        }
    }

    if (!start) {
        fl_alert("Place a '1' on the grid first!");
        return;
    }

    if (solve(grid, fixed, start->r, start->c, 1, maxNum, n, m)) {
        g_grid->grid = grid;
        g_grid->redraw();
    } else {
        fl_alert("No solution found.");
    }
}

void clear_cb(Fl_Widget *, void *) {
    g_grid->clear_grid();
    g_grid->redraw();
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fl_alert("Usage: %s <rows> <cols>", argv[0]);
        return 1;
    }

    const int rows = std::stoi(argv[1]);
    const int cols = std::stoi(argv[2]);

    if (rows <= 0 || cols <= 0 || rows > MAX_ROWS || cols > MAX_COLS) {
        fl_alert("Rows and cols must be between 1 and %d.", MAX_ROWS);
        return 1;
    }

    if (rows * cols > MAX_NUM) {
        fl_alert("Grid too large.");
        return 1;
    }

    const int grid_w = cols * CELL_SIZE;
    const int grid_h = rows * CELL_SIZE;
    const int win_w = grid_w + 40;
    const int win_h = grid_h + 70;

    Fl_Window win(win_w, win_h, "Hidato Solver");

    HidatoGrid grid_widget(20, 20, grid_w, grid_h, rows, cols);
    g_grid = &grid_widget;

    Fl_Button btn_solve(20, grid_h + 30, 90, 30, "Solve");
    Fl_Button btn_clear(120, grid_h + 30, 90, 30, "Clear");

    btn_solve.callback(solve_cb);
    btn_clear.callback(clear_cb);

    win.end();
    win.show();

    return Fl::run();
}