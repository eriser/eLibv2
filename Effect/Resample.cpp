#include <Effect/Resample.h>

using namespace eLibV2::Effect;

void Resample::Init(void)
{
    setRate(RESAMPLE_RATE_MAX);
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
    double dInput = 0.0, dOutput = 0.0;

    if (inputConnections[RESAMPLE_CONNECTION_BYPASS] != NULL)
        m_bBypass = ModuleHelper::double2bool(inputConnections[RESAMPLE_CONNECTION_BYPASS]->processConnection(), 0.5);
    if (inputConnections[RESAMPLE_CONNECTION_RATE] != NULL)
        setRate((unsigned int)inputConnections[RESAMPLE_CONNECTION_RATE]->processConnection());
    if (inputConnections[RESAMPLE_CONNECTION_INPUT])
        dInput = inputConnections[RESAMPLE_CONNECTION_INPUT]->processConnection();

    if (!m_bBypass)
        dOutput = Process(dInput);
    else
        dOutput = dInput;

    // ModuleLogger::print(LOG_CLASS_EFFECT, "%s::process %lf -> %lf", getModuleName().c_str(), dInput, dOutput);
    return dOutput;
}

void Resample::setSamplerate(const double Samplerate)
{
    BaseModule::setSamplerate(Samplerate);
    adjustSamplePeriod();
}

void Resample::setRate(const double Rate)
{
    m_dRate = Util::ModuleHelper::clamp(Rate, RESAMPLE_RATE_MIN, RESAMPLE_RATE_MAX);
    adjustSamplePeriod();
}
