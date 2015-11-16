#include <Util/ParamRandom.h>

using namespace eLibV2::Util;

void ParamRandomizer::Init(void)
{
    RandomizerMap.clear();
    srand((unsigned)time(NULL));
}

SInt16 ParamRandomizer::AddParam(SInt32 Id, double Min, double Max)
{
    sRandom tmp;

    if (!RandomizerMap.count(Id))
    {
        tmp.Max = Max;
        tmp.Min = Min;
        RandomizerMap[Id] = tmp;
        return Id;
    }
    else
        return -1;
}

SInt16 ParamRandomizer::DeleteParam(SInt32 Id)
{
    if (!RandomizerMap.count(Id))
        return -1;
    else
        RandomizerMap.erase(Id);

    return 0;
}

SInt16 ParamRandomizer::ProcessParam(SInt32 *Id, double *Value)
{
    SInt32 i, index;

    for (i = 0; i < 5; i++)
        index = (SInt32)(rand() % RandomizerMap.size());

    for (RandomizerMapIterator = RandomizerMap.begin(); RandomizerMapIterator != RandomizerMap.end(); RandomizerMapIterator++)
    {
        if (!index)
        {
            *Id = (*RandomizerMapIterator).first;
            *Value = ((double)rand() / (double)(RAND_MAX)) * (((*RandomizerMapIterator).second.Max - (*RandomizerMapIterator).second.Min) + (*RandomizerMapIterator).second.Min);
            break;
        }
        else
            index--;
    }
    return 0;
}
