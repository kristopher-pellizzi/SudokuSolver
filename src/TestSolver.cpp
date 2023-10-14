#include <set>
#include <sstream>
#include <iostream>
#include "CliView.h"
#include "Solver.h"

Solver::Solver(Grid& grid, View& view) : grid(grid), v(view), selected_cell(NULL), checkpoint_restored(false) {}

void Solver::restore_last_checkpoint(){
    if (checkpoints.size() <= 0){
        std::stringstream sstream;
        sstream << "There are no more checkpoints available to be restored." << std::endl;
        sstream << "This grid has no feasible solution";
        v.error(sstream.str());
    }

    Checkpoint* checkpoint = checkpoints.top();
    checkpoint->restore(grid);
    attempted_coords.clear();
    attempted_vals.clear();
    attempted_coords.insert(checkpoint->attempted_coords.begin(), checkpoint->attempted_coords.end());
    attempted_vals.insert(checkpoint->attempted_vals.begin(), checkpoint->attempted_vals.end());

    if (checkpoint->selected_coord == NULL)
        selected_cell = NULL;
    else
        selected_cell = new Coordinates(*(checkpoint->selected_coord));

    checkpoints.pop();
    delete checkpoint;
    checkpoint_restored = true;
}

Coordinates* Solver::do_search_hidden_single(const Coordinates& c, std::set<Coordinates>& adjacent_cells){
    std::set<unsigned> adjacent_cells_available_vals;

    for (auto iter = adjacent_cells.begin(); iter != adjacent_cells.end(); ++iter){
        std::set<unsigned>& available_vals = grid.get_available_vals(*iter);
        if (available_vals.size() > 0)
            adjacent_cells_available_vals.insert(available_vals.begin(), available_vals.end());
    }

    std::set<unsigned> current_available_vals = grid.get_available_vals(c);
    for (auto iter = adjacent_cells_available_vals.begin(); iter != adjacent_cells_available_vals.end(); ++iter){
        current_available_vals.erase(*iter);
    }

    if (current_available_vals.size() == 1){
        grid.set_hidden_single(c, *(current_available_vals.begin()));
        return new Coordinates(c);
    }

    return NULL;
}

Coordinates* Solver::search_row_hidden_single(const Coordinates& c){
    std::set<Coordinates> row_adjacent_cells = grid.get_row_adjacent_cells(c);
    
    return do_search_hidden_single(c, row_adjacent_cells);
}

Coordinates* Solver::search_col_hidden_single(const Coordinates& c){
    std::set<Coordinates> col_adjacent_cells = grid.get_col_adjacent_cells(c);

    return do_search_hidden_single(c, col_adjacent_cells);
}

Coordinates* Solver::search_block_hidden_single(const Coordinates& c){
    std::set<Coordinates> block_adjacent_cells = grid.get_block_adjacent_cells(c);
    return do_search_hidden_single(c, block_adjacent_cells);
}

Coordinates* Solver::search_hidden_single(){
    unsigned grid_width = grid.get_grid_width();
    Coordinates* ret = NULL;

    for (unsigned i = 0; i < grid_width; ++i){
        for (unsigned j = 0; j < grid_width; ++j){
            Coordinates c(i, j);

            ret = search_row_hidden_single(c);

            if (ret == NULL)
                ret = search_col_hidden_single(c);

            if (ret == NULL)
                ret = search_block_hidden_single(c);

            if (ret != NULL)
                return ret;
        }
    }

    return NULL;
}

Coordinates Solver::select_cell() {
    unsigned grid_width = grid.get_grid_width();
    Coordinates selected(0, 0);
    unsigned min_available_vals = UINT_MAX;

    /*
        If selected_cell is not NULL, then the last restored checkpoint was stored while choosing
        a value for that cell.
        Select the same cell, a new value will be attempted
    */
    if (selected_cell != NULL){
        return *selected_cell;
    }

    /*
        If all the empty cells have been attempted without success, this is an infeasible solution.
        Try restoring the last checkpoint.
    */
    if ((attempted_coords.size() + grid.get_filled_cells()) == grid_width * grid_width){
        std::stringstream sstream;
        sstream << "All coords have been attempted. Restoring last checkpoint...";
        v.message(sstream.str());

        restore_last_checkpoint();
        return selected;
    }

    for (unsigned i = 0; i < grid_width; ++i){
        for (unsigned j = 0; j < grid_width; ++j){
            Coordinates c(i, j);

            // If cell already has a value or it has already be attempted, ignore it
            if (grid[i][j] != 0 || attempted_coords.find(c) != attempted_coords.end())
                continue;

            std::set<unsigned>& available_vals = grid.get_available_vals(i, j);

            /*
                If there's any cell with no available values, this solution is infeasible.
                Try restoring the last checkpoint and proceed from there
            */
            if(available_vals.size() <= 0 && !grid.is_filled(i, j)){
                std::stringstream sstream;
                sstream << "Cell " << c.to_string() << " has no more available values" << std::endl;
                sstream << "Current state is unsolvable. Restoring last checkpoint...";
                v.message(sstream.str());

                restore_last_checkpoint();                
                return selected;
            }

            // Try to find the most constrained cell
            if (available_vals.size() < min_available_vals){
                min_available_vals = available_vals.size();
                selected = c;
            }
        }
    }

    /*
        If the selected cell has more than a single available value, search the grid for possible hidden singles, i.e. cells that have more than 1 available value,
        but that are the only cell in the block that can hold a certain value due to row, column and/or block constraints.
    */
    if (min_available_vals > 1){
        Coordinates* hidden_single = search_hidden_single();
        if (hidden_single != NULL){
            std::stringstream sstream;
            sstream << "Cell " << hidden_single->to_string() << " is an hidden single, so it is selected" << std::endl;
            v.message(sstream.str());
            selected = Coordinates(*hidden_single);
            min_available_vals = 1;
            delete hidden_single;
        }
    }

    /*
        If available vals size is > 1, create and push a new checkpoint. 
        Store in the checkpoint the current coords as attempted.
        Note that if available vals size is 1, there's no need for a checkpoint.
        Indeed, that would be a forced value for that cell.
        If no solution is found setting that value to this cell, the grid has no feasible solutions
    */
    std::stringstream sstream;
    if (min_available_vals > 1){
        sstream << "Creating checkpoint after choosing cell " << selected.to_string();
        v.message(sstream.str());
        Checkpoint* checkpoint = new Checkpoint(grid);
        checkpoint->attempted_coords.insert(attempted_coords.begin(), attempted_coords.end());
        checkpoint->attempted_coords.insert(selected);
        checkpoints.push(checkpoint);
    }

    sstream.str("");
    sstream << "Selected most constrained cell (" << min_available_vals << " constraints) with coordinates " << selected.to_string();
    v.message(sstream.str());

    return selected;
}

unsigned Solver::select_val(Coordinates& c) {
    unsigned max = 0;
    unsigned selected_val = 0;

    std::set<unsigned>& available_vals = grid.get_available_vals(c);

    if (attempted_vals.size() == available_vals.size()){
        std::stringstream sstream;
        sstream << "All values have been attempted for this cell. Restoring last checkpoint...";
        v.message(sstream.str());

        restore_last_checkpoint();
        return selected_val;
    }

    if (available_vals.size() == 1){
        selected_val = *available_vals.begin();
        std::stringstream sstream;
        sstream << "Selected value " << selected_val << " as it is the only one possible";
        v.message(sstream.str());
        return selected_val;
    }

    for(auto iter = available_vals.begin(); iter != available_vals.end(); ++iter){
        if (attempted_vals.find(*iter) != attempted_vals.end())
            continue;
            
        unsigned min_constraints_num = grid.simulate_add_constraint(c.get_x(), c.get_y(), *iter);

        if (min_constraints_num == 0){
            std::stringstream sstream;
            sstream << "By setting value " << *iter << " to this cell, another cell would have no more available values" << std::endl;
            sstream << "Restoring last checkpoint...";
            v.message(sstream.str());

            restore_last_checkpoint();
            return selected_val;
        }

        if (min_constraints_num >= max){
            max = min_constraints_num;
            selected_val = *iter;
        }
    }

    // Create and push a new checkpoint. Remove the current value from available values in the checkpoint
    std::stringstream sstream;
    sstream << "Creating checkpoint after choosing value " << selected_val;
    v.message(sstream.str());
    Checkpoint* checkpoint = new Checkpoint(grid);
    checkpoint->attempted_vals.insert(attempted_vals.begin(), attempted_vals.end());
    checkpoint->attempted_vals.insert(selected_val);
    checkpoint->selected_coord = new Coordinates(c);
    checkpoints.push(checkpoint);

    sstream.str("");
    sstream << "Selected value " << selected_val << " as it is the less constraining value";
    v.message(sstream.str());

    return selected_val;
}

void Solver::clean(){
    while (checkpoints.size() > 0){
        Checkpoint* c = checkpoints.top();
        delete c;
        checkpoints.pop();
    }
}

void Solver::solve() {
    std::stringstream sstream;
    sstream << "Start solving the grid...";
    v.message(sstream.str());

    unsigned block_width = grid.get_block_width();
    unsigned grid_width = block_width * block_width;
    // Insert all not filled coordinates as attempted
    for(unsigned i = 0; i < grid_width; ++i){
        for(unsigned j = 0; j < grid_width; ++j){
            Coordinates c(i, j);
            if (!grid.is_filled(c)){
                attempted_coords.insert(c);
            }
        }
    }

    sstream.str("");

    Coordinates selected(0, 3);
    // Store checkpoint
    sstream << "Creating checkpoint after choosing cell " << selected.to_string();
    v.message(sstream.str());
    Checkpoint* checkpoint = new Checkpoint(grid);
    checkpoint->attempted_coords.insert(attempted_coords.begin(), attempted_coords.end());
    checkpoint->attempted_coords.insert(selected);
    checkpoints.push(checkpoint);

    // Restore checkpoint
    v.message("Restoring..");
    restore_last_checkpoint();

    sstream.str("");
    sstream << "attempted_coords: " << std::endl;
    for(auto iter = attempted_coords.begin(); iter != attempted_coords.end(); ++iter){
        sstream << iter->to_string() << std::endl;
    }
    v.message(sstream.str());

    Coordinates&& c = select_cell();

    sstream.str("");
    sstream << UINT_MAX << std::endl;
    v.message(sstream.str());
    v.message(c.to_string());

    v.draw();
}