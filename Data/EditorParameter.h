#ifndef EDITORPARAMETER_H_
#define EDITORPARAMETER_H_

// std headers
#include <vector>

namespace eLibV2
{
    namespace Data
    {
        class EditorParameter
        {
        public:
            enum ControlType
            {
                Undefined = 0,
                OnOffButton,
                KickButton,
                TextLabel,
                OptionMenu,
                AnimKnob,
                MovieBitmap,
                NumControls
            };

        public:
            EditorParameter(const ControlType type, const UInt8 pageIndex, const SInt16 posX, const SInt16 posY, const UInt16 tag, const std::string bitmapId, const UInt16 subpix)
                : mType(type),
                mPageIndex(pageIndex),
                mPosX(posX),
                mPosY(posY),
                mTag(tag),
                mBitmapId(bitmapId),
                mSubpix(subpix)
            {}

            ControlType getControlType() { return mType; }
            UInt8 getPageIndex() { return mPageIndex; }
            SInt16 getPosX() { return mPosX; }
            SInt16 getPosY() { return mPosY; }
            UInt16 getTag() { return mTag; }
            std::string getBitmapId() { return mBitmapId; }
            UInt16 getSubpix() { return mSubpix; }

        private:
            ControlType mType;
            UInt8 mPageIndex;
            SInt16 mPosX;
            SInt16 mPosY;
            UInt16 mTag;
            std::string mBitmapId;
            UInt16 mSubpix;
        };

        typedef std::vector<EditorParameter> EditorParameters;
        typedef EditorParameters::iterator EditorParameterIterator;
    }
}

#endif
