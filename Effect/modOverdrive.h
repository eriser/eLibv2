#ifndef MODOVERDRIVE_H_
#define MODOVERDRIVE_H_

#include <Base/modBaseEffect.h>
#include <Util/modHelper.h>

using namespace eLibV2;

namespace eLibV2
{
	class FxOverdrive : public BaseEffect
	{
	public:
		enum
		{
			CONNECTION_OVERDRIVE_INPUT,
			CONNECTION_OVERDRIVE_LEVEL
		};

		FxOverdrive(std::string name = "FxOverdrive") : BaseName(name) { Init(); }

		void Init() { setLevel(150.0); }
		void Reset() {}
		double Process(double input);
		virtual double processConnection();

	public:
		void setLevel(double Level) {dLevel = Level;}
		double getLevel(void) {return dLevel;}

	public:
		void attachInput(BaseConnection *controller) { connect(CONNECTION_OVERDRIVE_INPUT, controller); }
		void attachLevel(BaseConnection *controller) { connect(CONNECTION_OVERDRIVE_LEVEL, controller); }

	protected:
		double dLevel;
	};
}

#endif
