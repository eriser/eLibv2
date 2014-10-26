/*
* modTestRunner.h
*
*  Created on: 24.11.2011
*      Author: dedokter
*/

#ifndef MODTESTRUNNER_H_
#define MODTESTRUNNER_H_

#include <Base/modTestCase.h>

// std headers
#include <vector>

namespace eLibV2
{
	class TestRunner
	{
	public:
		void addTestCase(TestCase* mod);
		void runTests();

	private:
		std::vector<TestCase*> TestCases;
	};
}

#endif