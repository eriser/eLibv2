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

#if defined(WIN32)
            static bool loadFromResource(HINSTANCE instance, UInt16 resourceId)
            {
                bool bRes = false;
                HRSRC hResource = NULL;
                HGLOBAL hRData = NULL;
                void *pRData;

                hResource = FindResource(instance, MAKEINTRESOURCE(resourceId), "RAW");
                if (hResource)
                {
                    UInt16 bufferSize = SizeofResource(instance, hResource);
                    hRData = LoadResource(instance, hResource);
                    if (hRData)
                    {
                        pRData = LockResource(hRData);
                        XmlParser parser(pRData, bufferSize);
                        nodeVector *nodes = parser.getNodes();

                        bRes = load(nodes);
                    }
                }
                return bRes;
            }
#endif

            static bool loadFromXml(std::string filename)
            {
                XmlParser parser(filename);
                nodeVector *nodes = parser.getNodes();

                return load(nodes);
            }

            static bool load(nodeVector* nodes)
            {
                bool res = false;
                if (nodes)
                {
                    res = true;
                    for (NodeVector::iterator nodeIt = nodes->begin(); nodeIt != nodes->end(); nodeIt++)
                    {
                        BitmapType type = BitmapTypeUndefined;
                        std::string id;
                        UInt16 resourceid, pageindex;
                        if ((*nodeIt)->getName() == "editor")
                        {
                            StringMap attributes = (*nodeIt)->getAttributes();
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
                return res;
            }

        private:
            static void addBackgroundBitmap(const UInt8 pageIndex, const UInt16 resourceId) { mBackgroundBitmaps[pageIndex] = resourceId; }
            static void addControlBitmap(const std::string bitmapId, const UInt16 resourceId) { mControlBitmaps[bitmapId] = resourceId; }
            static void setNumPages(const UInt8 numPages) { mNumPages = numPages; }

        public:
            static std::map<UInt8, UInt16> mBackgroundBitmaps;
            static std::map<std::string, UInt16> mControlBitmaps;
            static UInt8 mNumPages;
        };
    }
}

#endif
