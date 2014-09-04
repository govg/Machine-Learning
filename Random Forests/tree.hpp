/*

Author	:	ic070897

Title	:	Tree data type declration

This header file contains the declaration of the tree data type. 

*/

#pragma once

#include "nodes.hpp"
#include "typedef.hpp"


class Tree
{
public:
//	Public functions and variables
	void trainTree(const MatrixReal&, const VectorInteger&);
	VectorReal testTree(const VectorReal&);
	double informationGain(int i, int j, const VectorInteger&);
	bool checkThres(int i);
	Tree(int);

	VectorReal classWts;

	std::vector<bool> indices;
	std::vector<int> nodeix;
	int partition(int start,int end);
	void treeHist();
	bool isTrivial(int id,const VectorInteger& labels);

private:
//	The actual nodes are private members. This could be changed
//	to provide more of a scikit learn functionality, or maybe more
//	public functions, for displaying the nodes.
	std::vector<Node> nodes;
	int depth;
};
