/*

Author	:	ic070897

Title	:	Splitting Criterion, including the histogram calculations

This file is remarkably shorter because of the shift of the informationGain function to the
Tree class. 

Warning :	Hardcoded value of num of classes, line 31,32
*/



#pragma once

#include "typedef.hpp"

#include <vector>
#include <map>
#include <cmath>

double entr(VectorInteger y)
{

	VectorReal histogram;
	VectorInteger histInt;

//	histogram.resize((int)y.maxCoeff()+1);
//	histogram = VectorReal::Zero((int)y.maxCoeff()+1);
	
	histogram.resize(10);
	histogram = VectorReal::Zero(histogram.size());
	histInt = VectorInteger::Zero(histogram.size());

	for(int i = 0; i < y.size(); ++i)
	{
		histInt(y(i)) += 1;
	}

	for(int i = 0; i < histogram.size(); ++i)
		histInt(i) += 1;

	
	double sum(0.0);

	sum = histInt.sum();
	
	double temp;

	

	for(int i = 0; i < histogram.size(); ++i)
	{
		
//		std::cout<<"REDUC\n";

		temp = log((double)histInt(i)/sum)*((double)histInt(i)/sum);
		histogram(i) = temp;
//		histogram(i) = temp/ (double)log(2.0);
	}


	return -histogram.sum();

}


