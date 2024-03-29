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
#if defined(WIN32)
            static EditorParameters loadFromResource(HINSTANCE instance, UInt16 resourceId)
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

            static EditorParameters loadFromXml(std::string filename)
            {
                EditorParameters parameters;
                XmlParser parser(filename);
                nodeVector *nodes = parser.getNodes();

                return load(nodes);
            }

            static EditorParameters load(nodeVector* nodes)
            {
                EditorParameters parameters;
                UInt16 currentPageIndex = 0;
                if (nodes)
                {
                    for (NodeVector::iterator nodeIt = nodes->begin(); nodeIt != nodes->end(); nodeIt++)
                    {
                        if ((*nodeIt)->getName() == "page")
                        {
                            StringMap attributes = (*nodeIt)->getAttributes();
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
                            UInt16 pageIndex, posX, posY, tag, subpix;
                            std::string bitmapId;
                            
                            EditorParameter::ControlType type = EditorParameter::Undefined;

                            StringMap attributes = (*nodeIt)->getAttributes();
                            for (StringMap::iterator attributeIt = attributes.begin(); attributeIt != attributes.end(); attributeIt++)
                            {
                                std::string attributeName = (*attributeIt).first;
                                std::string attributeValue = (*attributeIt).second;

                                pageIndex = currentPageIndex;
                                if (attributeName == "type")
                                {
                                    if (attributeValue == "kickbutton")
                                        type = EditorParameter::KickButton;
                                    else if (attributeValue == "animknob")
                                        type = EditorParameter::AnimKnob;
                                    else if (attributeValue == "onoffbutton")
                                        type = EditorParameter::OnOffButton;
                                    else if (attributeValue == "moviebitmap")
                                        type = EditorParameter::MovieBitmap;
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
