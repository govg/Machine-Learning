
/*
Author	:	ic070897

Title	:	Dataset data type 

Holds the data type for the dataset that we shall use. This is simply to provide an interface
similar to scikit-learn's dataset.

*/


#pragma once

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>


#include <fstream>
#include <vector>
#include <cassert>

class Data
{
public:

	MatrixReal dataTrain(){return trainData;}
	MatrixReal dataTest(){return testData;}

	VectorInteger labelTrain(){return trainLabel;}
	VectorInteger labelTest(){return testLabel;}

	void readTrain();
	void readTest();

	VectorReal retFeatNum(int featNum);


private:
	std::fstream dataFile;
	std::fstream labelFile;

	MatrixReal trainData;
	MatrixReal testData;
	VectorInteger trainLabel, testLabel;

};
//	Following functions shall read the data and store them in the private
//	variables of the Data class.
void Data::readTrain()
{
	dataFile.open("D:\\randomForestCpp\\rewrite\\src10data.txt");
	labelFile.open("D:\\randomForestCpp\\rewrite\\src10label.txt");

	assert(!dataFile.fail());
	assert(!labelFile.fail());

	std::vector<std::vector<double> > filedata;
	std::vector<int> filelabels;

	double t;
	int ctr(0);
	std::vector<double> temp;

	std::cout<<"Attempting to read data\n";

	while(!dataFile.eof())
	{
		dataFile>>t;
		temp.push_back(t);

		if(dataFile.get() == '\n')
		{
			filedata.push_back(temp);
			temp.clear();
			ctr++;
//			Just a counter to keep track of how much data has been read.
			if(ctr%100 == 0)
				std::cout<<ctr<<"...";
		}

		
	}
	std::cout<<"Attempting to read labels\n";

	while(!labelFile.eof())
	{
		labelFile>>t;
		filelabels.push_back((int)t);

		if(ctr%100 == 0)
			std::cout<<ctr--<<"...";
	}

	std::cout<<"Attempting to store data\n";

	trainData.resize(filedata.size(),filedata[0].size());
	trainLabel.resize(filelabels.size());

	for(unsigned int i = 0; i < filedata.size(); ++i)
	{
		for(unsigned int j = 0; j < filedata[0].size(); ++j)
			trainData(i,j) = filedata[i][j];
	}

	std::cout<<"Attempting to store labels\n";
	
	
	for(unsigned int i = 0; i < filelabels.size(); ++i)
	{
		trainLabel(i) = filelabels[i];
		
	}
	std::cout<<"Stored everything, now to process\n";

	dataFile.close();
	labelFile.close();

}
void Data::readTest()
{

	std::fstream dataFile("D:\\randomForestCpp\\rewrite\\src10testdata.txt");
	std::fstream labelFile("D:\\randomForestCpp\\rewrite\\src10testlabel.txt");


	assert(!dataFile.fail());
	assert(!labelFile.fail());

	std::vector<std::vector<double> > filedata;
	std::vector<int> filelabels;

	double t;
	int ctr(0);
	std::vector<double> temp;

	std::cout<<"Attempting to read data\n";

	while(!dataFile.eof())
	{
		dataFile>>t;
		temp.push_back(t);

		if(dataFile.get() == '\n')
		{
			filedata.push_back(temp);
			temp.clear();
			ctr++;
			
			if(ctr%700 == 0)
				std::cout<<ctr<<"...";
		}		
	}

	std::cout<<"Attempting to read labels\n";

	while(!labelFile.eof())
	{
		labelFile>>t;
		filelabels.push_back((int)t);

		if(ctr%700 == 0)
			std::cout<<ctr--<<"...";
	}

	std::cout<<"Attempting to store data\n";

	testData.resize(filedata.size(),filedata[0].size());
	testLabel.resize(filelabels.size());

	for(unsigned int i = 0; i < filedata.size(); ++i)
	{
		for(unsigned int j = 0; j < filedata[0].size(); ++j)
			testData(i,j) = filedata[i][j];
	}

	std::cout<<"Attempting to store labels\n";
	
	
	for(unsigned int i = 0; i < filelabels.size(); ++i)
	{
		testLabel(i) = filelabels[i];
		
	}
	std::cout<<"Stored everything, now to process\n";
	
	dataFile.close();
	labelFile.close();
}
