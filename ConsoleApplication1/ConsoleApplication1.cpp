#include <iostream>
#include <algorithm> 
#include <vector>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <map>
#include <mutex>
#include <cstring>


#include <experimental/filesystem>
//#include <filesystem>

#include "LibSawAnalyzing.h"

namespace fs = std::experimental::filesystem;
namespace sa = SawAnalyzeFuncs;

std::mutex m_ostream;
std::mutex m_config;

bool err_only = true;
const int DEFAULT_AMPL_MIN = 0;
const int DEFAULT_AMPL_MAX = 480;
const int DEFAULT_AMPL_ERR = 20;
const int DEFAULT_PULSE_WIDTH = 50;

struct sa::SawAnalyzingFunc::TEST_PARAM tp =
{
    DEFAULT_AMPL_MIN,
    DEFAULT_AMPL_MAX,
    DEFAULT_AMPL_ERR,
    DEFAULT_PULSE_WIDTH
};

void prepare(const string& tested, const string& result)
{
    fs::path p = fs::current_path();
    fs::remove_all(tested);
    fs::remove_all(result);

    fs::create_directory(tested);
    fs::create_directory(result);
}

void check_data(const fs::path &file_path, const string &outdir)
{
    string val;
    fstream f;
    bool out = false;

    std::vector<int> v;

    f.open(file_path, ios::in);

    if (f.is_open())
    {
        while (!f.eof())
        {
            getline(f, val);
            if (!val.empty())
            {
                v.push_back(std::stoi(val, nullptr));
            }
        }
        f.close();
    }


    fs::path fname = fs::path(file_path).filename();

    struct sa::SawAnalyzingFunc::RESULT res = { 0 };

    sa::SawAnalyzingFunc::check_all_tests(tp, res, v);

    std::lock_guard<std::mutex> guard(m_ostream);
    if (err_only)
    {
        if (!sa::SawAnalyzingFunc::result_check(res))
            out = true;
    }
    else out = true;

    if (out)
    {
    std:cout << "TestFile," << fname << '\n';
        sa::SawAnalyzingFunc::result_out(res);
    }

}

void clean_cin(void)
{
    std::cin.clear();
    std::cin.ignore(INT_MAX, '\n');
}

void param_parcer(void)
{
    string cmd = "";

    while (1)
    {
        std::cin >> cmd;
        if (cmd == "cfg")
        {
            m_config.lock();
            std::cout << "----Enter config param---\n";
            try
            {
                std::cout << "err_only:";
                std::cin >> err_only;
                if (!cin.good())
                {
                    clean_cin();
                    std::cout << "Uncorrect value, set default: TRUE\n";
                    err_only = true;
                }


                std::cout << "amplitude_min:";
                std::cin >> tp.aml_min;
                if (!cin.good())
                {
                    clean_cin();
                    std::cout << "Uncorrect value, set default: " << DEFAULT_AMPL_MIN << '\n';
                    tp.aml_min = DEFAULT_AMPL_MIN;
                }

                std::cout << "amplitude_max:";
                std::cin >> tp.aml_max;
                if (!cin.good())
                {
                    clean_cin();
                    std::cout << "Uncorrect value, set default: " << DEFAULT_AMPL_MAX << '\n';
                    tp.aml_max = DEFAULT_AMPL_MAX;
                }

                std::cout << "amplitude error:";
                std::cin >> tp.err;
                if (!cin.good())
                {
                    clean_cin();
                    std::cout << "Uncorrect value, set default: " << DEFAULT_AMPL_ERR << '\n';
                    tp.err = DEFAULT_AMPL_ERR;
                }

                std::cout << "pulse width:";
                std::cin >> tp.width;
                if (!cin.good())
                {
                    clean_cin();
                    std::cout << "Uncorrect value, set default: " << DEFAULT_PULSE_WIDTH << '\n';
                    tp.width = DEFAULT_PULSE_WIDTH;
                }
            }
            catch (...)
            {
                std::cout << "Enter correct param\n";
            }
            sa::SawAnalyzingFunc::test_param_out(tp);
            std::cout << "---Config accept---\n";
            m_config.unlock();
        }
    }
}

int main(int argc, char ** argv)
{
    for (int i = 0; i < argc; i++)
    {
        string arg(argv[i]);
        try
        {
            if (arg == "-amin")
            {
                string p(argv[i + 1]);
                int i = std::stoi(p, nullptr);
                tp.aml_min = i;
            }
            if (arg == "-amax")
            {
                string p(argv[i + 1]);
                int i = std::stoi(p, nullptr);
                tp.aml_max = i;
            }
            if (arg == "-aerr")
            {
                string p(argv[i + 1]);
                int i = std::stoi(p, nullptr);
                tp.err = i;
            }
            if (arg == "-w")
            {
                string p(argv[i + 1]);
                int i = std::stoi(p, nullptr);
                tp.width = i;
            }
        }
        catch (const std::invalid_argument& ia) 
        {
            std::cerr << "Invalid argument: " << ia.what() << '\n';
        }
    }

    const string WORK_DIR_NAME = "TestData";
    const string TESTED_DIR_NAME = "Tested";
    const string RESULT_DIR_NAME = "Result";

    bool working = false;

    std::cout << "Welcome to SawAnalyser v0.1 " << '\n';
    sa::SawAnalyzingFunc::test_param_out(tp);

    prepare(TESTED_DIR_NAME, RESULT_DIR_NAME);

    std::thread(param_parcer).detach();

    while (1)
    {
        for (auto& p : fs::directory_iterator(WORK_DIR_NAME))
        {
            m_config.lock();
            if (!working)
            {
                std::cout << "Start working... " << "\n\n";
                working = true;
            }

            if (!fs::is_directory(p))
            {
                auto to = TESTED_DIR_NAME / fs::path(p).filename();
                fs::copy_file(p, to, fs::copy_options::overwrite_existing);
                fs::remove(p);

                std::thread(check_data, to, RESULT_DIR_NAME).detach();
                //std::this_thread::sleep_for(1s);
            }
            m_config.unlock();
        }
        std::this_thread::sleep_for(1s);
        if (working)
        {
            std::cout << "Work done\n";
            working = false;
        }
    }

}

