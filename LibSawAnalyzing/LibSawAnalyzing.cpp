#include "LibSawAnalyzing.h"

#include <algorithm> 
#include <iostream>

namespace SawAnalyzeFuncs
{
	SawAnalyzingFunc::AMPLITUDE_ERRORS SawAnalyzingFunc::amplitude_checking(TEST_PARAM &tp, RESULT &res, vector<int> &data)
	{
		int min_trh = 0, max_trh = 0;

		res.ampl_test = AMPL_NORM;

		std::vector<int>::iterator a_max = max_element(data.begin(), data.end());
		std::vector<int>::iterator a_min = min_element(data.begin(), data.end());
		
		if (!(*a_min >= tp.aml_min - tp.err && *a_min <= tp.aml_min + tp.err))
			res.ampl_test = AMPL_TOO_LOW;

		if (!(*a_max >= tp.aml_max - tp.err && *a_max <= tp.aml_max + tp.err))
		{
			if (res.ampl_test == AMPL_TOO_LOW)
				res.ampl_test = AMPL_OUT_BORDER;
			else 
				res.ampl_test = AMPL_TOO_HIGHT;
		}

		return res.ampl_test;
	}

	bool SawAnalyzingFunc::pulse_width_checking(TEST_PARAM &tp, RESULT &res, vector<int> &data)
	{
		res.widht_test = false;
		if (tp.width == data.size())
			res.widht_test = true;
		return res.widht_test;
	}

	bool SawAnalyzingFunc::form_checking(RESULT &res, vector<int> &data)
	{
		res.form_tst = true;
		for (int i = 0; i < data.size() - 1; i++)
		{
			if (data[i] >= data[i + 1])
			{
				res.form_tst = false;
				break;
			}
		}		
		return res.form_tst;
	}

	bool SawAnalyzingFunc::result_check(RESULT &res)
	{
		if (res.ampl_test == AMPL_NORM && res.form_tst && res.widht_test)
			return true;
		return false;
	}

	void SawAnalyzingFunc::result_out(RESULT &res)
	{
		std::cout << "FormTest," << res.form_tst << '\n';
		std::cout << "AmplTest," << res.ampl_test << '\n';
		std::cout << "WidthTest," << res.widht_test << '\n';
		std::cout << '\n';
	}
}