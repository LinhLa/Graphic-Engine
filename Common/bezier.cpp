#include "stdafx.h"
#include <math.h>
#include "bezier.h"
double factorial(double num)
{
    if(num <= 1){
      return 1;
    }
    double fac = 1;
    for(double i = 1; i <= num; i++)
    {
        fac *= i;
    }
    return fac;
}

double choose(double a, double b )
{
  return factorial(a) / (factorial(b ) * factorial(a - b ));
}