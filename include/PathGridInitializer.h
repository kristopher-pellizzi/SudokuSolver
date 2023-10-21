#ifndef PATHGRIDINITIALIZER
#define PATHGRIDINITIALIZER

#include <string>
#include "IGridInitializer.h"
#include "View.h"

using std::string;

class PathGridInitializer : public IGridInitializer{
    private:
        string grid_path;
        View* v;

    public:
        PathGridInitializer(string grid_path, View* v);

        void init(Grid& grid);
};

#endif // PATHGRIDINITIALIZER