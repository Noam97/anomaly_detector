
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector()
{
	my_threshold = 0.9;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector()
{
	// TODO Auto-generated destructor stub
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts)
{
	string f1, f2;
	Point **p = new Point *[ts.vectorSize()];
	vector<string> vfeature = ts.getFeatures();
	for (int i = 0; i < vfeature.size(); i++)
	{
		float maxDev = 0; //threshold
		float maxPre = 0;
		float pears;
		correlatedFeatures c;
		f1 = vfeature.at(i);
		vector<float> v2;
		vector<float> v1 = ts.getVector(f1);
		vector<float> vec2;
		vector<float> vec1;
		//convertion a vector of floats to float*
		float *vf1 = v1.data();
		for (int j = i + 1; j < vfeature.size(); j++)
		{
			//	f2 = vfeature.at(j);
			v2 = ts.getVector(vfeature.at(j));
			float *vf2 = v2.data();
			pears = fabs(pearson(vf1, vf2, ts.getVector(vfeature.at(j)).size()));
			if (maxPre < pears)
			{
				vec1 = v1;
				vec2 = v2;
				float *vf2 = vec2.data();
				maxPre = pears;
				f1 = vfeature.at(i);
				f2 = vfeature.at(j);
			}
		}
		if (maxPre > 0.5)
		{
			for (int k = 0; k < ts.vectorSize(); k++)
			{
				p[k] = new Point(vec1[k], vec2[k]);
			}

			Line lin_reg = linear_reg(p, ts.vectorSize());

			for (int t = 0; t < ts.vectorSize(); t++)
			{
				float dis = dev(*p[t], lin_reg);
				if (maxDev < dis)
				{
					maxDev = dis;
				}
			}
			c.feature1 = f1;
			c.feature2 = f2;
			c.corrlation = maxPre;
			c.lin_reg = lin_reg;
			c.threshold = (maxDev * 1.1);
			cf.push_back(c);
		}
	}
}
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts)
{
	//vector<AnomalyReport> report;
	int rows = ts.vectorSize();
	int cfs = cf.size();
	for (int i = 0; i < cfs; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			if(cf[i].corrlation >= my_threshold){
				vector<float> vec1 = ts.getVector(cf[i].feature1);
				vector<float> vec2 = ts.getVector(cf[i].feature2);
				Point cor = Point(vec1[j], vec2[j]);
				if (cf[i].threshold < dev(cor, cf[i].lin_reg))
				{
				AnomalyReport ar = AnomalyReport(cf[i].feature1 + "-" + cf[i].feature2, (long)j + 1);
				this->report.push_back(ar);
				}
			}
		}
	}
	return report;
}
