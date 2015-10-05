#include <Effect/Resample.h>

using namespace eLibV2::Effect;

void Resample::Init(void)
{
    setRate(44100.0);
    setBypass(false);
    m_dCurrentValue = 0.0;
    m_dCurrentSampleIndex = 0.0;
    m_dSamplePeriod = 0.0;
}

void Resample::Reset(void)
{
    Init();
}

double Resample::Process(const double Input)
{
    // get every x-samples the actual input
    // and hold this value until the next sampling period

    if (m_dCurrentSampleIndex > m_dSamplePeriod)
    {
        m_dCurrentValue = Input;
        m_dCurrentSampleIndex = 0.0;
    }
    else
        m_dCurrentSampleIndex++;

    return m_dCurrentValue;
}

double Resample::processConnection()
{
    double input = 0.0;

    //if (inputConnections[DITHER_CONNECTION_BYPASS] != NULL)
    //   inputConnections[DITHER_CONNECTION_BYPASS]->processConnection();
    if (inputConnections[RESAMPLE_CONNECTION_RATE] != NULL)
        setRate((unsigned int)inputConnections[RESAMPLE_CONNECTION_RATE]->processConnection());
    if (inputConnections[RESAMPLE_CONNECTION_INPUT])
        input = inputConnections[RESAMPLE_CONNECTION_INPUT]->processConnection();
    // ModuleLogger::print(LOG_CLASS_EFFECT, "%s::process %lf", getModuleName().c_str(), input);
    if (!m_bBypass)
        return Process(input);
    else
        return input;
}

void eLibV2::Effect::Resample::setSamplerate(double Samplerate)
{
    BaseModule::setSamplerate(Samplerate);
    adjustSamplePeriod();
}

void eLibV2::Effect::Resample::setRate(double samplerate)
{
    m_dRate = Util::ModuleHelper::clamp(samplerate, RESAMPLE_RATE_MIN, RESAMPLE_RATE_MAX);
    adjustSamplePeriod();
}
