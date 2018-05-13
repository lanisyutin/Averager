// Integrator.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

int main()
{
	std::ifstream in1;
	in1.open("source.txt");
	double y, p, u, v, h=0.0416;
	double u_prior;
	double integral = 0;
	in1 >> y >> p >> u_prior >> v;
	while (!in1.eof()) {
		in1 >> y >> p >> u >> v;
		integral += (u_prior + u) / 2 * h;
		u_prior = u;
	}
	cout << integral;
    return 0;
}

