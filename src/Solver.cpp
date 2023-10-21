#include <set>
#include <sstream>
#include <iostream>
#include "CliView.h"
#include "Solver.h"

Solver::Solver(Grid& grid, View& view, bool quiet_mode_enabled) : grid(grid), v(view), selected_cell(NULL), candidate_locked(false), checkpoint_restored(false), quiet_mode_enabled(quiet_mode_enabled) {}

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
    row_locked_candidates.clear();
    col_locked_candidates.clear();
    attempted_coords.insert(checkpoint->attempted_coords.begin(), checkpoint->attempted_coords.end());
    attempted_vals.insert(checkpoint->attempted_vals.begin(), checkpoint->attempted_vals.end());
    row_locked_candidates.insert(checkpoint->row_locked_candidates.begin(), checkpoint->row_locked_candidates.end());
    col_locked_candidates.insert(checkpoint->col_locked_candidates.begin(), checkpoint->col_locked_candidates.end());

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

bool Solver::try_lock_row_candidates(unsigned block_idx, unsigned first_row_idx, const std::set<Coordinates>& block_cells){
    unsigned block_width = grid.get_block_width();
    std::set<Coordinates> block_rows[block_width];
    std::set<unsigned> rows_vals[block_width];

    for(auto iter = block_cells.begin(); iter != block_cells.end(); ++iter){
        block_rows[iter->get_x() % block_width].insert(*iter);
    }

    // Iterate for each row
    for(unsigned i = 0; i < block_width; ++i){
        std::set<unsigned> row_available_vals;

        for(auto row_iter = block_rows[i].begin(); row_iter != block_rows[i].end(); ++row_iter){
            std::set<unsigned> iter_avail_vals = grid.get_available_vals(*row_iter);
            row_available_vals.insert(iter_avail_vals.begin(), iter_avail_vals.end());
        }

        rows_vals[i] = row_available_vals;
    }

    for(unsigned i = 0; i < block_width; ++i){
        // If this row has already locked values in this block, ignore it
        if(row_locked_candidates.find(LockedCandidateIndex(block_idx, i)) != row_locked_candidates.end())
            continue;

        std::set<unsigned> diff;
        diff.insert(rows_vals[i].begin(), rows_vals[i].end());

        for(unsigned j = 0; j < block_width; ++j){
            if (i == j)
                continue;

            for(auto iter = rows_vals[j].begin(); iter != rows_vals[j].end(); ++iter){
                diff.erase(*iter);
            }
        }

        if (diff.size() > 0){
            candidate_locked = true;
            row_locked_candidates.insert(LockedCandidateIndex(block_idx, i));
            grid.set_row_locked_candidates(block_idx, i, diff);

            if (!quiet_mode_enabled){
                std::stringstream sstream;

                sstream << (diff.size() > 1 ? "Values " : "Value ");
                for(auto iter = diff.begin(); iter != (--diff.end()); ++iter){
                    sstream << *iter << ", ";
                }
                sstream << *(--diff.end()) << " can only be inserted in row " << i << " of block " << block_idx << std::endl;
                v.message(sstream.str());
            }

            return true;
        }
    }

    return false;
}

bool Solver::try_lock_col_candidates(unsigned block_idx, unsigned first_col_idx, const std::set<Coordinates>& block_cells){
    unsigned block_width = grid.get_block_width();
    std::set<Coordinates> block_cols[block_width];
    std::set<unsigned> cols_vals[block_width];

    for(auto iter = block_cells.begin(); iter != block_cells.end(); ++iter){
        block_cols[iter->get_y() % block_width].insert(*iter);
    }

    // Iterate for each col
    for(unsigned i = 0; i < block_width; ++i){
        std::set<unsigned> col_available_vals;

        for(auto col_iter = block_cols[i].begin(); col_iter != block_cols[i].end(); ++col_iter){
            std::set<unsigned> iter_avail_vals = grid.get_available_vals(*col_iter);
            col_available_vals.insert(iter_avail_vals.begin(), iter_avail_vals.end());
        }

        cols_vals[i] = col_available_vals;
    }

    for(unsigned i = 0; i < block_width; ++i){
        // If this column has already locked values in this block, ignore it
        if(col_locked_candidates.find(LockedCandidateIndex(block_idx, i)) != col_locked_candidates.end())
            continue;

        std::set<unsigned> diff;
        diff.insert(cols_vals[i].begin(), cols_vals[i].end());

        for(unsigned j = 0; j < block_width; ++j){
            if (i == j)
                continue;

            for(auto iter = cols_vals[j].begin(); iter != cols_vals[j].end(); ++iter){
                diff.erase(*iter);
            }
        }

        if (diff.size() > 0){
            candidate_locked = true;
            col_locked_candidates.insert(LockedCandidateIndex(block_idx, i));
            grid.set_col_locked_candidates(block_idx, i, diff);

            if (!quiet_mode_enabled){
                std::stringstream sstream;

                sstream << (diff.size() > 1 ? "Values " : "Value ");
                for(auto iter = diff.begin(); iter != (--diff.end()); ++iter){
                    sstream << *iter << ", ";
                }
                sstream << *(--diff.end()) << " can only be inserted in col " << i << " of block " << block_idx << std::endl;
                v.message(sstream.str());
            }

            return true;
        }
    }

    return false;
}

bool Solver::try_lock_candidates(){
    unsigned block_width = grid.get_block_width();
    
    for(unsigned i = 0; i < block_width; ++i){
        for(unsigned j = 0; j < block_width; ++j){
            Coordinates c(i * block_width, j * block_width);
            unsigned block_idx = i * block_width + j;

            // Get all the cells belonging to this block
            std::set<Coordinates> block_cells = grid.get_block_adjacent_cells(c);
            block_cells.insert(c);

            // Avoid attempting to lock col candidates if a row candidate has already been locked
            if (try_lock_row_candidates(block_idx, i, block_cells))
                return true;

            if (try_lock_col_candidates(block_idx, j, block_cells))
                return true;
        }
    }
    return false;
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
        If all the cells have been attempted without success, this is an infeasible solution.
        Try restoring the last checkpoint.
    */
    if ((attempted_coords.size() + grid.get_filled_cells()) == grid_width * grid_width){
        if (!quiet_mode_enabled){
            std::stringstream sstream;
            sstream << "All coords have been attempted. Restoring last checkpoint...";
            v.message(sstream.str());
        }

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
                if (!quiet_mode_enabled){
                    std::stringstream sstream;
                    sstream << "Cell " << c.to_string() << " has no more available values" << std::endl;
                    sstream << "Current state is unsolvable. Restoring last checkpoint...";
                    v.message(sstream.str());
                }

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
            if (!quiet_mode_enabled){
                std::stringstream sstream;
                sstream << "Cell " << hidden_single->to_string() << " is an hidden single, so it is selected" << std::endl;
                v.message(sstream.str());
            }
            selected = Coordinates(*hidden_single);
            min_available_vals = 1;
            delete hidden_single;
        }
    }

    if (min_available_vals > 1 && try_lock_candidates())
        return selected;

    /*
        Cell selection checkpoints are meaningful only in case the checkpoint stack is empty.
        In case it is not, indeed, if at a certain point any cell will have no more available values,
        then we would have attempted all the available values of the most contrained cell, so
        the grid is infeasible and it is required to change either the value chosen for the first "randomly" 
        selected cell or the first selected cell.
        If available vals size is > 1, create and push a new checkpoint. 
        Store in the checkpoint the current coords as attempted.
        Note that if available vals size is 1, there's no need for a checkpoint.
        Indeed, that would be a forced value for that cell.
        If no solution is found setting that value to this cell, the grid has no feasible solutions
    */
    std::stringstream sstream;
    if (checkpoints.size() == 0 && min_available_vals > 1){
        sstream << "Creating checkpoint after choosing cell " << selected.to_string();
        if (!quiet_mode_enabled)
            v.message(sstream.str());
        Checkpoint* checkpoint = new Checkpoint(grid);
        checkpoint->attempted_coords.insert(attempted_coords.begin(), attempted_coords.end());
        checkpoint->attempted_coords.insert(selected);
        checkpoint->row_locked_candidates.insert(row_locked_candidates.begin(), row_locked_candidates.end());
        checkpoint->col_locked_candidates.insert(col_locked_candidates.begin(), col_locked_candidates.end());
        checkpoints.push(checkpoint);
    }

    sstream.str("");
    sstream << "Selected most constrained cell (" << min_available_vals << " constraints) with coordinates " << selected.to_string();
    if (!quiet_mode_enabled)
        v.message(sstream.str());

    return selected;
}

unsigned Solver::select_val(Coordinates& c) {
    unsigned max = 0;
    unsigned selected_val = 0;

    std::set<unsigned>& available_vals = grid.get_available_vals(c);

    if (attempted_vals.size() == available_vals.size()){
        if (!quiet_mode_enabled){
            std::stringstream sstream;
            sstream << "All values have been attempted for this cell. Restoring last checkpoint...";
            v.message(sstream.str());
        }

        restore_last_checkpoint();
        return selected_val;
    }

    if (available_vals.size() == 1){
        selected_val = *available_vals.begin();
        if (!quiet_mode_enabled){
            std::stringstream sstream;
            sstream << "Selected value " << selected_val << " as it is the only one possible";
            v.message(sstream.str());
        }
        return selected_val;
    }

    for(auto iter = available_vals.begin(); iter != available_vals.end(); ++iter){
        if (attempted_vals.find(*iter) != attempted_vals.end())
            continue;
            
        unsigned min_constraints_num = grid.simulate_add_constraint(c.get_x(), c.get_y(), *iter);

        if (min_constraints_num == 0){
            if (!quiet_mode_enabled){
                std::stringstream sstream;
                sstream << "By setting value " << *iter << " to this cell, another cell would have no more available values" << std::endl;
                sstream << "Restoring last checkpoint...";
                v.message(sstream.str());
            }

            restore_last_checkpoint();
            return selected_val;
        }

        if (min_constraints_num >= max){
            max = min_constraints_num;
            selected_val = *iter;
        }
    }

    std::stringstream sstream;
    // Create a new checkpoint only if there are other available values that have not been attempted yet
    if (available_vals.size() - attempted_vals.size() > 1){
        // Create and push a new checkpoint. Remove the current value from available values in the checkpoint
        sstream << "Creating checkpoint after choosing value " << selected_val;
        if (!quiet_mode_enabled)
            v.message(sstream.str());
        Checkpoint* checkpoint = new Checkpoint(grid);
        checkpoint->attempted_vals.insert(attempted_vals.begin(), attempted_vals.end());
        checkpoint->attempted_vals.insert(selected_val);
        checkpoint->selected_coord = new Coordinates(c);
        checkpoints.push(checkpoint);
    }

    sstream.str("");
    sstream << "Selected value " << selected_val << " as it is the less constraining value";
    if (!quiet_mode_enabled)
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
    if (!quiet_mode_enabled)
        v.message(sstream.str());

    while (!grid.is_completed()){
        Coordinates&& c = select_cell();
        unsigned val = 0;

        /*
            Select a value for the cell only if a candidate has not been
            locked AND the cell selection did not perform a checkpoint restore
        */
        if (!candidate_locked && !checkpoint_restored)
            val = select_val(c);

        // If a candidate has been locked or a checkpoint was restored during cell or value selection, no cell must be set
        if (!candidate_locked && !checkpoint_restored){
            grid.set(c, val);
            attempted_coords.clear();
            attempted_vals.clear();
            row_locked_candidates.clear();
            col_locked_candidates.clear();
            if (selected_cell != NULL){
                delete selected_cell;
                selected_cell = NULL;
            }
        }

        candidate_locked = false;
        checkpoint_restored = false;
        if (!quiet_mode_enabled)
            v.draw();

        // std::cin.get();
    }

    sstream.str("");
    sstream << "Grid has been completely filled." << std::endl;
    sstream << "Here's the solution" << std::endl;
    if (!quiet_mode_enabled){
        v.message(sstream.str());
        v.draw();
    }
    clean();
}