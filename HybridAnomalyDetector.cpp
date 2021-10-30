
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector(){
}

void HybridAnomalyDetector::learnNormal(const TimeSeries& ts){
	SimpleAnomalyDetector:: learnNormal(ts);
	int cf_size = cf.size();
	for(int i=0; i < cf_size; i++){
		Point **p = new Point *[ts.vectorSize()];
			if(0.5 < cf[i].corrlation && cf[i].corrlation < this->my_threshold ){
				// create array of points of feature 1 and 2
				for (int j = 0; j < ts.vectorSize(); j++)
				{
					p[j] = new Point(ts.getVector(cf[i].feature1)[j],ts.getVector(cf[i].feature2)[j]);
				}
				Circle c = findMinCircle(p, ts.vectorSize());
				cf.at(i).x_center = c.center.x;
				cf.at(i).y_center = c.center.y;
				cf.at(i).r_center = c.radius * 1.1;
				for (int k=0; k<ts.vectorSize(); k++){
				delete p[k];
				}
				delete[] p;
			}
		}
	}

 vector<AnomalyReport> HybridAnomalyDetector:: detect(const TimeSeries& ts){
	 vector<AnomalyReport> v_report;
	 int row = ts.vectorSize();
	 for(int i = 0; i < cf.size(); i++){
		 Circle c = Circle(Point(cf[i].x_center,cf[i].y_center), cf[i].r_center);
		 for(int j=0; j<row; j++){
			 //the original detect from simple anomaly detector
				vector<float> vec1 = ts.getVector(cf[i].feature1);
				vector<float> vec2 = ts.getVector(cf[i].feature2);
				Point p = Point(vec1[j], vec2[j]);
				if(cf[i].corrlation >= my_threshold){
					if (cf[i].threshold < dev(p, cf[i].lin_reg))
					{
						AnomalyReport ar = AnomalyReport(cf[i].feature1 + "-" + cf[i].feature2, (long)j + 1);
						v_report.push_back(ar);
					}
				}

				if(0.5 < cf[i].corrlation &&  cf[i].corrlation < this->my_threshold ){
					if(!(isInside(c,p))){
						string description = cf[i].feature1 + "-" + cf[i].feature2;
						long timeStep = (long)j + 1;
						AnomalyReport ar = AnomalyReport(description, timeStep);
						v_report.push_back(ar);
				}
			}
		 }
	 }
	 return v_report;
 }

vector<AnomalyReport> HybridAnomalyDetector:: getAnomalyReport(const TimeSeries& ts){
	vector<AnomalyReport> v_report = HybridAnomalyDetector::detect(ts);
	return v_report;
}

void HybridAnomalyDetector:: setTreshold(float threshold){
	this->my_threshold = threshold;
}

float HybridAnomalyDetector:: getThreshold(){
	return this->my_threshold;
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
}
