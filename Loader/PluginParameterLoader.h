#ifndef PLUGINPARAMETERLOADER_H_
#define PLUGINPARAMETERLOADER_H_

#include <string>

#include <Data/PluginParameter.h>
#include <Util/Xml/XmlParser.h>
#include <Util/Logger.h>

using namespace eLibV2::Data;
using namespace eLibV2::XML;
using namespace eLibV2::Util;

namespace eLibV2
{
    namespace Loader
    {
        class PluginParameterLoader
        {
        public:
#if defined(WIN32)
            static PluginParameters loadFromResource(HINSTANCE instance, UInt16 resourceId)
            {
                HRSRC hResource = NULL;
                HGLOBAL hRData = NULL;
                void *pRData;

                hResource = FindResource(instance, MAKEINTRESOURCE(resourceId), "RAW");
                UInt16 bufferSize = SizeofResource(instance, hResource);
                hRData = LoadResource(instance, hResource);
                pRData = LockResource(hRData);
                XmlParser parser(pRData, bufferSize);
                nodeVector *nodes = parser.getNodes();

                return load(nodes);
            }
#endif

            static PluginParameters loadFromXml(std::string filename)
            {
                XmlParser parser(filename);
                nodeVector *nodes = parser.getNodes();

                return load(nodes);
            }

            static PluginParameters load(nodeVector* nodes)
            {
                PluginParameters parameters;
                if (nodes)
                {
                    for (NodeVector::iterator nodeIt = nodes->begin(); nodeIt != nodes->end(); nodeIt++)
                    {
                        if ((*nodeIt)->getName() == "parameter")
                        {
                            std::string id, label;
                            PluginParameter::ParameterType type = PluginParameter::ParameterTypeDouble;
                            double min = 0.0, max = 0.0, init = 0.0;

                            StringMap attributes = (*nodeIt)->getAttributes();
                            for (StringMap::iterator attributeIt = attributes.begin(); attributeIt != attributes.end(); attributeIt++)
                            {
                                std::string attributeName = (*attributeIt).first;
                                std::string attributeValue = (*attributeIt).second;

                                if (attributeName == "id")
                                    id = attributeValue;
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
                                else if (attributeName == "init")
                                    init = atof(attributeValue.c_str());
                            }
                            PluginParameter parameter(id, label, type, min, max, init);
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
