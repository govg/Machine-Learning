/*

Author	:	ic070897

Title	:	Node data type definition

This is the header file behind the classes. 

*/


#pragma once

#include "typedef.hpp"

#include <vector>
#include <cmath>
#include <iostream>

//First we define the most primary class, the Node.
class Node
{
public:

//	Functions mainly for accessing private variables, this style
//	may be a bit inconsistent.
	bool returnType();
	bool isType();
	void setType(bool);

	Node();
//	Member variables. Most of them are public, just for ease of use
	int x,depth;
	double threshold;
	int start,end;
	VectorReal hist;
	int leftChild,rightChild;

private:
	bool type;
};


