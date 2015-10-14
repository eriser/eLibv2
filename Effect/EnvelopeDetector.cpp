#include <Effect/EnvelopeDetector.h>

using namespace eLibV2::Effect;

void EnvelopeDetector::Init(void)
{
    bBypass = false;
    setSamplerate(44100.0);
    setAttack(0.0);
    setRelease(0.0);
    setTCMode(ENVELOPEDETECTOR_TCMODE_ANALOG);
    setDetectMode(ENVELOPEDETECTOR_DETECTMODE_PEAK);
    m_dCurrentLevel = 0.0;
}

void EnvelopeDetector::Reset(void)
{
    Init();
}

double EnvelopeDetector::Process(const double Input)
{
    double dInput = 0.0;

    switch (m_eDetectMode)
    {
        case ENVELOPEDETECTOR_DETECTMODE_PEAK:
            dInput = fabs(Input);
            break;

        case ENVELOPEDETECTOR_DETECTMODE_MS:
            dInput = fabs(Input) * fabs(Input);
            break;

        case ENVELOPEDETECTOR_DETECTMODE_RMS:
            dInput = pow(fabs(Input) * fabs(Input), 0.5);
            break;
    }

    if (dInput > m_dCurrentLevel)
        m_dCurrentLevel = m_dAttackScaled * (m_dCurrentLevel - dInput) + dInput;
    else
        m_dCurrentLevel = m_dReleaseScaled * (m_dCurrentLevel - dInput) + dInput;

    if (m_dCurrentLevel > 0.0 && m_dCurrentLevel < FLT_MIN_PLUS) m_dCurrentLevel = 0.0;
    if (m_dCurrentLevel < 0.0 && m_dCurrentLevel > FLT_MIN_MINUS) m_dCurrentLevel = 0.0;

    m_dCurrentLevel = ModuleHelper::clamp(m_dCurrentLevel, 0.0, 1.0);

    return m_dCurrentLevel;
}

double EnvelopeDetector::processConnection()
{
    double input = 0.0;

    if (isInputConnected(ENVELOPEDETECTOR_CONNECTION_ATTACK))
        setAttack(inputConnections[ENVELOPEDETECTOR_CONNECTION_ATTACK]->processConnection());
    if (isInputConnected(ENVELOPEDETECTOR_CONNECTION_RELEASE))
        setRelease(inputConnections[ENVELOPEDETECTOR_CONNECTION_RELEASE]->processConnection());
    if (isInputConnected(ENVELOPEDETECTOR_CONNECTION_INPUT))
        input = inputConnections[ENVELOPEDETECTOR_CONNECTION_INPUT]->processConnection();
    return Process(input);
}

void EnvelopeDetector::setSamplerate(const double Samplerate)
{
    BaseModule::setSamplerate(Samplerate);
    setAttack(m_dAttack);
    setRelease(m_dRelease);
}

void EnvelopeDetector::setAttack(const double Attack)
{
    m_dAttack = Attack;

    if (m_eTCMode == ENVELOPEDETECTOR_TCMODE_ANALOG)
        m_dAttackScaled = exp(ANALOG_TC / (m_dAttack * mSamplerate * 0.001));
    else
        m_dAttackScaled = exp(DIGITAL_TC / (m_dAttack * mSamplerate * 0.001));
}

void EnvelopeDetector::setRelease(const double Release)
{
    m_dRelease = Release;

    if (m_eTCMode == ENVELOPEDETECTOR_TCMODE_ANALOG)
        m_dReleaseScaled = exp(ANALOG_TC / (m_dRelease * mSamplerate * 0.001));
    else
        m_dReleaseScaled = exp(DIGITAL_TC / (m_dRelease * mSamplerate * 0.001));
}

void EnvelopeDetector::setTCMode(const TCMode TCMode)
{
    m_eTCMode = TCMode;
    setAttack(m_dAttack);
    setRelease(m_dRelease);
}

void EnvelopeDetector::setDetectMode(const DetectMode DetectMode)
{
    m_eDetectMode = DetectMode;
}
