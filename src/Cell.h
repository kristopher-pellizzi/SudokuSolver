#ifndef CELL
#define CELL

#include <set>

class Cell {
    private:
        unsigned val;
        std::set<unsigned> available_vals;

    public:
        Cell(unsigned val, unsigned max_val);

        unsigned get() const;
        int set(unsigned val);
        void operator=(unsigned val);
        bool operator<(const Cell& other) const;
        bool operator!=(const unsigned& other) const;
        friend std::ostream& operator<<(std::ostream& stream, Cell& elem);
        // Removes val from available_vals
        void constrain(unsigned val);
        std::set<unsigned>& get_available_vals();
};

#endif // CELL