#ifndef MODCONTROLIO_H_
#define MODCONTROLIO_H_

#include <Base/modBaseName.h>
#include <Util/modLogger.h>

#include <map>

using namespace eLibV2;

namespace eLibV2
{
	class ControlIO : virtual public BaseName
	{
	public:
		enum
		{
			CONNECTOR_INPUT1,
			CONNECTOR_INPUT2,
			CONNECTOR_INPUT3,
			CONNECTOR_INPUT4,
			CONNECTOR_INPUT5,
			CONNECTOR_INPUT6,
			CONNECTOR_INPUT7,
			CONNECTOR_INPUT8,
			CONNECTOR_OUTPUT1,
			CONNECTOR_OUTPUT2,
			CONNECTOR_OUTPUT3,
			CONNECTOR_OUTPUT4,
			CONNECTOR_OUTPUT5,
			CONNECTOR_OUTPUT6,
			CONNECTOR_OUTPUT7,
			CONNECTOR_OUTPUT8,
		};

	public:
		ControlIO() : BaseName("ControlIO") {}

		// polymophism doesn't work without virtual!!!
		virtual void attachIO(const int connectionId, ControlIO* input) { controlIOs[connectionId] = input; }
		virtual void detachIO(const int connectionId) { controlIOs.erase(connectionId); }
		virtual bool isAttached(const int connectionId) { return(controlIOs.count(connectionId) > 0); }
		virtual double processIOs();
		virtual void testChaining(int depth = 0);

	protected:
		typedef std::map<const int, ControlIO*> controlIOmap;
		typedef controlIOmap::iterator controlIOiterator;

		controlIOmap controlIOs;
	};
}

#endif
