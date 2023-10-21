#include <fstream>
#include <sstream>
#include "PathGridInitializer.h"

PathGridInitializer::PathGridInitializer(string grid_path, View* v) : 
    grid_path(grid_path), 
    v(v)
{}


void PathGridInitializer::init(Grid& grid){
    unsigned block_width = grid.get_block_width();
    unsigned grid_width = block_width * block_width;

    v->message("Initializing grid...");
    std::ifstream init_file(grid_path);

    for (int i = 0; i < grid_width; ++i){
        for (int j = 0; j < grid_width; ++j){
            unsigned val;
            init_file >> val;
            if (val < 0 || val > grid_width){
                std::stringstream sstream;
                sstream << "Invalid value " << val << "." << std::endl;
                sstream << "Check initialization values.";
                v->error(sstream.str());
            }

            if (val != 0)
                grid.set(i, j, val);
        }
    }

    init_file.close();
    grid.init_constraints();
    grid.check_initial_constraints();
}