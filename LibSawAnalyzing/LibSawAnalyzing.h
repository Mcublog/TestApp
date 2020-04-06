#pragma once

using namespace std;

#include <vector>

namespace SawAnalyzeFuncs
{
	class SawAnalyzingFunc
	{
	public:
		static bool amplitude_checking(int aml_min, int aml_max, int err, vector<int>& data);
		static bool pulse_width_checking(int w, vector<int>& data);
		static bool form_checking(vector<int>& data);
	};
}