#include "timeseries.h"
//ifstream file;

TimeSeries::TimeSeries(const char *CSVfileName)
{
    string line;
    string sKey;
    string sVal;

    int position = 0;

    file.open(CSVfileName);

    getline(file, line);
    //While there is no more"," and the line is not empty
    while (line.find(",") != string::npos)
    {
        position = line.find(",");
        sKey = line.substr(0, position);
        vfeature.push_back(sKey);
        myMap.insert(std::pair<string, vector<float>>(sKey, vector<float>()));
        line.erase(0, position + 1);
    }
    //The last feature
    sKey = line;
    myMap.insert(std::pair<string, vector<float>>(sKey, vector<float>()));
    vfeature.push_back(sKey);

    //The other lines.
    while (getline(file, line))
    {

        for (std::map<string, vector<float>>::iterator it = myMap.begin(); it != myMap.end(); ++it)
        {
            position = line.find(",");
            sVal = line.substr(0, position);
            line.erase(0, position +  1);
            float s = stof(sVal);
            it->second.push_back(s);
            counter++;
        }
    }
}

vector<string> TimeSeries::getFeatures() const
{
    return vfeature;
}


void TimeSeries::addLine(string line)
{
    int position = 0;
    for (std::map<string, vector<float>>::iterator it = myMap.begin(); it != myMap.end(); ++it)
    {
        position = line.find(",");
        string sVal = line.substr(0, position);
        line.erase(0, position + 1);
        it->second.push_back(stof(sVal));
    }
}

map<string, vector<float>> TimeSeries::getMap() const
{
    return myMap;
}

int TimeSeries::vectorSize() const
{
    int numberOfRows = counter/(vfeature.size());
    return numberOfRows;
}

vector<float> TimeSeries::getVector(string s) const
{
    std::map<string, vector<float>>::const_iterator it = myMap.find(s);
    return it->second;
}
