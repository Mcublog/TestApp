#include <iostream>
#include <algorithm> 
#include <vector>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <map>
#include <mutex>

#include <experimental/filesystem>
//#include <filesystem>

#include "LibSawAnalyzing.h"

namespace fs = std::experimental::filesystem;
namespace sa = SawAnalyzeFuncs;

std::mutex m_ostream;
const bool err_only = true;

std::ostream& operator<<(std::ostream& out, const sa::SawAnalyzingFunc::AMPLITUDE_ERRORS value)
{
	static std::map<sa::SawAnalyzingFunc::AMPLITUDE_ERRORS, std::string> strings;
	if (strings.size() == 0)
	{
#define INSERT_ELEMENT(p) strings[p] = #p
		INSERT_ELEMENT(sa::SawAnalyzingFunc::AMPL_NORM);
		INSERT_ELEMENT(sa::SawAnalyzingFunc::AMPL_TOO_LOW);
		INSERT_ELEMENT(sa::SawAnalyzingFunc::AMPL_TOO_HIGHT);
		INSERT_ELEMENT(sa::SawAnalyzingFunc::AMPL_OUT_BORDER);
#undef INSERT_ELEMENT
	}
	return out << strings[value];
}

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

	struct sa::SawAnalyzingFunc::TEST_PARAM tp =
	{
		0,  //.aml_min 
		480, //.aml_max
		20,  //.err
		50, // .widtр
	};

	struct sa::SawAnalyzingFunc::RESULT res = { 0 };

	sa::SawAnalyzingFunc::form_checking(res, v) << '\n';
	sa::SawAnalyzingFunc::amplitude_checking(tp, res, v) << '\n';
	sa::SawAnalyzingFunc::pulse_width_checking(tp, res, v) << '\n';

	std::lock_guard<std::mutex> guard(m_ostream);
	if (err_only)
	{
		if (!check_data(res))
			sa::SawAnalyzingFunc::result_out(res);
	}
	else sa::SawAnalyzingFunc::result_out(res);

}

int main()
{
	const string WORK_DIR_NAME	 = "TestData";
	const string TESTED_DIR_NAME = "Tested";
	const string RESULT_DIR_NAME = "Result";
		

	prepare(TESTED_DIR_NAME, RESULT_DIR_NAME);

	while (1)
	{
		std::cout << "Working... " << '\n';

		for (auto& p : fs::directory_iterator(WORK_DIR_NAME))
		{
			if (!fs::is_directory(p))
			{
				auto to = TESTED_DIR_NAME / fs::path(p).filename();
				fs::copy_file(p, to, fs::copy_options::overwrite_existing);
				fs::remove(p);

				std::thread(check_data, to, RESULT_DIR_NAME).detach();
			}

		}

		std::this_thread::sleep_for(1s);
	}

}

