#include <Util/modTestRunner.h>

void TestRunner::addTestCase(ModuleTestCase* mod)
{
	if (mod)
		TestCases.push_back(mod);
}

void TestRunner::runTests()
{
	for (std::vector<ModuleTestCase*>::iterator it = TestCases.begin(); it != TestCases.end(); it++)
		(*it)->Test();
}