#ifndef VSTBASEBITMAPMANAGER_H_
#define VSTBASEBITMAPMANAGER_H_

// std headers
#include <vector>
#include <map>
#include <string>

#include <Util/Logger.h>
#include <Util/Defines.h>

// vst headers
#include <audioeffectx.h>
#include <vstcontrols.h>

namespace eLibV2
{
    namespace VSTBase
    {
        class VSTBaseBitmapManager
        {
        public:
            VSTBaseBitmapManager() {}
            ~VSTBaseBitmapManager();

            void addBitmap(const VstInt32 id, CBitmap *bitmap);
            CBitmap *getBitmap(const VstInt32 bitmapId) { return mBitmaps[bitmapId]; }
            void forgetBitmap(const VstInt32 id);

        private:
            std::map<VstInt32, CBitmap*> mBitmaps;
        };
    }
}

#endif
