/*
 * modClip.h
 *
 *  Created on: 26.11.2011
 *      Author: dedokter
 */

#ifndef MODCLIP_H_
#define MODCLIP_H_

#include <Base/modBaseEffect.h>

namespace eLibV2
{
    enum
    {
        CLIP_MODE_MIN = 0,
        CLIP_MODE_POSITIVE = 0,
        CLIP_MODE_NEGATIVE,
        CLIP_MODE_BOTH,
        CLIP_MODE_MAX
    };

    static const double CLIP_LEVEL_MIN = 0.0;
    static const double CLIP_LEVEL_MAX = 1.0;

    class FxClip : public BaseEffect
    {
    public:
		FxClip();
        ~FxClip();

        void Init(void);
        void Reset(void) {}

        double getClipLevel(void) {return dClipLevel;}
        long getClipMode(void) {return lClipMode;}

        void setClipLevel(double ClipLevel);
        void setClipMode(long ClipMode);

        double Process(double Input);
        double Process2(double Input1, double Input2);

		void runTests(void);

    private:
        double dClipLevel;
        long lClipMode;
    };
}

#endif /* MODCLIP_H_ */
