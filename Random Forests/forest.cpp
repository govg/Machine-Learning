
#include "forest.hpp"
#include "typedef.hpp"

#include <iostream>

Forest::Forest(int n = 5, int d = 10)
{
	numTrees = n;
	Tree t(d);
	while(n--)
	{
		trees.push_back(t);
	}
}

double Forest::accuracy(const MatrixReal& featMat, const VectorInteger& labels)
{
	int total, correct;
	
	total = correct = 0;
	
	std::cout<<"Attempting to check with labels\n";

	for(int i = 0; i < featMat.rows(); ++i )
	{

		total++;

		correct += ((labels(i) == predict(featMat.row(i)))?(1):(0));
	}

	return (correct/total) * 100;
}
void Forest::fit(const MatrixReal& featMat, const VectorInteger& labels)
{
	for(int i = 0; i < numTrees; ++i)
	{
		std::cout<<"Training tree no "<<i<<"\n";
		trees[i].trainTree(featMat,labels);
	}
}
void Forest::forestHists()
{
	for(unsigned int i = 0; i < trees.size(); ++i)
	{
		std::cout<<"\nTree number "<<i+1<<"\n";
		trees[i].treeHist();
	}
}
int Forest::predict(const VectorReal& feat)
{
	VectorReal result;
//	result.resize(10);
//	result = VectorReal::Zero(10);
	int probableClass;
	double t;

	result = trees[0].testTree(feat);

//	std::cout<<"Testing forest now\n";

	for(int i = 1; i < numTrees; ++i)
	{
		result += trees[i].testTree(feat);
	}
//	for(int i = 0; i < result.size(); ++i)
//		std::cout<<result(i)<<"\t";

	t =  result.maxCoeff(&probableClass); 

	return probableClass;
}

