#ifndef VSTBASEBITMAPMANAGER_H_
#define VSTBASEBITMAPMANAGER_H_

// std headers
#include <vector>
#include <map>
#include <string>

#include "Util/modLogger.h"

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
            ~VSTBaseBitmapManager() {}

            void addBitmap(VstInt32 id, CBitmap *bitmap);
            CBitmap *getBitmap(VstInt32 id) { return mBitmaps[id]; }
            void forgetBitmap(VstInt32 id);

        private:
            std::map<VstInt32, CBitmap*> mBitmaps;
        };
    }
}

#endif /* VSTBASEBITMAPMANAGER_H_ */