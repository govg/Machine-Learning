/*

Author	:	ic070897

Title	:	Forest data type definition

This is the header file behind the classes. 

*/
#pragma once

#include "tree.hpp"
#include "typedef.hpp"


class Forest
{
public:

	void fit(const MatrixReal&, const VectorInteger&);
	int predict(const VectorReal&);
	double accuracy(const MatrixReal&, const VectorInteger&);
	Forest(int, int);
	void forestHists();

private:
	std::vector<Tree> trees;
	int numTrees;
};

