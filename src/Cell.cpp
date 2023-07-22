#include "Cell.h"
#include <sstream>

Cell::Cell(unsigned val, unsigned max_val) : val(val) {
    for(unsigned i = 1; i <= max_val; ++i){
        if(i != val)
            available_vals.insert(i);
    }
}

unsigned Cell::get() const {
    return val;
}

int Cell::set(unsigned val){
    if (available_vals.find(val) == available_vals.end())
        return -1;

    this->val = val;
    available_vals.clear();
    return 0;
}

void Cell::operator=(unsigned val){
    set(val);
}

bool Cell::operator<(const Cell& other) const{
    return val < other.val;
}

bool Cell::operator!=(const int& other) const{
    return val != other;
}

std::ostream& operator<<(std::ostream& stream, Cell& elem){
    stream << elem.val;
    return stream;
}

void Cell::constrain(unsigned val){
    available_vals.erase(val);
}

std::set<unsigned>& Cell::get_available_vals(){
    return available_vals;
}