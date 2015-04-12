#ifndef MODBASECONNECTION_H_
#define MODBASECONNECTION_H_

#include <Base/modBaseName.h>
#include <Util/modLogger.h>

#include <map>
#include <sstream>

using namespace eLibV2::Util;

namespace eLibV2
{
    namespace Base
    {
        /**
        this module provides connections between individual modules
        */
        class BaseConnection : virtual public BaseName // virtual is needed for initialisation in subclasses
        {
        public:
            enum ConnectionType
            {
                CONNECTION_INPUT = 0x00010000,
                CONNECTION_OUTPUT = 0x00020000,
            };

        public:
            BaseConnection(int numInputs = 0, int numOutputs = 0)
                : numberOfInputs(numInputs), numberOfOutputs(numOutputs) {}

            /**
            process signal flow for all connected module
            */
            virtual double processConnection()
            {
                double res = 0.0;

                ModuleLogger::print("%s::processIO (inputs: %i)", getModuleName().c_str(), connections.size());
                for (connectionIterator it = connections.begin(); it != connections.end(); it++)
                    res = it->second->processConnection();
                return res;
            }

            /**
            print signal flow of modules
            @param depth identation depth for output
            */
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

            /**
            get number of available inputs for module
            @return number of inputs
            */
            virtual int getNumberOfInputs() { return numberOfInputs; }

            /**
            get number of available outputs for module
            @return number of outputs
            */
            virtual int getNumberOfOutputs() { return numberOfOutputs; }

            /**
            create connection between modules
            @param connectionId id for the created connection
            @param input the connection pin of the other module
            */
            virtual void connect(const int connectionId, BaseConnection* input) { connections[connectionId] = input; }

            /**
            delete connection between modules
            @param connectionId id of the connection to be deleted
            @return true if connection was present and deleted, false if not
            */
            virtual bool disconnect(const int connectionId) { return (connections.erase(connectionId) > 1); }

            /**
            test if connection with given id is present
            @param connectionId id of the connection to be tested
            @return true if connection is present, false if not
            */
            virtual bool isConnected(const int connectionId) { return(connections.count(connectionId) > 0); }

        protected:
            typedef std::map<const int, BaseConnection*> connectionMap; ///< typedef for internal connection-map
            typedef connectionMap::iterator connectionIterator; ///< typedef for iterator of internal connection-map

            const int numberOfInputs; ///< number of inputs available
            const int numberOfOutputs; ///< number of outputs available
            connectionMap connections; ///< map with all currently set up connections
        };
    }
}

#endif
