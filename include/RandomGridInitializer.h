#ifndef RANDOMGRIDINITIALIZER
#define RANDOMGRIDINITIALIZER

#include "IGridInitializer.h"

class RandomGridInitializer : public IGridInitializer{
    public:
        void init(Grid& grid);
};

#endif // RANDOMGRIDINITIALIZER