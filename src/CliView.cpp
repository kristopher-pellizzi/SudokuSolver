#include "CliView.h"
#include <iostream>
#include <sstream>
#include <string>

using std::endl;
using std::cout;

const string TL_ANGLE = "\u250f";
const string TR_ANGLE = "\u2513";
const string BL_ANGLE = "\u2517";
const string BR_ANGLE = "\u251b";
const string H_LINE = "\u2501";
const string V_LINE = "\u2503";
const string LEFT_CROSS = "\u2523";
const string RIGHT_CROSS = "\u252b";
const string TOP_CROSS = "\u2533";
const string BOTTOM_CROSS = "\u253b";
const string CROSS = "\u254b";

const string HL_LEFT_CROSS = "\u2520";
const string HL_RIGHT_CROSS = "\u2528";
const string HL_TOP_CROSS = "\u252f";
const string HL_BOTTOM_CROSS = "\u2537";
const string HL_CROSS_LV = "\u253f";
const string HL_CROSS_LH = "\u2542";

const string LIGHT_H_LINE = "\u2500";
const string LIGHT_V_LINE = "\u2502";
const string LIGHT_CROSS = "\u253c";

CliView::CliView(Grid& grid) : grid(grid) {
    grid_width = grid.get_grid_width();
    block_width = grid.get_block_width();
}

void CliView::draw_top_line() const {
    std::stringstream line;

    line << TL_ANGLE;

    for (int i = 1; i < grid_width; ++i){
        line << H_LINE;

        if (i % block_width == 0)
            line << TOP_CROSS;
        else
            line << HL_TOP_CROSS;
    }

    line << H_LINE << TR_ANGLE;
    cout << line.str() << endl;
}

void CliView::draw_bottom_line() const{
    std::stringstream line;

    line << BL_ANGLE;

    for (int i = 1; i < grid_width; ++i){
        line << H_LINE;
        
        if (i % block_width == 0)
            line << BOTTOM_CROSS;
        else
            line << HL_BOTTOM_CROSS;
    }

    line << H_LINE << BR_ANGLE;
    cout << line.str() << endl;
}

void CliView::draw_cell_line(unsigned line_idx) const{
    std::stringstream line;
    line << V_LINE;

    for (int i = 1; i < grid_width; ++i){
        line << grid[line_idx][i - 1];
        
        if (i % block_width == 0)
            line << V_LINE;
        else
            line << LIGHT_V_LINE;
    }

    line << grid[line_idx][grid_width - 1] << V_LINE;
    cout << line.str() << endl;
}

void CliView::draw_cell_separator(unsigned line_idx) const{
    std::stringstream line;
    string H_LINE_CHAR;
    string LEFT_CROSS_CHAR;
    string RIGHT_CROSS_CHAR;
    string CROSS_CHAR;
    bool is_block_separator;

    if ((line_idx + 1) % block_width == 0){
        is_block_separator = true;
        H_LINE_CHAR = H_LINE;
        CROSS_CHAR = HL_CROSS_LV;
        LEFT_CROSS_CHAR = LEFT_CROSS;
        RIGHT_CROSS_CHAR = RIGHT_CROSS;
    }
    else{
        is_block_separator = false;
        H_LINE_CHAR = LIGHT_H_LINE;
        CROSS_CHAR = LIGHT_CROSS;
        LEFT_CROSS_CHAR = HL_LEFT_CROSS;
        RIGHT_CROSS_CHAR = HL_RIGHT_CROSS;
    }

    line << LEFT_CROSS_CHAR;

    for (int i = 1; i < grid_width; ++i){
        line << H_LINE_CHAR;
        
        if (i % block_width == 0) 
            line << (is_block_separator ? CROSS : HL_CROSS_LH);
        else 
            line << CROSS_CHAR;
    }

    line << H_LINE_CHAR << RIGHT_CROSS_CHAR;
    cout << line.str() << endl;
}

void CliView::draw() const{
    draw_top_line();

    for (int i = 0; i < grid_width - 1; ++i){
        draw_cell_line(i);
        draw_cell_separator(i);
    }
    
    draw_cell_line(grid_width - 1);
    draw_bottom_line();
}