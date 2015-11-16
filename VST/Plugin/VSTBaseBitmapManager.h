#ifndef VSTBASEBITMAPMANAGER_H_
#define VSTBASEBITMAPMANAGER_H_

// std headers
#include <vector>
#include <map>
#include <string>

#include <Util/Logger.h>
#include <Util/Defines.h>

// vst headers
#include <vstcontrols.h>

namespace eLibV2
{
    namespace VST
    {
        namespace Plugin
        {
            class VSTBaseBitmapManager
            {
            public:
                VSTBaseBitmapManager() {}
                ~VSTBaseBitmapManager();

                void addBitmap(const std::string id, CBitmap *bitmap);
                void addBackgroundBitmap(const UInt8 pageIndex, CBitmap *bitmap);

                CBitmap* getBitmap(const std::string bitmapId) { return mBitmaps[bitmapId]; }
                CBitmap* getBackgroundBitmap(const UInt8 pageIndex) { return mBackgroundBitmaps[pageIndex]; }

                void forgetBitmap(const std::string id);
                void clearAll();

            private:
                std::map<std::string, CBitmap*> mBitmaps;
                std::map<UInt8, CBitmap*> mBackgroundBitmaps;
            };
        }
    }
}

#endif
