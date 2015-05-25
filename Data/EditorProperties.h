#ifndef EDITORPROPERIES_H_
#define EDITORPROPERIES_H_

// std headers
#include <vector>

// vst headers
#include <audioeffectx.h>

namespace eLibV2
{
    namespace Data
    {
        class EditorProperties
        {
        public:
            EditorProperties() {}

            void addBackgroundBitmap(const VstInt32 bitmapId) { mBackgroundBitmaps.push_back(bitmapId); }
            void addControlBitmap(const VstInt32 bitmapId) { mControlBitmaps.push_back(bitmapId); }

            void setNumPages(const VstInt32 numPages) { mNumPages = numPages; }

        public:
            std::vector<VstInt32> mBackgroundBitmaps;
            std::vector<VstInt32> mControlBitmaps;
            VstInt32 mNumPages;
        };
    }
}

#endif
