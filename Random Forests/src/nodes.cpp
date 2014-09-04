/*
Author	:	ic070897

Title	:	node data type

The data types and functions governing the random forest library

Set DATA_MIN to the required number of minimum elements at each split node
for further splitting. 

Set NUM_CHECKS as the number of random variables to be generated

*/

#pragma once



#include "nodes.hpp"

#include <vector>
#include <queue>
#include <iostream>

//	The definitions of the functions used in the Node class
Node::Node()
{
	type = false;
	start = end = 0;
	leftChild = rightChild = 0;
	depth = 1;
	hist.resize(10);
}

bool Node::isType()
{
	return type;
}

void Node::setType(bool isLeaf)
{
	type = isLeaf;
}

