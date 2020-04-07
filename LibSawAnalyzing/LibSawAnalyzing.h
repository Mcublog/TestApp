#pragma once

using namespace std;

#include <vector>
#include <iostream>

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
            int  bad_sample_num;

            AMPLITUDE_ERRORS ampl_test;
            int aml_min_err;
            int aml_max_err;

            bool widht_test;
            int  width_err;
        }result_t;

        struct TEST_PARAM
        {
            int aml_min;
            int aml_max;
            int err;
            int width;
        }test_parameter;

        static AMPLITUDE_ERRORS amplitude_checking(const TEST_PARAM &tp, RESULT &res, vector<int> &data);
        static bool pulse_width_checking(const TEST_PARAM &tp, RESULT &res, vector<int> &data);
        static bool form_checking(RESULT &res, vector<int> &data);
        static bool check_all_tests(const TEST_PARAM &tp, RESULT &res, vector<int> &data);

        static bool result_check(RESULT &res);

        static void result_out(RESULT &res);
        static void test_param_out(const TEST_PARAM &tp);

        friend std::ostream& operator<<(std::ostream& out, const AMPLITUDE_ERRORS err);
    };
}