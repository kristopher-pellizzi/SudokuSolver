#include <chrono>
#include <random>
#include <iostream>
#include "RandomNumberGenerator.h"

RandomNumberGenerator::RandomNumberGenerator(){
    uint64_t seed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    rng.seed(seed);
}

RandomNumberGenerator::RandomNumberGenerator(const RandomNumberGenerator& other){
    rng = other.rng;
}

RandomNumberGenerator& RandomNumberGenerator::operator=(const RandomNumberGenerator& other){
    rng = other.rng;

    return *this;
}

unsigned RandomNumberGenerator::randInt(unsigned start, unsigned end){
    unsigned diff = end - start + 1;

    return (rng() % diff) + start;
}