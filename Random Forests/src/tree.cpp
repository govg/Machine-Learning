/*
This contains the definitions of all the class variables and the functions for
the class of the Tree datatype.
*/
#include "tree.hpp"
#include "typedef.hpp"
#include "splitCriterion.hpp"

#include <queue>
#include <vector>
#include <iostream>
#include <cmath>

#define DATA_MIN 3
#define NUM_CHECKS 5


//	The definitions of the functions used in the Tree class.
Tree::Tree(int d)
{
	Node n;
	depth = d;
	nodes.push_back(n);	
}

//	The crux of the entire library, this contains the part that trains
//	each tree. 

void Tree::trainTree(const MatrixReal& featMat, const VectorInteger& labels)
{

//	We work with a queue of nodes, initially containing only the root node.
//	We process the queue until it becomes empty. 
	std::queue<int> toTrain;

	int size,numClasses,numVars,dims;

	size = labels.size();
	dims = featMat.cols();
	numClasses = labels.maxCoeff();

	
	classWts = VectorReal::Zero(numClasses+1);

	for(int i = 0; i < labels.size(); ++i)
		classWts(labels(i)) += 1.0;

	classWts /= size;
	for(int i = 0; i < size; ++i)
		nodeix.push_back(i);

	std::cout<<"Training tree, dimensions set\n";

	numVars = (int)((double)sqrt((double)dims)) + 1;
	int cur;

//	The relevant indices for the root node is the entire set of training data
	nodes[0].start = 0;
	nodes[0].end = size-1;

//	Initialise the queue with just the root node
	toTrain.push(0);

//	Stores the relevant features.
	VectorReal relFeat;

//	Resize our boolean array, more on this later. 
	indices.resize(size);
	
	std::cout<<"Starting the queue\n";

	int lpoints,rpoints;
//	While the queue isn't empty, continue processing.
	while(!toTrain.empty())
	{
		int featNum;
		double threshold;
		lpoints = rpoints = 0;

		cur = toTrain.front();
		
//		std::cout<<"In queue, node being processed is d :"<<cur.depth<<"\n";
	
//		There are two ways for a node to get out of the queue trivially,
//		a) it doesn't have enough data to be a non-trivial split, or
//		b) it has hit the maximum permissible depth
		
		if((nodes[cur].end - nodes[cur].start < DATA_MIN) || (nodes[cur].depth == depth))
		{
//			Tell ourselves that this is a leaf node, and remove the node
//			from the queue.
//			std::cout<<"Popping a leaf node\n";

			nodes[cur].setType(true);

//			Initialize the histogram and set it to zero
			
			nodes[cur].hist = VectorReal::Zero(numClasses+1);

//			The below code should give the histogram of all the elements
			for(int i = nodes[cur].start; i <= nodes[cur].end; ++i)
			{
				nodes[cur].hist[labels(nodeix[i])] += 1.0;
			}

			for(int i = 0 ; i < classWts.size(); ++i)
				nodes[cur].hist[i] = nodes[cur].hist[i] / classWts[i];

			toTrain.pop();
			continue;

		}	

		double infoGain(-100.0);
		relFeat.resize(size);

//		In case this isn't a trivial node, we need to process it. 
		for(int i = 0; i < numVars; ++i)
		{
//			std::cout<<"Choosing a random variable\n";
//			Randomly select a feature
			featNum = rand()%dims;
//			std::cout<<"Feat: "<<featNum<<std::endl;
//			Extract the relevant feature set from the training data
			relFeat = featMat.col(featNum);

			double tmax,tmin,curInfo;

			tmax = relFeat.maxCoeff();
			tmin = relFeat.minCoeff();
//			infoGain = -100;
			//std::cout<<"Min "<<tmin<<"Max: "<<tmax<<std::endl;

//			NUM_CHECKS is a macro defined at the start
			for(int j = 0; j  < NUM_CHECKS; ++j)
			{
//				std::cout<<"Choosing a random threshold\n";
//				Generate a random threshold
				threshold = ((rand()%100)/100.0)*(tmax - tmin) + tmin;
				//std::cout<<"Thresh: "<<threshold<<std::endl;
				
				for(int k = nodes[cur].start; k <= nodes[cur].end ; ++k)
					indices[k] = (relFeat(k) < threshold);

//				Check if we have enough information gain
				curInfo = informationGain(nodes[cur].start,nodes[cur].end, labels);
//				std::cout<<"Info gain : "<<curInfo<<"\n";
//				curInfo = (double) ((rand()%10)/10.0);

				if(curInfo > infoGain)
				{
					infoGain = curInfo;
					nodes[cur].x = featNum;
					nodes[cur].threshold = threshold;
				}

			}

		}
//		We have selected a feature and a threshold for it that maximises the information gain.

		relFeat = featMat.col(nodes[cur].x);

//		We just set the indices depending on whether the features are greater or lesser.
//		Conventions followed : greater goes to the right.
		for(int k = nodes[cur].start; k <= nodes[cur].end; ++k)
		{
//			If relfeat is lesser, indices[k] will be true, which will put it in the 
//			left side of the partition.
			indices[k] = relFeat(k) < nodes[cur].threshold;
//			indices[k] = (bool)(rand()%2);
			
			if(indices[k])
				lpoints++;
			else
				rpoints++;
		}


		if( (lpoints < DATA_MIN) || (rpoints < DATA_MIN) )
		{
//			Tell ourselves that this is a leaf node, and remove the node
//			from the queue.
//			std::cout<<"Popping a leaf node\n";

			nodes[cur].setType(true);

//			Initialize the histogram and set it to zero
			
			nodes[cur].hist.resize(numClasses+1);
			nodes[cur].hist = VectorReal::Zero(numClasses+1);

//			The below code should give the histogram of all the elements
			for(int i = nodes[cur].start; i <= nodes[cur].end; ++i)
			{
				nodes[cur].hist[labels(nodeix[i])] += 1.0;
			}
			
			toTrain.pop();
			continue;

		}

		int part;

//		Use the prebuilt function to linearly partition our data
		part = partition(nodes[cur].start,nodes[cur].end);

		Node right, left;
//		Increase the depth of the children
		right.depth = left.depth = nodes[cur].depth + 1;

//		Correctly assign the partitions
		left.start = nodes[cur].start;
		left.end = part -1;
		
//		Push back into the relevant places and also link the parent and the child
		nodes.push_back(left);
		nodes[cur].leftChild = nodes.size()-1;
		toTrain.push(nodes[cur].leftChild);

//		Ditto with the right node. 
		right.start = part;
		right.end = nodes[cur].end;

		nodes.push_back(right);
		nodes[cur].rightChild = nodes.size()-1;
		toTrain.push(nodes[cur].rightChild);

//		Finally remove our node from the queue. 
		toTrain.pop();

	}
}

VectorReal Tree::testTree(const VectorReal& feat)
{
	int cur;
	cur = 0;

	while(!nodes[cur].isType())
	{
//		std::cout<<"Right now at node no : "<<cur<<"\n";
//		std::cout<<"Node variables : x,t "<<nodes[cur].x<<"\t"<<nodes[cur].threshold<<"\n";

//		std::cout<<"The feature value is "<<feat(nodes[cur].x)<<"\n";

		if(nodes[cur].threshold < feat(nodes[cur].x))
			cur = nodes[cur].rightChild;
		else
			cur = nodes[cur].leftChild;	
	}
//	std::cout<<"Right now at node no : "<<cur<<"\n";
//	for(int i = 0; i < nodes[cur].hist.size(); ++i)
//		std::cout<<nodes[cur].hist(i)<<"\t";

	return nodes[cur].hist;
}

double Tree::informationGain(int i, int j, const VectorInteger& labels)
{
	
	VectorInteger l,r,t;
	int ctr(0);

	for(int k = i; k <= j; ++k)
		ctr += ((indices[k])?1:0);

	l.resize(ctr);
	r.resize(j+1-i-ctr);
	t.resize(j-i+1);

	l = VectorInteger::Zero(ctr);
	r = VectorInteger::Zero(j+1-i-ctr);
	t = VectorInteger::Zero(j-i+1);

	for(int k = i, m=0, n=0; k <= j; ++k)
	{
		if(indices[k])
			l(m++) = labels[k];
		else
			r(n++) = labels[k];
		
		t(k-i) = labels[k];
	}
	
	double h,hl,hr;

	h = entr(t);
	hl = entr(l);
	hr = entr(r);

//	std::cout<<"\n"<<h<<"\t"<<hl<<"\t"<<hr<<"\n";

	return h - ((l.size()*hl)+(r.size()*hr))/(double)labels.size();
}


int Tree::partition(int l, int r)
{

	int temp;
	bool temp2;

	while ( l <= r)
	{
		if(!indices[l])
		{
			temp = nodeix[l];
			nodeix[l] = nodeix[r];
			nodeix[r] = temp;

			temp2 = indices[l];
			indices[l] = indices[r];
			indices[r] = temp2;

			r--;
		}
		else
			l++;
	}

	return r+1;

/*	
	while(1)
	{
		while(r && !indices[--r]);
		while(indices[++k]);
		if(k<r)
		{
			temp = nodeix[r];
			nodeix[r] = nodeix[k];
			nodeix[k] = temp;

			temp2 = indices[r];
			indices[r] = indices[k];
			indices[k] = temp2;
			k--;
		}
		else break;
	}

	if(r == 0) 
		return 0;

	return k;

*/
}

void Tree::treeHist()
{
	for(unsigned int i = 0; i < nodes.size(); ++i)
	{
		if(nodes[i].isType())
		{
			std::cout<<"\nNode number "<<i<<"\n";			
			for(signed int j = 0; j < nodes[i].hist.size(); ++j)
				std::cout<<nodes[i].hist(j)<<"\t";

		}
		else
		{
			std::cout<<"\nNode number "<<i<<"\n";
			std::cout<<"X : "<<nodes[i].x<<" thres : "<<nodes[i].threshold<<"\n";
		}
	}

}

bool Tree::isTrivial(int i, const VectorInteger& labels)
{
//This part checks for triviality of the node
//The three ways a node can be trivial are  
//a) it has too little elements,
//b) it has the same class elements as before
//c) it has hit the max depth
  if(nodes[i].rightChild - nodes[i].leftChild < DATA_MIN)
    nodes[i].setType(true);

  if(nodes[i].depth == depth)
    nodes[i].setType(true);

  VectorReal hist;

  hist = VectorReal::Zero(labels.maxCoeff()+1);

  for(int k = nodes[i].start; k <= nodes[i].end; ++k)
    hist(labels(nodeix[k])) += 1.0;

  hist = hist/hist.sum();

  if(hist.maxCoeff()>.99)
    nodes[i].setType(true);


  return nodes[i].isType();

}

