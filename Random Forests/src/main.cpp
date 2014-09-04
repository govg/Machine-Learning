
/*

Author	:	ic070897

Title	:	Driver Program

*/

#include "forest.hpp"
#include "data.hpp"
#include "typedef.hpp"


#include <iostream>
#include <fstream>
#include <ctime>

int main()
{

	Forest a(25,4);

	Data x;

	time_t start,end;

	x.readTrain();
	x.readTest();
	start = time(NULL);
	a.fit(x.dataTrain(),x.labelTrain());
	end = time(NULL);
	std::cout<<"Training done\n";
	std::cout<<"Time taken : "<< (double)(end-start)<<"\n";


	MatrixReal b;
	VectorInteger c;

	b = x.dataTest();
	c = x.labelTest();
	int total(0),correct(0);
	
//	a.forestHists();

	for(int i = 0; i < b.rows(); ++i)
	{
		total++;
		if(a.predict(b.row(i))== c(i))
			correct++;
	}



	std::cout<<"Total number of samples : "<<total<<"\n";
	std::cout<<"Correctly labelled : "<<correct<<"\n";

	std::cin>>correct;

	return 0;

}

