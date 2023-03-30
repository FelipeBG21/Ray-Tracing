#ifndef MOUSEQUEHERRAMIENTAS_H
#define MOUSEQUEHERRAMIENTAS_H

#include <memory>
#include <limits>
#include <cstdlib>

#include <random>


using std::shared_ptr;
using std::make_shared;

const double infinity = std::numeric_limits<double>::infinity();
// const double pi = 3.1415926535897932385;


double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

/*
inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}
*/

double clamp(double arg_x, double min, double max) {
    if (arg_x < min) return min;
    if (arg_x > max) return max;
    return arg_x;
}

#include "ray.h"
#include "vector.h"

#endif