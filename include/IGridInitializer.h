#ifndef IGRIDINITIALIZER
#define IGRIDINITIALIZER

#include "Grid.h"

class IGridInitializer{
    public:
        virtual void init(Grid& grid) = 0;
};

#endif // IGRIDINITIALIZER