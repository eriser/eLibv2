#ifndef EDITORPARAMETER_H_
#define EDITORPARAMETER_H_

// std headers
#include <string>
#include <sstream>
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
            EditorParameter(const ControlType type, const unsigned int pageIndex, const unsigned int posX, const unsigned int posY, const unsigned int tag, const std::string bitmapId, const unsigned int subpix)
                : mType(type),
                mPageIndex(pageIndex),
                mPosX(posX),
                mPosY(posY),
                mTag(tag),
                mBitmapId(bitmapId),
                mSubpix(subpix)
            {}

            ControlType getControlType() { return mType; }
            unsigned int getPageIndex() { return mPageIndex; }
            unsigned int getPosX() { return mPosX; }
            unsigned int getPosY() { return mPosY; }
            unsigned int getTag() { return mTag; }
            std::string getBitmapId() { return mBitmapId; }
            unsigned int getSubpix() { return mSubpix; }

        private:
            ControlType mType;
            unsigned int mPageIndex;
            unsigned int mPosX;
            unsigned int mPosY;
            unsigned int mTag;
            std::string mBitmapId;
            unsigned int mSubpix;
        };

        typedef std::vector<EditorParameter> EditorParameters;
        typedef EditorParameters::iterator EditorParameterIterator;
    }
}

#endif
