#ifndef RANDOMNUMBERGENERATOR
#define RANDOMNUMBERGENERATOR

#include <random>

class RandomNumberGenerator{
    private:
        std::mt19937 rng;

    public:
        RandomNumberGenerator();
        RandomNumberGenerator(const RandomNumberGenerator& other);
        RandomNumberGenerator& operator=(const RandomNumberGenerator& other);

        /*
            Generates a random integer between start and end, both range ends included
        */
        unsigned randInt(unsigned start, unsigned end);
};

#endif // RANDOMNUMBERGENERATOR