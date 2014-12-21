#ifndef MODTESTCASE_H_
#define MODTESTCASE_H_

namespace eLibV2
{
	class ModuleTestCase
	{
	public:
		virtual bool Test(void) = 0;
	};
}

#endif