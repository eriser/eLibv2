#ifndef MODTESTRUNNER_H_
#define MODTESTRUNNER_H_

#include <Util/modTestCase.h>

#include <vector>

using namespace eLibV2;

namespace eLibV2
{
	class TestRunner
	{
	public:
		void addTestCase(ModuleTestCase* mod);
		void runTests();

	private:
		std::vector<ModuleTestCase*> TestCases;
	};
}

#endif