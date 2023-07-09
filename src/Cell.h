#ifndef CELL
#define CELL

#include <iostream>

class Cell {
    private:
        unsigned val;

    public:
        Cell(unsigned val);

        unsigned get() const;
        void set(unsigned val);
        void operator=(unsigned val);
        bool operator<(const Cell& other) const;
        bool operator!=(const int& other) const;
        friend std::ostream& operator<<(std::ostream& stream, Cell& elem);
};

#endif // CELL