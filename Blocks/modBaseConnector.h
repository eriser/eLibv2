#ifndef MODBASECONNECTOR_H_
#define MODBASECONNECTOR_H_

#include <Base/modBaseName.h>
#include <Util/modLogger.h>

#include <map>
#include <sstream>

namespace eLibV2
{
	class BaseConnector : virtual public BaseName // virtual is needed for initialisation in subclasses
	{
	public:
		enum
		{
			CONNECTOR_INPUT = 0xff000000,
			CONNECTOR_OUTPUT = 0xff100000,
		};

	public:
		BaseConnector(std::string name = "BaseConnector", int numInputs = 0, int numOutputs = 0)
			: BaseName(name), numberOfInputs(numInputs), numberOfOutputs(numOutputs) {}

		/* process signal chain */
		virtual double processConnection();
		/* print chaining of modules */
		virtual void printConnection(int depth = 0);

		virtual int getNumberOfInputs() { return numberOfInputs; }
		virtual int getNumberOfOutputs() { return numberOfOutputs; }

		// virtual is needed for polymophism
		virtual void connect(const int connectionId, BaseConnector* input) { connections[connectionId] = input; }
		virtual void disconnect(const int connectionId) { connections.erase(connectionId); }
		virtual bool isConnected(const int connectionId) { return(connections.count(connectionId) > 0); }

	protected:
		typedef std::map<const int, BaseConnector*> controlIOmap;
		typedef controlIOmap::iterator controlIOiterator;

		const int numberOfInputs, numberOfOutputs;
		controlIOmap connections;
	};
}

#endif
