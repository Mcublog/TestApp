#include "LibSawAnalyzing.h"

#include <algorithm> 
#include <iostream>
#include <map>

namespace SawAnalyzeFuncs
{
    SawAnalyzingFunc::AMPLITUDE_ERRORS SawAnalyzingFunc::amplitude_checking(const TEST_PARAM &tp, RESULT &res, vector<int> &data)
    {
        int min_trh = 0, max_trh = 0;

        res.ampl_test = AMPL_NORM;

        std::vector<int>::iterator a_max = max_element(data.begin(), data.end());
        std::vector<int>::iterator a_min = min_element(data.begin(), data.end());

        if (!(*a_min >= tp.aml_min - tp.err && *a_min <= tp.aml_min + tp.err))
        {
            res.ampl_test = AMPL_TOO_LOW;
            res.aml_min_err = *a_min;
        }

        if (!(*a_max >= tp.aml_max - tp.err && *a_max <= tp.aml_max + tp.err))
        {
            if (res.ampl_test == AMPL_TOO_LOW)
                res.ampl_test = AMPL_OUT_BORDER;
            else
                res.ampl_test = AMPL_TOO_HIGHT;

            res.aml_max_err = *a_max;
        }

        return res.ampl_test;
    }

    bool SawAnalyzingFunc::pulse_width_checking(const TEST_PARAM &tp, RESULT &res, vector<int> &data)
    {
        res.widht_test = false;
        res.width_err = data.size();
        if (tp.width == res.width_err)
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
                res.bad_sample_num = i;
                res.form_tst = false;
                break;
            }
        }
        return res.form_tst;
    }

    bool SawAnalyzingFunc::check_all_tests(const TEST_PARAM &tp, RESULT &res, vector<int> &data)
    {
        form_checking(res, data);
        pulse_width_checking(tp, res, data);
        amplitude_checking(tp, res, data);

        return result_check(res);
    }

    bool SawAnalyzingFunc::result_check(RESULT &res)
    {
        if (res.ampl_test == AMPL_NORM && res.form_tst && res.widht_test)
            return true;
        return false;
    }

    std::ostream& operator<<(std::ostream& out, const SawAnalyzingFunc::AMPLITUDE_ERRORS err)
    {
        static std::map<SawAnalyzingFunc::AMPLITUDE_ERRORS, std::string> strings;
        if (strings.size() == 0)
        {
#define INSERT_ELEMENT(p) strings[p] = #p
            INSERT_ELEMENT(SawAnalyzingFunc::AMPL_NORM);
            INSERT_ELEMENT(SawAnalyzingFunc::AMPL_TOO_LOW);
            INSERT_ELEMENT(SawAnalyzingFunc::AMPL_TOO_HIGHT);
            INSERT_ELEMENT(SawAnalyzingFunc::AMPL_OUT_BORDER);
#undef INSERT_ELEMENT
        }
        return out << strings[err].c_str();
    }

    void SawAnalyzingFunc::result_out(RESULT &res)
    {
        std::cout << "FormTest," << res.form_tst;
        if (!res.form_tst)
            std::cout << ",BadSample," << res.bad_sample_num;
        std::cout << '\n';

        std::cout << "AmplTest," << res.ampl_test;
        if (res.ampl_test == AMPL_TOO_LOW)
            std::cout << ",A_MIN_CURRENT," << res.aml_min_err;
        else if (res.ampl_test == AMPL_TOO_HIGHT)
            std::cout << ",A_MAX_CURRENT," << res.aml_max_err;
        else if (res.ampl_test == AMPL_OUT_BORDER)
        {
            std::cout << ",A_MIN_CURRENT," << res.aml_min_err;
            std::cout << ",A_MAX_CURRENT," << res.aml_max_err;
        }
        std::cout << '\n';

        std::cout << "WidthTest," << res.widht_test;
        if (!res.widht_test)
            std::cout << ",WIDTH_CURRENT," << res.width_err;
        std::cout << '\n';
    }

    void SawAnalyzingFunc::test_param_out(const TEST_PARAM &tp)
    {
        std::cout << "TestParameters\n";
        std::cout << "AMPLITUDE_MIN," << tp.aml_min << '\n';
        std::cout << "AMPLITUDE_MAX," << tp.aml_max << '\n';
        std::cout << "AMPLITUDE_ERR," << tp.err << '\n';
        std::cout << "PULSE_WIDTH," << tp.width << '\n';
    }
}