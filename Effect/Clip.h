#ifndef MODCLIP_H_
#define MODCLIP_H_

#include <Base/BaseEffect.h>
#include <Util/Helper.h>

namespace eLibV2
{
    namespace Effect
    {
        static const double CLIP_LEVEL_MIN = 0.0;
        static const double CLIP_LEVEL_MAX = 1.0;

        class Clip : public Base::BaseEffect
        {
        public:
            enum
            {
                CLIP_CONNECTION_BYPASS = 0,
                CLIP_CONNECTION_INPUT,
                CLIP_CONNECTION_NUM
            };

        public:
            enum ClipMode
            {
                CLIP_MODE_UNDEF = 0,
                CLIP_MODE_POSITIVE = 0,
                CLIP_MODE_NEGATIVE,
                CLIP_MODE_BOTH
            };

        public:
            Clip(std::string name = "Clip") :
                BaseName(name)
            {
                Init();
            }
            virtual ~Clip() {}

            /* inherited */
            virtual void Init(void);
            virtual void Reset(void) {}
            virtual double Process(const double Input);
            virtual double processConnection(void);

        public:
            /* getter/setter */
            void setClipLevel(double ClipLevel) { dClipLevel = Util::ModuleHelper::clamp(ClipLevel, CLIP_LEVEL_MIN, CLIP_LEVEL_MAX); }
            double getClipLevel(void) { return dClipLevel; }

            void setClipMode(ClipMode Mode) { eClipMode = Mode; }
            ClipMode getClipMode(void) { return eClipMode; }

        public:
            void attachBypass(BaseConnection *connection) { inputConnections[CLIP_CONNECTION_BYPASS] = connection; }
            void attachInput(BaseConnection *connection) { inputConnections[CLIP_CONNECTION_INPUT] = connection; }

        private:
            double dClipLevel;
            ClipMode eClipMode;
        };
    }
}

#endif
