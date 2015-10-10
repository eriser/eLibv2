#ifndef MODBASECONNECTION_H_
#define MODBASECONNECTION_H_

#include <Base/BaseName.h>
#include <Util/Logger.h>
#include <Util/Defines.h>

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
            BaseConnection(int numInputs = 1, int numOutputs = 1) :
                numberOfInputs(numInputs),
                numberOfOutputs(numOutputs)
            {
                inputConnections = new BaseConnection*[numberOfInputs];
                memset(inputConnections, 0, sizeof(inputConnections) * numberOfInputs);
                outputConnections = new BaseConnection*[numberOfOutputs];
                memset(outputConnections, 0, sizeof(outputConnections) * numberOfOutputs);
            }

            virtual ~BaseConnection()
            {
                if (inputConnections != NULL)
                    delete[] inputConnections;
                if (outputConnections != NULL)
                    delete[] outputConnections;
            }

            /**
            process signal flow for all connected module
            */
            virtual double processConnection() = 0;

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
                oss << getModuleName().c_str() << " (" << (int)getNumberOfInputs() << "/" << (int)getNumberOfOutputs() << ")";
                ModuleLogger::print(LOG_CLASS_CONNECTION, oss.str().c_str());

                if (inputConnections != NULL)
                {
                    for (unsigned char inputIndex = 0; inputIndex < numberOfInputs; ++inputIndex)
                    {
                        if (isInputConnected(inputIndex))
                            inputConnections[inputIndex]->printConnection(depth);
                    }
                }
            }

        protected:
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
            check if input with given index is connected
            outputs an error message if something is wrong
            @param connectionIndex index of input
            @return true if connected
            */
            virtual bool isInputConnected(unsigned int connectionIndex)
            {
                bool bRes = false;
                if (connectionIndex < getNumberOfInputs())
                {
                    if (inputConnections[connectionIndex])
                        bRes = true;
                }
                else
                    ModuleLogger::print(0, "Warning: Number of Inputs not correctly set!");
                return bRes;
            }

            /**
            check if output with given index is connected
            outputs an error message if something is wrong
            @param connectionIndex index of output
            @return true if connected
            */
            virtual bool isOutputConnected(unsigned int connectionIndex)
            {
                bool bRes = false;
                if (connectionIndex < getNumberOfOutputs())
                {
                    if (outputConnections[connectionIndex])
                        bRes = true;
                }
                else
                    ModuleLogger::print(0, "Warning: Number of Outputs not correctly set!");
                return bRes;
            }

        protected:
            const int numberOfInputs; ///< number of inputs available
            const int numberOfOutputs; ///< number of outputs available

            BaseConnection** inputConnections;  ///< all input-connections
            BaseConnection** outputConnections; ///< all output-connections
        };
    }
}

#endif
