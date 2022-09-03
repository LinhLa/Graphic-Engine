#pragma once
#include <cstdint>
#include <vector>

double factorial(double num);
double choose(double a, double b );

template<class POINT>
POINT bezier(std::vector<POINT>& points, double t)
{
    POINT p;
    std::size_t len = points.size();
    for(std::size_t i = 0; i < len; i++)
    {
        p += points[i] * choose(len - 1, i) * pow(1 - t, len - 1 - i) * pow(t, i);
    }
    return p;
}