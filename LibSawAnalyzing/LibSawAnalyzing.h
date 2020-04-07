#pragma once

using namespace std;

#include <vector>

namespace SawAnalyzeFuncs
{
	class SawAnalyzingFunc
	{
	public:
		enum AMPLITUDE_ERRORS
		{
			AMPL_NORM,
			AMPL_TOO_LOW,
			AMPL_TOO_HIGHT,
			AMPL_OUT_BORDER
		};

		struct RESULT
		{
			bool form_tst;
			AMPLITUDE_ERRORS ampl_test;
			bool widht_test;
		}result_t;

		struct TEST_PARAM
		{
			int aml_min;
			int aml_max;
			int err;
			int width;
		}test_parameter;

		static AMPLITUDE_ERRORS amplitude_checking(TEST_PARAM &tp, RESULT &res, vector<int> &data);
		static bool pulse_width_checking(TEST_PARAM &tp, RESULT &res, vector<int> &data);
		static bool form_checking(RESULT &res, vector<int> &data);
		static bool result_check(RESULT &res);
		static void result_out(RESULT &res);

	};
}