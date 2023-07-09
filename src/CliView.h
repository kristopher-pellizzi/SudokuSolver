#ifndef CLIVIEW
#define CLIVIEW

#include "Grid.h"
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


class CliView {
    private:
        Grid& grid;
        unsigned grid_width;
        unsigned block_width;

        void draw_top_line() const;
        void draw_bottom_line() const;
        void draw_cell_line(unsigned line_idx) const;
        void draw_cell_separator(unsigned line_idx) const;

    public:
        CliView(Grid& grid);
        
        void draw() const;
};

#endif //CLIVIEW