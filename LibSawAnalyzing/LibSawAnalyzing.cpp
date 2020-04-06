#include "LibSawAnalyzing.h"
#include <algorithm> 

namespace SawAnalyzeFuncs
{
	bool SawAnalyzingFunc::amplitude_checking(int aml_min, int aml_max, int err, vector<int>& data)
	{
		int min_trh = 0, max_trh = 0;

		std::vector<int>::iterator a_max = max_element(data.begin(), data.end());
		std::vector<int>::iterator a_min = min_element(data.begin(), data.end());
		
		if (*a_min <= aml_min - err && *a_min >= aml_min + err)
			return true;
		else 
			return false;

		if (*a_max >= aml_max - err && *a_max <= aml_max + err)
			return true;
		return false;
	}

	bool SawAnalyzingFunc::pulse_width_checking(int w, vector<int>& data)
	{
		if (w == data.size())
			return true;
		return false;
	}

	bool SawAnalyzingFunc::form_checking(vector<int>& data)
	{
		for (int i = 0; i < data.size() - 1; i++)
		{
			if (data[i] >= data[i + 1])
				return false;
		}
		return true;
	}

}