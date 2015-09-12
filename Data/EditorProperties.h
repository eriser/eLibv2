#ifndef EDITORPROPERIES_H_
#define EDITORPROPERIES_H_

#include <Util/Defines.h>
#include <Util/Xml/GenericNode.h>
#include <Util/Xml/XmlParser.h>
#include <Util/Logger.h>

#include <string>
#include <map>

using namespace eLibV2::Util;
using namespace eLibV2::XML;

namespace eLibV2
{
    namespace Data
    {
        class EditorProperties
        {
        public:
            enum BitmapType
            {
                BitmapTypeUndefined = 0,
                BitmapTypeBackground,
                BitmapTypeControl
            };

        public:
            EditorProperties() {}

            static void loadFromXml(std::string filename)
            {
                XmlParser parser(filename);
                nodeVector *nodes = parser.getNodes();
                if (nodes)
                {
                    for (NodeVector::iterator nodeIt = nodes->begin(); nodeIt != nodes->end(); nodeIt++)
                    {
                        BitmapType type = BitmapTypeUndefined;
                        std::string id;
                        unsigned int resourceid, pageindex;
                        if ((*nodeIt)->getName() == "editor")
                        {
                            StringMap attributes = (*nodeIt)->getAttributes();
                            ModuleLogger::print(LOG_CLASS_DATA, "%li attributes", attributes.size());
                            for (StringMap::iterator attributeIt = attributes.begin(); attributeIt != attributes.end(); attributeIt++)
                            {
                                std::string attributeName = (*attributeIt).first;
                                std::string attributeValue = (*attributeIt).second;

                                if (attributeName == "numpages")
                                    setNumPages(atoi(attributeValue.c_str()));
                            }
                        }
                        if ((*nodeIt)->getName() == "bitmap")
                        {
                            StringMap attributes = (*nodeIt)->getAttributes();
                            ModuleLogger::print(LOG_CLASS_DATA, "%li attributes", attributes.size());
                            for (StringMap::iterator attributeIt = attributes.begin(); attributeIt != attributes.end(); attributeIt++)
                            {
                                std::string attributeName = (*attributeIt).first;
                                std::string attributeValue = (*attributeIt).second;

                                if (attributeName == "id")
                                    id = attributeValue;
                                else if (attributeName == "resourceid")
                                    resourceid = atoi(attributeValue.c_str());
                                else if (attributeName == "pageindex")
                                    pageindex = atoi(attributeValue.c_str());
                                else if (attributeName == "type")
                                {
                                    if (attributeValue == "background")
                                        type = BitmapTypeBackground;
                                    else if (attributeValue == "control")
                                        type = BitmapTypeControl;
                                }
                            }
                            if (type == BitmapTypeBackground)
                                addBackgroundBitmap(pageindex, resourceid);
                            else if (type == BitmapTypeControl)
                                addControlBitmap(id, resourceid);
                        }
                    }
                }
            }

        private:
            static void addBackgroundBitmap(const unsigned int pageIndex, const unsigned int resourceId) { mBackgroundBitmaps[pageIndex] = resourceId; }
            static void addControlBitmap(const std::string bitmapId, const unsigned int resourceId) { mControlBitmaps[bitmapId] = resourceId; }
            static void setNumPages(const unsigned int numPages) { mNumPages = numPages; }

        public:
            static std::map<unsigned int, unsigned int> mBackgroundBitmaps;
            static std::map<std::string, unsigned int> mControlBitmaps;
            static unsigned int mNumPages;
        };
    }
}

#endif
