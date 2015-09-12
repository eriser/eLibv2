#ifndef EDITORPARAMETERLOADER_H_
#define EDITORPARAMETERLOADER_H_

#include <string>

#include <Data/EditorParameter.h>
#include <Util/Xml/XmlParser.h>
#include <Util/Logger.h>

using namespace eLibV2::Data;
using namespace eLibV2::XML;
using namespace eLibV2::Util;

namespace eLibV2
{
    namespace Loader
    {
        class EditorParameterLoader
        {
        public:
            static EditorParameters loadFromXml(std::string filename)
            {
                EditorParameters parameters;
                XmlParser parser(filename);
                nodeVector *nodes = parser.getNodes();
                unsigned int currentPageIndex = 0;
                if (nodes)
                {
                    for (NodeVector::iterator nodeIt = nodes->begin(); nodeIt != nodes->end(); nodeIt++)
                    {
                        if ((*nodeIt)->getName() == "page")
                        {
                            StringMap attributes = (*nodeIt)->getAttributes();
                            ModuleLogger::print(LOG_CLASS_LOADER, "%li attributes", attributes.size());
                            for (StringMap::iterator attributeIt = attributes.begin(); attributeIt != attributes.end(); attributeIt++)
                            {
                                std::string attributeName = (*attributeIt).first;
                                std::string attributeValue = (*attributeIt).second;

                                if (attributeName == "index")
                                    currentPageIndex = atoi(attributeValue.c_str()) - 1;
                            }
                        }
                        else if ((*nodeIt)->getName() == "control")
                        {
                            unsigned int pageIndex, posX, posY, tag, subpix;
                            std::string bitmapId;
                            EditorParameter::ControlType type = EditorParameter::ControlType::Undefined;

                            StringMap attributes = (*nodeIt)->getAttributes();
                            ModuleLogger::print(LOG_CLASS_LOADER, "%li attributes", attributes.size());
                            for (StringMap::iterator attributeIt = attributes.begin(); attributeIt != attributes.end(); attributeIt++)
                            {
                                std::string attributeName = (*attributeIt).first;
                                std::string attributeValue = (*attributeIt).second;

                                pageIndex = currentPageIndex;
                                if (attributeName == "type")
                                {
                                    if (attributeValue == "kickbutton")
                                        type = EditorParameter::ControlType::KickButton;
                                    else if (attributeValue == "animknob")
                                        type = EditorParameter::ControlType::AnimKnob;
                                    else if (attributeValue == "onoffbutton")
                                        type = EditorParameter::ControlType::OnOffButton;
                                    else if (attributeValue == "moviebitmap")
                                        type = EditorParameter::ControlType::MovieBitmap;
                                }
                                else if (attributeName == "posx")
                                    posX = atoi(attributeValue.c_str());
                                else if (attributeName == "posy")
                                    posY = atoi(attributeValue.c_str());
                                else if (attributeName == "tag")
                                    tag = atoi(attributeValue.c_str());
                                else if (attributeName == "bitmapid")
                                    bitmapId = attributeValue;
                                else if (attributeName == "subpix")
                                    subpix = atoi(attributeValue.c_str());
                            }
                            EditorParameter parameter(type, pageIndex, posX, posY, tag, bitmapId, subpix);
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
