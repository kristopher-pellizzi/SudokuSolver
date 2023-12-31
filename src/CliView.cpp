#include "CliView.h"
#include <iostream>
#include <sstream>
#include <string>
#include <set>

using std::endl;
using std::cout;
using std::cerr;

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

const string ANSI_RST  = "\x1b[0m";
const string ANSI_BOLD = "\x1b[1m";
const string ANSI_BLUE = "\x1b[34m";

CliView::CliView(Grid& grid){
    grid_width = grid.get_grid_width();
    block_width = grid.get_block_width();
    digits = get_num_digits(grid_width);
    h_space = 2 + digits;
}

unsigned CliView::get_num_digits(unsigned n) const{
    unsigned count = 1;
    unsigned curr = n / 10;

    while (curr != 0){
        curr /= 10;
        ++count;
    }

    return count;
}

string CliView::get_string_repr(unsigned n) const{
    unsigned n_digits = get_num_digits(n);

    std::stringstream sstream;
    sstream << std::string(digits - n_digits, ' ');
    sstream << n;

    return sstream.str();
}

void CliView::insert_vertical_space(unsigned count) const{
    for(unsigned i = 0; i < count; ++i)
        cout << endl;
}

void CliView::insert_horizontal_space(unsigned count) const{
    cout << std::string(count, ' ');
}

void CliView::draw_col_coords() const{
    insert_horizontal_space(h_space);
    for(unsigned i = 0; i < grid_width; ++i){
        cout << " " << get_string_repr(i);
    }
}

void CliView::draw_row_coord(unsigned row_idx) const{
    cout << get_string_repr(row_idx) << "  ";
}

void CliView::draw_top_line() const {
    insert_horizontal_space(h_space);

    std::stringstream line;

    line << TL_ANGLE;

    for (unsigned i = 1; i < grid_width; ++i){
        for(unsigned j = 0; j < digits; ++j)
            line << H_LINE;

        if (i % block_width == 0)
            line << TOP_CROSS;
        else
            line << HL_TOP_CROSS;
    }

    for (unsigned i = 0; i < digits; ++i)
        line << H_LINE;

    line << TR_ANGLE;
    cout << line.str() << endl;
}

void CliView::draw_bottom_line() const{
    insert_horizontal_space(h_space);

    std::stringstream line;

    line << BL_ANGLE;

    for (unsigned i = 1; i < grid_width; ++i){
        for (unsigned j = 0; j < digits; ++j)
            line << H_LINE;
        
        if (i % block_width == 0)
            line << BOTTOM_CROSS;
        else
            line << HL_BOTTOM_CROSS;
    }

    for (unsigned i = 0; i < digits; ++i)
        line << H_LINE;
    
    line << BR_ANGLE;
    cout << line.str() << endl;
}

void CliView::draw_cell_line(unsigned line_idx) const{
    draw_row_coord(line_idx);
    std::set<Coordinates>& fixed_cells = grid->get_fixed_cells();

    std::stringstream line;
    line << V_LINE;

    for (unsigned i = 1; i < grid_width; ++i){
        unsigned cell_cont = grid->get(line_idx, i - 1);
        auto found = fixed_cells.find(Coordinates(line_idx, i - 1));
        
        if (found != fixed_cells.end())
            line << ANSI_BOLD << ANSI_BLUE;

        if (cell_cont != 0)
            line << get_string_repr(cell_cont);
        else 
            line << std::string(digits, ' ');

        line << ANSI_RST;
        
        if (i % block_width == 0)
            line << V_LINE;
        else
            line << LIGHT_V_LINE;
    }

    unsigned cell_cont = grid->get(line_idx, grid_width - 1);
    auto found = fixed_cells.find(Coordinates(line_idx, grid_width - 1));

    if (found != fixed_cells.end())
        line << ANSI_BOLD << ANSI_BLUE;

    if (cell_cont != 0)
        line << get_string_repr(cell_cont);
    else
        line << std::string(digits, ' ');

    line << ANSI_RST;

    line << V_LINE;
    cout << line.str() << endl;
}

void CliView::draw_cell_separator(unsigned line_idx) const{
    insert_horizontal_space(h_space);

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

    for (unsigned i = 1; i < grid_width; ++i){
        for(unsigned j = 0; j < digits; ++j)
            line << H_LINE_CHAR;
        
        if (i % block_width == 0) 
            line << (is_block_separator ? CROSS : HL_CROSS_LH);
        else 
            line << CROSS_CHAR;
    }

    for(unsigned i = 0; i < digits; ++i)
        line << H_LINE_CHAR;
    
    line << RIGHT_CROSS_CHAR;
    cout << line.str() << endl;
}

void CliView::draw(IGrid* grid){
    this->grid = grid;
    draw_col_coords();
    insert_vertical_space();
    draw_top_line();

    for (unsigned i = 0; i < grid_width - 1; ++i){
        draw_cell_line(i);
        draw_cell_separator(i);
    }
    
    draw_cell_line(grid_width - 1);
    draw_bottom_line();
}

void CliView::print_constraints(IGrid* grid) {
    this->grid = grid;
    unsigned idx_limit = grid_width * grid_width;

    for(unsigned i = 0; i < idx_limit; ++i){
        unsigned row_idx = i / grid_width;
        unsigned col_idx = i % grid_width;
        std::set<unsigned>& available_vals = grid->get_available_vals(row_idx, col_idx);
        cout << row_idx << "; " << col_idx << ": " << endl;
        cout << "SIZE: " << available_vals.size() << endl;
        for(auto iter = available_vals.begin(); iter != available_vals.end(); ++iter){
            cout << "\t" << *iter << endl;
        }
    }
}

std::deque<string> CliView::split_on_newlines(string s) const{
    string curr(s);
    size_t pos = curr.find("\n");
    std::deque<string> ret;

    if (pos == std::string::npos){
        ret.push_back(s);
        return ret;
    }

    while (pos != std::string::npos){
        string new_s = curr.substr(0, pos);
        ret.push_back(new_s);
        curr = curr.substr(pos + 1);
        pos = curr.find("\n");
    }

    pos = curr.size();
    if(pos > 0)
        ret.push_back(curr.substr(0, pos));

    return ret;
}

void CliView::message(string msg) const{
    std::deque<string> l = split_on_newlines(msg);
    auto fst = l.begin();
    cout << "[*] " << *fst << endl;

    for (auto iter = ++fst; iter != l.end(); ++iter){
        cout << "    " << *iter << endl;
    }
}

void CliView::warning(string wrn) const{
    std::deque<string> l = split_on_newlines(wrn);
    auto fst = l.begin();
    cout << "[!] " << *fst << endl;

    for (auto iter = ++fst; iter != l.end(); ++iter){
        cout << "    " << *iter << endl;
    }
}

void CliView::error(string err) const{
    std::deque<string> l = split_on_newlines(err);
    auto fst = l.begin();
    cout << "[!!!] " << *fst << endl;

    for (auto iter = ++fst; iter != l.end(); ++iter){
        cout << "      " << *iter << endl;
    }

    exit(1);
}

void CliView::print(string s) const{
    cout << s << endl;
}

std::istream& CliView::get_input_stream(){
    return std::cin;
}