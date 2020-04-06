#include <iostream>
#include <algorithm> 
#include <vector>
#include <fstream>
#include <string>

#include "LibSawAnalyzing.h"


int main()
{
	string STRING;
	ifstream myfile;
	std::vector<int> v;
	std::string::size_type sz;

	myfile.open("TestData/0.txt");

	if (myfile.is_open())
	{
		std::cout << "File is open" << std::endl;
		while (!myfile.eof())
		{
			getline(myfile, STRING);
			cout << STRING << endl;
			v.push_back(std::stoi(STRING, &sz));
		}
	}
	myfile.close();

	double a = 9.9;
	double b = 7.7;

	

	std::vector<int>::iterator result = std::max_element(v.begin(), v.end());

	std::cout << "Form checking... " << SawAnalyzeFuncs::SawAnalyzingFunc::form_checking(v) << std::endl;

}

