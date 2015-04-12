#ifndef PLUGINPARAMETERLOADER_H_
#define PLUGINPARAMETERLOADER_H_

#include <string>

#include <Data/modPluginParameter.h>

#include <Util/Xml/modXmlParser.h>

using namespace eLibV2::Data;
using namespace eLibV2::XML;

namespace eLibV2
{
    namespace Loader
    {
        class PluginParameterLoader
        {
        public:
            static PluginParameters loadFromXml(std::string filename)
            {
                PluginParameters parameters;
                XmlParser parser(filename);
                nodeVector *nodes = parser.getNodes();
                if (nodes)
                {
                    for (NodeVector::iterator nodeIt = nodes->begin(); nodeIt != nodes->end(); nodeIt++)
                    {
                        if ((*nodeIt)->getName() == "parameter")
                        {
                            std::string name, label;
                            PluginParameter::ParameterType type;
                            double min, max;

                            StringMap attributes = (*nodeIt)->getAttributes();
                            ModuleLogger::print("%li attributes", attributes.size());
                            for (StringMap::iterator attributeIt = attributes.begin(); attributeIt != attributes.end(); attributeIt++)
                            {
                                std::string attributeName = (*attributeIt).first;
                                std::string attributeValue = (*attributeIt).second;

                                if (attributeName == "name")
                                    name = attributeValue;
                                else if (attributeName == "label")
                                    label = attributeValue;
                                else if (attributeName == "type")
                                {
                                    if (attributeValue == "boolean")
                                        type = PluginParameter::ParameterTypeBoolean;
                                    else if (attributeValue == "integer")
                                        type = PluginParameter::ParameterTypeInteger;
                                    else if (attributeValue == "double")
                                        type = PluginParameter::ParameterTypeDouble;
                                }
                                else if (attributeName == "min")
                                    min = atof(attributeValue.c_str());
                                else if (attributeName == "max")
                                    max = atof(attributeValue.c_str());
                            }
                            PluginParameter parameter(name, label, type, min, max);
                            parameters.push_back(parameter);
                        }
                    }
                }
                return parameters;
            }
        };
    }
}

#endif
