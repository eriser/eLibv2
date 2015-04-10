#ifndef MODCLIP_H_
#define MODCLIP_H_

#include <Base/modBaseEffect.h>
#include <Util/modHelper.h>

namespace eLibV2
{
    namespace Effect
    {
        enum ClipModes
        {
            CLIP_MODE_POSITIVE = 0,
            CLIP_MODE_NEGATIVE,
            CLIP_MODE_BOTH
        };

        static const double CLIP_LEVEL_MIN = 0.0;
        static const double CLIP_LEVEL_MAX = 1.0;

        class FxClip : public BaseEffect
        {
        public:
            FxClip(std::string name = "FxClip")
                : BaseName(name) {
                Init();
            }

            /* inherited */
            void Init(void);
            void Reset(void) {}
            bool Test();
            double Process(double Input);

            /* setter, getter */
            void setClipLevel(double ClipLevel);
            double getClipLevel(void) { return dClipLevel; }

            void setClipMode(ClipModes ClipMode);
            ClipModes getClipMode(void) { return eClipMode; }

        private:
            double dClipLevel;
            ClipModes eClipMode;
        };
    }
}

#endif
