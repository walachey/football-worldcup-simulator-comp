#include "stdafx.h"
#include "json.h"

#include "Simulation.h"
#include "ResultSummary.h"

#ifdef __GNUC__
#include <execinfo.h>
#include <signal.h>

void handler(int sig)
{
	void *array[20];
	size_t size;
	size = backtrace(array, 20);
	fprintf(stderr, "Error: signal %d:\n", sig);
	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(-11);
}

#endif

void debug_terminate()
{
	std::cerr << "Uncaught exception." << std::endl;
	assert(false);
}

int main(int argc, char* argv[])
{

	std::set_terminate(debug_terminate);
	
#ifdef __GNUC__
	signal(SIGSEGV, handler);
#endif

	// options can either be passed as a filename in the command line or as input to stdin
	std::string inputFilename = "";
	for (int i = 1; i < argc; ++i)
	{
		std::string option = argv[i];

		if (option == "-f" && (i < argc - 1))
		{
			inputFilename = argv[i + 1];
		}
	}

	json_spirit::Value jsonData;
	bool jsonResult = false;

	if (!inputFilename.empty())
	{
		std::ifstream input(inputFilename, std::ifstream::in);
		jsonResult = json_spirit::read((std::istream&)input, jsonData);
	}
	else
	{
		jsonResult = json_spirit::read((std::istream&)std::cin, jsonData);
	}

	if (!jsonResult)
	{
		exit(1);
	}


#ifdef NDEBUG
	try
#endif
	{

		sim::Simulation simulation(jsonData);
		simulation.execute();

		/*
		// Originally, the simulator outputs sophisticated results in JSON format to stdout.
		// Those results are then used by the simulation backend to generate statistics, which can be displayed to the user.

		json_spirit::Object results = simulation.getJSONResults();
		std::string json = json_spirit::write(results, json_spirit::remove_trailing_zeros);
		std::cout << json << std::endl;
		*/

		// for the contest, print a simple summary only
		sim::ResultSummary summary(&simulation);
		summary.printResults();

	}
#ifdef NDEBUG
	catch(const std::runtime_error& re)
	{
		std::cerr << "Runtime error: " << re.what() << std::endl;
		assert (false);
		exit(1);
	}
	catch(const std::exception& ex)
	{
		std::cerr << "Error occurred: " << ex.what() << std::endl;
		assert (false);
		exit(1);
	}
	catch (const char *s)
	{
		std::cerr << s << std::endl;
		assert(false);
		exit(1);
	}
	catch (std::string &s)
	{
		std::cerr << s << std::endl;
		assert(false);
		exit(1);
	}
	catch(...)
	{
		std::cerr << "An unexpected error occurred." << std::endl;
		assert (false);
		exit(1);
	}
#endif
	

	return 0;
}

