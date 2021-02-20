/*
TERMS OF USE - EASING EQUATIONS

Open source under the BSD License.

Copyright © 2001 Robert Penner
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
Neither the name of the author nor the names of contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "stdafx.h"
#include <math.h>
#include "easing.h"
#define PI 3.14159265

/*Four thing to learn:
t: current time
b: start value
c: change value
d: duration
Note: t and d could be frame per second/seconds/miliseconds*/

//simple linear tweening - no easing, no acceleration
double linearTween(double t,double b, double c, double d)
{
	return c*t/d + b;
}

// quadratic easing in - accelerating from zero velocity
double easeInQuad(double t,double b, double c, double d)
{
	t /= d;
	return c*t*t + b;
}

// quadratic easing out - decelerating to zero velocity
double easeOutQuad(double t,double b, double c, double d)
{
	t /= d;
	return -c * t*(t-2) + b;
}

// quadratic easing in/out - acceleration until halfway, then deceleration
double easeInOutQuad(double t,double b, double c, double d)
{
	t /= d/2;
	if (t < 1) return c/2*t*t + b;
	t--;
	return -c/2 * (t*(t-2) - 1) + b;
}

// cubic easing in - accelerating from zero velocity
double easeInCubic(double t,double b, double c, double d)
{
	t /= d;
	return c*t*t*t + b;
}

// cubic easing out - decelerating to zero velocity
double easeOutCubic(double t,double b, double c, double d)
{
	t /= d;
	t--;
	return c*(t*t*t + 1) + b;
}

// cubic easing in/out - acceleration until halfway, then deceleration
double easeInOutCubic(double t,double b, double c, double d)
{
	t /= d/2;
	if (t < 1) return c/2*t*t*t + b;
	t -= 2;
	return c/2*(t*t*t + 2) + b;
}

// quartic easing in - accelerating from zero velocity
double easeInQuart(double t,double b, double c, double d)
{
	t /= d;
	return c*t*t*t*t + b;
}

// quartic easing out - decelerating to zero velocity
double easeOutQuart(double t,double b, double c, double d)
{
	t /= d;
	t--;
	return -c * (t*t*t*t - 1) + b;
}

// quartic easing in/out - acceleration until halfway, then deceleration
double easeInOutQuart(double t,double b, double c, double d)
{
	t /= d/2;
	if (t < 1) return c/2*t*t*t*t + b;
	t -= 2;
	return -c/2 * (t*t*t*t - 2) + b;
}

// quintic easing in - accelerating from zero velocity
double easeInQuint(double t,double b, double c, double d)
{
	t /= d;
	return c*t*t*t*t*t + b;
}

// quintic easing out - decelerating to zero velocity
double easeOutQuint(double t,double b, double c, double d)
{
	t /= d;
	t--;
	return c*(t*t*t*t*t + 1) + b;
}

// quintic easing in/out - acceleration until halfway, then deceleration
double easeInOutQuint(double t,double b, double c, double d)
{
	t /= d/2;
	if (t < 1) return c/2*t*t*t*t*t + b;
	t -= 2;
	return c/2*(t*t*t*t*t + 2) + b;
}

// sinusoidal easing in - accelerating from zero velocity
double easeInSine(double t,double b, double c, double d)
{
	return -c * cos(t/d * (PI/2)) + c + b;
}

// sinusoidal easing out - decelerating to zero velocity
double easeOutSine(double t,double b, double c, double d)
{
	return c * sin(t/d * (PI/2)) + b;
}

// sinusoidal easing in/out - accelerating until halfway, then decelerating
double easeInOutSine(double t,double b, double c, double d)
{
	return -c/2 * (cos(PI*t/d) - 1) + b;
}

// exponential easing in - accelerating from zero velocity
double easeInExpo(double t,double b, double c, double d)
{
	return c * pow( 2, 10 * (t/d - 1) ) + b;
}

// exponential easing out - decelerating to zero velocity
double easeOutExpo(double t,double b, double c, double d)
{
	return c * ( -pow( 2, -10 * t/d ) + 1 ) + b;
}

// exponential easing in/out - accelerating until halfway, then decelerating
double easeInOutExpo(double t,double b, double c, double d)
{
	t /= d/2;
	if (t < 1) return c/2 * pow( 2, 10 * (t - 1) ) + b;
	t--;
	return c/2 * ( -pow( 2, -10 * t) + 2 ) + b;
}

// circular easing in - accelerating from zero velocity
double easeInCirc(double t,double b, double c, double d)
{
	t /= d;
	return -c * (sqrt(1 - t*t) - 1) + b;
}

// circular easing out - decelerating to zero velocity
double easeOutCirc(double t,double b, double c, double d)
{
	t /= d;
	t--;
	return c * sqrt(1 - t*t) + b;
}

// circular easing in/out - acceleration until halfway, then deceleration
double easeInOutCirc(double t,double b, double c, double d)
{
	t /= d/2;
	if (t < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b;
	t -= 2;
	return c/2 * (sqrt(1 - t*t) + 1) + b;
};
