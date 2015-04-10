#ifndef MODBASECONNECTION_H_
#define MODBASECONNECTION_H_

#include <Base/modBaseName.h>
#include <Util/modLogger.h>

#include <map>
#include <sstream>

namespace eLibV2
{
    class BaseConnection : virtual public BaseName // virtual is needed for initialisation in subclasses
    {
    public:
        enum
        {
            CONNECTION_INPUT = 0xff000000,
            CONNECTION_OUTPUT = 0xff100000,
        };

    public:
        BaseConnection(int numInputs = 0, int numOutputs = 0)
            : numberOfInputs(numInputs), numberOfOutputs(numOutputs) {}

        /* process signal flow */
        virtual double processConnection()
        {
            double res = 0.0;

            ModuleLogger::print("%s::processIO (inputs: %i)", getModuleName().c_str(), connections.size());
            for (connectionIterator it = connections.begin(); it != connections.end(); it++)
                res = it->second->processConnection();
            return res;
        }

        /* print signal flow of modules */
        virtual void printConnection(int depth = 0)
        {
            std::stringstream oss;

            depth++;
            for (int i = 1; i < depth; i++)
                oss << ' ';
            oss << getModuleName().c_str() << " " << (int)getNumberOfInputs() << "/" << (int)getNumberOfOutputs();
            ModuleLogger::print(oss.str().c_str());

            if (connections.size() > 0)
            {
                for (connectionIterator it = connections.begin(); it != connections.end(); it++)
                    it->second->printConnection(depth);
            }
        }

        virtual int getNumberOfInputs() { return numberOfInputs; }
        virtual int getNumberOfOutputs() { return numberOfOutputs; }

        // virtual is needed for polymophism
        virtual void connect(const int connectionId, BaseConnection* input) { connections[connectionId] = input; }
        virtual void disconnect(const int connectionId) { connections.erase(connectionId); }
        virtual bool isConnected(const int connectionId) { return(connections.count(connectionId) > 0); }

    protected:
        typedef std::map<const int, BaseConnection*> connectionMap;
        typedef connectionMap::iterator connectionIterator;

        const int numberOfInputs, numberOfOutputs;
        connectionMap connections;
    };
}

#endif
