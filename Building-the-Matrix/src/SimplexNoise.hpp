/*
* A speed-improved simplex noise algorithm for 2D, 3D and 4D in Java.
*
* Based on example code by Stefan Gustavson (stegu@itn.liu.se).
* Optimisations by Peter Eastman (peastman@drizzle.stanford.edu).
* Better rank ordering method by Stefan Gustavson in 2012.
*
* This could be speeded up even further, but it's useful as it is.
*
* Version 2012-03-09
*
* This code was placed in the public domain by its original author,
* Stefan Gustavson. You may use it as you see fit, but
* attribution is appreciated.
*
* http://webstaff.itn.liu.se/~stegu/simplexnoise/
*
*/

#include "Common.hpp"

///
/// Port of 3D SimplexNoise.java to C++
/// 
class SimplexNoise {  // Simplex noise in 3D

	// Inner class to speed upp gradient computations
	// (array access is a lot slower than member access)
	class Grad
	{
	public:
		double x, y, z;
		Grad() : x(0.0), y(0.0), z(0.0) {}
		Grad(double x, double y, double z) :
			x(x), y(y), z(z)
		{
		}
	};

	Grad grad3[12];

	// To remove the need for index wrapping, double the permutation table length
	short perm[512];
	short permMod12[512];
public:

	static const short p[256];
	SimplexNoise();

	// This method is a *lot* faster than using (int)Math.floor(x)
	int fastfloor(double x) {
		int xi = (int)x;
		return x < xi ? xi - 1 : xi;
	}

	double dot(Grad g, double x, double y, double z) {
		return g.x*x + g.y*y + g.z*z;
	}

	// 3D simplex noise
	double noise(double xin, double yin, double zin);
};

