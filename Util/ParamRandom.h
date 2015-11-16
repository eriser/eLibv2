#ifndef MODPARAMRANDOM_H_
#define MODPARAMRANDOM_H_

#include <Base/BaseModule.h>

#include <map>
#include <time.h>
#include <stdlib.h>

namespace eLibV2
{
    namespace Util
    {
        class ParamRandomizer : public Base::BaseModule
        {
        public:
            ParamRandomizer() : Base::BaseName("ParamRandomizer") { Init(); }

            void Init(void);

            SInt16 AddParam(SInt32 Id, double Min, double Max);
            SInt16 DeleteParam(SInt32 Id);
            SInt16 SearchParam(SInt32 Id);
            SInt16 ProcessParam(SInt32 *Id, double *Value);

        private:
            struct sRandom
            {
                double Min;
                double Max;
            };

        private:
            std::map<SInt32, sRandom> RandomizerMap;
            std::map<SInt32, sRandom>::iterator RandomizerMapIterator;
        };
    }

}

#endif