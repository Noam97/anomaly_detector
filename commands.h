

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

};

struct commandsFeatures{
    TimeSeries ts_train;
    TimeSeries ts_test;
	vector<AnomalyReport> v_report;
	HybridAnomalyDetector hybrid;
	commandsFeatures(): ts_train(NULL), ts_test(NULL), v_report(vector <AnomalyReport>()), hybrid(HybridAnomalyDetector()){}
 };

class Command{
	protected:
	DefaultIO* dio;
	commandsFeatures* c;
	string description;

	
	public:

	Command(DefaultIO* dio, commandsFeatures* c):dio(dio), c(c){}
	virtual void execute()=0;
	void setDescription(string d) { this->description = d; }
	string getDescription() { return this->description; }

	virtual ~Command(){}
};



class command_1:public Command{
public:
	command_1(DefaultIO* dio,  commandsFeatures* c):Command(dio,c){
	this->description = "1.upload a time series csv file\n";
	}

void execute(){
	ofstream f_train;
	ofstream f_test;
	string read;

	dio->write("Please upload your local train CSV file.\n");
	f_train.open("anomalyTrain.csv");
	read = dio->read();
	while(read != "done"){
		f_train <<  read << endl;
		read = dio->read();
	}
	c->ts_train = TimeSeries("anomalyTrain.csv");

	dio->write("Upload complete.\n");
	f_train.close();

	//create csv file

	dio->write("Please upload your local test CSV file.\n");
	f_test.open("anomalyTest.csv");
	read = dio->read();
	while(read  != "done"){
		f_test << read << endl;
		read = dio->read();
	}
	c->ts_test = TimeSeries("anomalyTest.csv");
	dio->write("Upload complete.\n");

	f_test.close();
	}
};


class command_2:public Command{
	public:
	command_2(DefaultIO* dio,  commandsFeatures* c):Command(dio,c){
	this->description = "2.algorithm settings\n";
	} 

	 void execute(){
		 dio->write("The current correlation threshold is ");
		 float threshold = c->hybrid.getThreshold();
		 dio->write(threshold);
		 dio->write("\n");
		 dio->write("Type a new threshold\n");
		 threshold = stof(dio->read());
		 if (threshold <= 0 || 1 <= threshold)
		 {
			 dio->write("please choose a value beteen 0 and 1.\n");
			 threshold = stof(dio->read());
		}
		 c->hybrid.setTreshold(threshold);
	}
};



class command_3:public Command{
	public:
	command_3(DefaultIO* dio,  commandsFeatures* c):Command(dio,c){
	this->description = "3.detect anomalies\n";
	}
	 void execute(){
		 c->hybrid.learnNormal(c->ts_train);
		 c->v_report = c->hybrid.detect(c->ts_test);
		 dio->write("anomaly detection complete.\n");
		}
};

class command_4:public Command{
	public:
	command_4(DefaultIO* dio,  commandsFeatures* c):Command(dio,c){
	this->description = "4.display results\n";
	}
	void execute(){
		int i;
		int size = c->v_report.size();
		//display results of test
		for (i = 0; i < size;  i++){
			dio->write((c->v_report)[i].timeStep);
			dio->write("	");
			dio->write((c->v_report)[i].description);
			dio->write("\n");
		}
		dio->write("Done.\n");
    }
};


class report{
    long begin, end;
    string desc; //description
 public:
    report(long begin, long end, string desc): begin(begin), end(end), desc(desc){}
    //getters
    long getBegin() { return begin; }
    long getEnd() { return end; }
    string getDescription() { return desc; }
};

class command_5:public Command{
public:
	command_5(DefaultIO* dio,  commandsFeatures* c):Command(dio,c){
	this->description = "5.upload anomalies and analyze results\n";
	}

	void execute(){
		vector <report> r = vector <report>();
        int size = c->v_report.size();
        string preDesc, currDesc;
        long preTime, currTime;
		// Insert the anomalies without duplicates
        if (size != 0)
        {
            preDesc = string(c->v_report[0].description);
            preTime = c->v_report[0].timeStep;
			currDesc = preDesc;
			currTime = preTime;
		}
        for (int i = 1; i <= size; i++)
        {
            if(currDesc.compare(c->v_report[i].description) != 0)
            {
				if(size == i || preTime!=currTime){
                r.push_back(report(preTime, currTime, preDesc));
				}

				preDesc = string(c->v_report[i-1].description);
           		preTime = c->v_report[i-1].timeStep;
				currDesc = preDesc;
				currTime = preTime;
			
			}
			else{
			currTime = c->v_report[i].timeStep;
			currDesc = c->v_report[i].description;
			}

		}
		//test to vector r
		// dio->write("vector r");
		// for (int i = 0; i < r.size(); i++)
		// {
		// 	dio->write("\n");
		// 	dio->write(r[i].getBegin());
		// 	dio->write("	");
		// 	dio->write(r[i].getEnd());
		// 	dio->write("	");
		// 	dio->write( r[i].getDescription());
		// }


		//anomalies report by the user
		vector <report> rUser = vector <report>();
		dio->write("Please upload your local anomalies file.\n");
		string begin, end;
		//anomalies from the user
		string anomaly = dio -> read();
		long position;
		int P = 0; 
		int N; //N , the number of timeSteps that haven't an anomaly, n minus sumAnomalies
		int sumAnomalies = 0; // The amount of the anomalies

		while(anomaly != "done"){
			position = anomaly.find(",");
			begin = anomaly.substr(0, position);
			end = anomaly.substr(position + 1, anomaly.length() - 1);
			rUser.push_back(report(stoi(begin), stoi(end), " "));
			sumAnomalies = sumAnomalies + (stoi(end) - stoi(begin) + 1);
			P++;
			anomaly = dio -> read();
		}

			//N = c->v_report.size() - sumAnomalies;
			N = c->ts_test.vectorSize();
		
			dio->write("Upload complete.\n");
			int rUserSize = rUser.size();
			int rSize = r.size();
			double TP = 0;
			double FP = 0;
			double TPR, FAR;  //true positive rate, false alarm rate
			bool fp = false;

			//TP
			for (int i = 0; i < rUserSize; i++) //anomalies from the user
			{
				for (int j = 0; j < rSize; j++)  //report
				{
					//if the anomaly from the user has been in the scope of the report
					if(!(rUser[i].getEnd() < r[j].getBegin()) && !(r[j].getEnd() < rUser[i].getBegin())) {
					TP++;
					continue;
					}
				}
			}
			//FP
			for (int i = 0; i < rSize; i++){  //report
				fp = false;
				for (int j = 0; j < rUserSize; j++)
				{
					//if report false
					if(!(rUser[i].getEnd() < r[j].getBegin()) && !(r[j].getEnd() < rUser[i].getBegin())) {
						fp = true;
						continue;
					}
					else if((j == rUserSize-1) && (fp==false)) //
						{
							FP++;
						}
				}
			}

			TPR = TP/P;
			FAR = FP / N;

			dio->write("True Positive Rate: ");
			dio->write((floor(TPR * 1000))/1000);
			dio->write("\n");
			dio->write("False Positive Rate: "); 
			dio->write((floor(FAR * 1000))/1000);
			dio->write("\n");
	 }
};

class command_6:public Command{
	public:
	command_6(DefaultIO* dio,  commandsFeatures* c):Command(dio,c){
	this->description = "6.exit\n";
	}

	virtual void execute(){
	}
};



#endif /* COMMANDS_H_ */
