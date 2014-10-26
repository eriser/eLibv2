/*
 * modParamRandom.h
 *
 *  Created on: 25.11.2011
 *      Author: dedokter
 */

#ifndef MODPARAMRANDOM_H_
#define MODPARAMRANDOM_H_

#include <Base/modBaseModule.h>

#include <map>
#include <time.h>
#include <stdlib.h>

namespace eLibV2
{
	struct sRandom
	{
		double Min;
		double Max;
	};

	class ParamRandomizer : public BaseModule
	{
    public:
        ParamRandomizer();
        ~ParamRandomizer() {}

        void Init(void);

        int AddParam(long Id, double Min, double Max);
        int DeleteParam(long Id);
        int SearchParam(long Id);
        int ProcessParam(long *Id, double *Value);

    private:
        std::map<long, sRandom> RandomizerMap;
		std::map<long, sRandom>::iterator RandomizerMapIterator;
	};

}

#endif /* MODPARAMRANDOM_H_ */
