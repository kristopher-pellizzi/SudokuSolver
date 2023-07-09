#include "Cell.h"

Cell::Cell(unsigned val) : val(val) {}

unsigned Cell::get() const {
    return val;
}

void Cell::set(unsigned val){
    this->val = val;
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