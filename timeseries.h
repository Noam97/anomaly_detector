

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;
#include <string.h>
#include <string>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

class TimeSeries{
map<string, vector<float>> myMap;
fstream file;
vector <string> vfeature;
int counter = 0;




public:
	TimeSeries(const char* CSVfileName);
	vector<string> getFeatures() const;
	map<string, vector<float>> getMap() const;
	void addLine(string line);
	float valInTime(string feature, float time) const;
	vector<float> getVector(string s) const;
	int vectorSize() const;


};



#endif /* TIMESERIES_H_ */
