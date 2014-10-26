#ifndef VSTBASEEDITORPROPERIES_H_
#define VSTBASEEDITORPROPERIES_H_

// std headers
#include <vector>

// vst headers
#include <audioeffectx.h>

namespace eLibV2
{
    namespace VSTBase
    {
        class VSTBaseEditorProperties
        {
        public:
            VSTBaseEditorProperties();
            virtual ~VSTBaseEditorProperties() {};

            void addBackgroundBitmap(const VstInt32 bitmapId);
            void addControlBitmap(const VstInt32 bitmapId);

            void setNumPages(const VstInt32 numPages) { mNumPages = numPages; }

        public:
            std::vector<VstInt32> mBackgroundBitmaps;
            std::vector<VstInt32> mControlBitmaps;
            VstInt32 mNumPages;
        };
    }
}

#endif /* VSTBASEEDITORPROPERIES_H_ */