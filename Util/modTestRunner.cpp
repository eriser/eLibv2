#include <Util/modTestRunner.h>

using namespace eLibV2;

void TestRunner::addTestCase(TestCase* mod)
{
	if (mod)
		TestCases.push_back(mod);
}

void TestRunner::runTests()
{
	for (std::vector<TestCase*>::iterator it = TestCases.begin(); it != TestCases.end(); it++)
		(*it)->runTests();
}