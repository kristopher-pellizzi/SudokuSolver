#ifndef RANDOMGRIDINITIALIZER
#define RANDOMGRIDINITIALIZER

#include "IGridInitializer.h"
#include "RandomNumberGenerator.h"

class RandomGridInitializer : public IGridInitializer{
    private:
        View* v;

        void init_block(RandomNumberGenerator& rng, unsigned grid_width, Grid& grid, unsigned i, unsigned j);

    public:
        RandomGridInitializer(View* v);

        void init(Grid& grid);
};

#endif // RANDOMGRIDINITIALIZER