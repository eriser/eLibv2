#include <Util/modParamRandom.h>

void ParamRandomizer::Init(void)
{
    RandomizerMap.clear();
    srand((unsigned)time(NULL));
}

int ParamRandomizer::AddParam(long Id, double Min, double Max)
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

int ParamRandomizer::DeleteParam(long Id)
{
    if (!RandomizerMap.count(Id))
        return -1;
    else
        RandomizerMap.erase(Id);

    return 0;
}

int ParamRandomizer::ProcessParam(long *Id, double *Value)
{
    long i, index;

    for (i = 0; i < 5; i++)
        index = (long)(rand() % RandomizerMap.size());

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
