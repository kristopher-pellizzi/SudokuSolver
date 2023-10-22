#ifndef CLIVIEW
#define CLIVIEW

#include "View.h"
#include "Grid.h"
#include <deque>
#include <string>

using std::string;

// Heavy Solid Lines
extern const string TL_ANGLE;
extern const string TR_ANGLE;
extern const string BL_ANGLE;
extern const string BR_ANGLE;
extern const string H_LINE;
extern const string V_LINE;
extern const string LEFT_CROSS;
extern const string RIGHT_CROSS;
extern const string TOP_CROSS;
extern const string BOTTOM_CROSS;
extern const string CROSS;

// Hybrid Heavy/Light Solid Lines (For T box lines and crosses)
extern const string HL_LEFT_CROSS;
extern const string HL_RIGHT_CROSS;
extern const string HL_TOP_CROSS;
extern const string HL_BOTTOM_CROSS;
extern const string HL_CROSS_LV; // Cross with light vertical line
extern const string HL_CROSS_LH; // Cross with light horizontal line

// Light Solid Lines
extern const string LIGHT_H_LINE;
extern const string LIGHT_V_LINE;
extern const string LIGHT_CROSS;


class CliView : public View {
    private:
        Grid& grid;
        unsigned grid_width;
        unsigned block_width;
        unsigned h_space;
        unsigned digits;

        unsigned get_num_digits(unsigned n) const;
        string get_string_repr(unsigned n) const;
        void insert_vertical_space(unsigned count = 1) const;
        void insert_horizontal_space(unsigned count = 1) const;
        void draw_col_coords() const;
        void draw_row_coord(unsigned row_idx) const;
        void draw_top_line() const;
        void draw_bottom_line() const;
        void draw_cell_line(unsigned line_idx) const;
        void draw_cell_separator(unsigned line_idx) const;
        std::deque<string> split_on_newlines(string s) const;

    public:
        CliView(Grid& grid);

        void draw() const override;
        void print_constraints() const override;
        void message(string msg) const override;
        void warning(string wrn) const override;
        void error(string err) const override;
};

#endif //CLIVIEW