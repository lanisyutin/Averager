// Averager.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;


struct DataPoint {
	double x;
	double y;
	double p;
	double u;
	double v;
	double fx;
	double fy;
	DataPoint();
	DataPoint(double x, double y, double p, double u, double v, double fx, double fy) {
		this->x = x;
		this->y = y;
		this->p = p;
		this->u = u;
		this->v = v;
		this->fx = fx;
		this->fy = fy;
	}
	DataPoint operator+(DataPoint a) {
		this->x += a.x;
		this->y += a.y;
		this->p += a.p;
		this->u += a.u;
		this->v += a.v;
		this->fx += a.fx;
		this->fy += a.fy;
		return *this;
	}
};

void main() {
	std::ifstream in1, in2, config;
	std::ofstream out;
	config.open("config.txt");
	
	int time_step;
	double X;
	int Ny;
	int time_count = 0;
	vector<DataPoint> SpaceSectionOverTime;

	path dir1;
	config >> dir1 >> X >> Ny;
	out.open("AveragingOverTime[x=" + to_string(X) + "].plt");
	cout << "Averagin over time date from " << dir1 << endl;

	try
	{
		if (exists(dir1))
		{
			if (is_regular_file(dir1))
				cout << dir1 << " it is a file not a folder!! " << '\n';

			else if (is_directory(dir1))
			{
				for (directory_entry& x : directory_iterator(dir1)) {
					int n = 0;
					cout << '.';
					if (is_regular_file(x.path()) && x.path().extension() == ".plt"&& x.path().filename().string().find("step") !=-1) {
						time_count++;
						in1.open((x.path()).c_str());
						string s1;
						vector<DataPoint> SpaceSection;
						for (int i = 0; i < 4; i++)
							getline(in1, s1);
						while (!in1.eof()) {
							double x;
							double y;
							double p;
							double u;
							double v;
							double fx;
							double fy;
							in1 >> x >> y >> p >> u >> v >> fx >> fy;
							//int index = (int)s1.find_first_of(' ');
							//string x_current = s1.substr(0, index);
							if (X == x) {
								DataPoint p(x, y, p, u, v, fx, fy);
								SpaceSection.push_back(p);
								//out << s1 << endl;
								n++;
							}
							if (n == Ny) {
								if (time_count == 1)
									SpaceSectionOverTime = SpaceSection;
								else {
									for (int i = 0; i < SpaceSection.size(); i++) {
										SpaceSectionOverTime[i] = SpaceSectionOverTime[i] + SpaceSection[i];
									}
								}
								in1.close();
								break;
							}
						}
					}
				}
				out << "title = \"Averaging over time in x= "+ to_string(X)+"\"\nVariables = x y p u v fx fy" << endl;
				for (auto point : SpaceSectionOverTime) {
					out << point.x / time_count << " " 
						<< point.y / time_count << " " 
						<< point.p / time_count << " " 
						<< point.u / time_count << " " 
						<< point.v / time_count << " " 
						<< point.fx / time_count << " "
						<< point.fy / time_count << endl;
				}

			}
			else
				cout << dir1 << " exists, but is not a regular file or directory\n";
		}
		else
			cout << dir1 << " does not exist\n";

	}
	catch (const filesystem_error& ex)
	{
		cout << ex.what() << '\n';
	}
	cout << "Well done!" << endl;

	out.close();
}