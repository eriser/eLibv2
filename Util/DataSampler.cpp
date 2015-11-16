#include <Util/DataSampler.h>

using namespace eLibV2::Util;

UInt16 DataSampler::ms_SampleDuration = 1000;
UInt16 DataSampler::ms_CurrentlySampled = 0;

std::string DataSampler::ms_MessagePrefix = "sampled: ";

void DataSampler::sample(const SInt16 input)
{
    if (ms_CurrentlySampled >= ms_SampleDuration)
    {
        std::stringstream ss;
        ss << ms_MessagePrefix << (SInt16)input << std::endl;

#ifdef WIN32
        OutputDebugString(ss.str().c_str());
#else
        std::cout << ss.str();
#endif
        ms_CurrentlySampled = 0;
    }
    else
        ms_CurrentlySampled++;
}

void DataSampler::sample(const double input)
{
    if (ms_CurrentlySampled >= ms_SampleDuration)
    {
        std::stringstream ss;
        ss << ms_MessagePrefix << (double)input << std::endl;

#ifdef WIN32
        OutputDebugString(ss.str().c_str());
#else
        std::cout << ss.str();
#endif
        ms_CurrentlySampled = 0;
    }
    else
        ms_CurrentlySampled++;
}
