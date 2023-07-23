#include <set>
#include <sstream>
#include <iostream>
#include "CliView.h"
#include "Solver.h"

Solver::Solver(Grid& grid, View& view) : grid(grid), v(view), checkpoint_restored(false) {}

void Solver::restore_last_checkpoint(){
    if (checkpoints.size() <= 0){
        std::stringstream sstream;
        sstream << "There are no more checkpoints available to be restored." << std::endl;
        sstream << "This grid has no feasible solution";
        v.error(sstream.str());
    }

    Checkpoint& checkpoint = checkpoints.top();
    checkpoint.restore(grid);
    attempted_coords.clear();
    attempted_vals.clear();
    attempted_coords.insert(checkpoint.attempted_coords.begin(), checkpoint.attempted_coords.end());
    attempted_vals.insert(checkpoint.attempted_vals.begin(), checkpoint.attempted_vals.end());

    if (checkpoint.selected_coord == NULL)
        selected_cell = NULL;
    else
        selected_cell = new Coordinates(*checkpoint.selected_coord);

    checkpoints.pop();
    checkpoint_restored = true;
}

Coordinates Solver::select_cell() {
    unsigned grid_width = grid.get_grid_width();
    Coordinates selected;
    unsigned min_available_vals = UINT_MAX;

    if (selected_cell != NULL){
        return *selected_cell;
    }

    if (attempted_coords.size() == grid_width * grid_width){
        std::stringstream sstream;
        sstream << "All coords have been attempted. Restoring last checkpoint...";
        v.message(sstream.str());

        restore_last_checkpoint();
    }

    for (unsigned i = 0; i < grid_width; ++i){
        for (unsigned j = 0; j < grid_width; ++j){
            Coordinates c(i, j);

            // If cell already has a value or it has already be attempted, ignore it
            if (grid[i][j] != 0 || attempted_coords.find(c) != attempted_coords.end())
                continue;

            std::set<unsigned>& available_vals = grid.get_available_vals(i, j);

            if(available_vals.size() <= 0){
                std::stringstream sstream;
                sstream << "Cell " << c.to_string() << " has no more available values" << std::endl;
                sstream << "Current state is unsolvable. Restoring last checkpoint...";
                v.message(sstream.str());

                restore_last_checkpoint();                
                // Restart looking for a new cell
                i = 0;
                j = 0;
            }

            // Try to find the most constrained cell
            if (available_vals.size() < min_available_vals){
                min_available_vals = available_vals.size();
                selected = c;
            }
        }
    }

    // If available vals size is > 1, create and push a new checkpoint. Store in the checkpoint the current coords as attempted
    if (min_available_vals > 1){
        Checkpoint checkpoint(grid);
        checkpoint.attempted_coords.insert(selected);
        checkpoints.push(checkpoint);
    }

    std::stringstream sstream;
    sstream << "Selected most constrained cell (" << min_available_vals << " constraints) with coordinates " << selected.to_string();
    v.message(sstream.str());

    return selected;
}

unsigned Solver::select_val(Coordinates c) {
    unsigned max = 0;
    unsigned selected_val = 0;

    std::set<unsigned>& available_vals = grid.get_available_vals(c);

    if (available_vals.size() == 1){
        selected_val = *available_vals.begin();
        std::stringstream sstream;
        sstream << "Selected value " << selected_val << " as it is the only one possible";
        v.message(sstream.str());
        return selected_val;
    }

    for(auto iter = available_vals.begin(); iter != available_vals.end(); ++iter){
        unsigned min_constraints_num = grid.simulate_add_constraint(c.get_x(), c.get_y(), *iter);

        if (min_constraints_num >= max){
            max = min_constraints_num;
            selected_val = *iter;
        }
    }

    // TODO: Create and push a new checkpoint. Remove the current value from available values in the checkpoint
    // Store in the checkpoint the current iterator
    // so that we can restore this method from the subsequent value.
    // If there are no more values available, pop another checkpoint

    std::stringstream sstream;
    sstream << "Selected value " << selected_val << " as it is the less constraining value";
    v.message(sstream.str());

    return selected_val;
}

void Solver::solve() {
    while (!grid.is_completed()){
        Coordinates&& c = select_cell();
        unsigned val = select_val(c);
        // If a checkpoint was restored, no cell must be set
        if (!checkpoint_restored){
            grid.set(c, val);
            attempted_coords.clear();
            attempted_vals.clear();
        }
        v.draw();

        std::string s;
        std::cin >> s;
    }
}