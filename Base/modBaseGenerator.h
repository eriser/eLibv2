#ifndef MODBASEGENERATOR_H_
#define MODBASEGENERATOR_H_

#include <Base/modBaseModule.h>

#include <string>

namespace eLibV2
{
	class BaseGenerator : public BaseModule
	{
	public:
		enum { kNumNotes = 128 };

	public:
		BaseGenerator(std::string name = "BaseGenerator") : BaseName(name) {}

		virtual void Init(void) = 0;
		virtual void Reset(void) = 0;
		virtual double Process(VstInt16 Note) = 0;
	};
}

#endif
