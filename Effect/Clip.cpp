#include <Effect/Clip.h>

using namespace eLibV2::Effect;

void Clip::Init()
{
    setClipLevel(1.0);
    setClipMode(CLIP_MODE_POSITIVE);
}

double Clip::Process(const double Input)
{
    double res = 0.0;

    switch (eClipMode)
    {
        case CLIP_MODE_POSITIVE:
            if (Input > dClipLevel)
                res = dClipLevel;
            else
                res = Input;
            break;

        case CLIP_MODE_NEGATIVE:
            if (Input < -dClipLevel)
                res = -dClipLevel;
            else
                res = Input;
            break;

        case CLIP_MODE_BOTH:
            if (Input < -dClipLevel)
                res = -dClipLevel;
            else if (Input > dClipLevel)
                res = dClipLevel;
            else
                res = Input;
            break;
    }
    return res;
}

double Clip::processConnection()
{
    double dInput = 0.0, dOutput = 0.0;

    if (isInputConnected(CLIP_CONNECTION_BYPASS))
        setBypass(ModuleHelper::double2bool(inputConnections[CLIP_CONNECTION_BYPASS]->processConnection(), 0.5));
    if (isInputConnected(CLIP_CONNECTION_INPUT))
        dInput = inputConnections[CLIP_CONNECTION_INPUT]->processConnection();

    if (!m_bBypass)
        dOutput = Process(dInput);
    else
        dOutput = dInput;
    return dOutput;
}
