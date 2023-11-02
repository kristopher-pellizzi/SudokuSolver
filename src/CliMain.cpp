#include "CliView.h"
#include "Grid.h"
#include "Solver.h"
#include "ArgumentParser.h"
#include "PathGridInitializer.h"
#include "RandomGridInitializer.h"

using namespace AP;

ArgumentsMap parse_args(int argc, char* argv[]){
    ArgumentParser parser(argc, argv);

    parser.add_argument(
        NamedArgument<string>("name", "--grid-path"),
        NamedArgument<string>("abbreviation", "-p"),
        NamedArgument<string>("help_string", "Path of a text file describing the initial state of the Sudoku grid (see files in folder \"grids\" for reference). If a path is not provided, a random grid will be generated"),
        NamedArgument<string>("dest", "path"),
        NamedArgument<string*>("default_val", new string(""))
    );

    parser.add_argument(
        NamedArgument<string>("name", "--block-width"),
        NamedArgument<string>("abbreviation", "-w"),
        NamedArgument<string>("help_string", "Width of a block of the Sudoku grid. For instance, for a classic Sudoku grid, each block is 3x3 cells. In that case, this argument would have value 3."),
        NamedArgument<string>("dest", "block_width"),
        NamedArgument<string*>("default_val", new string("3")),
        NamedArgument<Converter*>("arg_converter", UnsignedConverter::get_instance())
    );

    return parser.parse_args();
}

int main(int argc, char* argv[]){
    ArgumentsMap args = parse_args(argc, argv);
    unsigned block_width;
    string grid_path;

    args["block_width"]->get_value(&block_width);
    args["path"]->get_value(&grid_path);

    Grid grid(block_width);    
    CliView v(grid);
    grid.set_view(v);

    IGridInitializer* initializer;
    if (grid_path != "")
        initializer = new PathGridInitializer(grid_path, &v);
    else   
        initializer = new RandomGridInitializer(&v);

    initializer->init(grid);
    v.draw(&grid);

    Solver solver(grid, v);
    solver.solve();
    return 0;
}