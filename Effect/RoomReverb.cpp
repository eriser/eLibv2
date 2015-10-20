#include <Effect/RoomReverb.h>

using namespace eLibV2::Effect;

void RoomReverb::Init()
{
    m_dPreDelayGain = 1.0;
    m_dReverbTimeRT60 = 1000.0;
    m_dMixLevel = 0.5;

    m_pPreDelay->setDelayLength(ModuleHelper::ms2samples(40.0, getSamplerate()));
    m_pInputLPF->setGain(0.45);
    m_pInputAPF[0]->setDelayLength(ModuleHelper::ms2samples(13.28, getSamplerate()));
    m_pInputAPF[0]->setGain(0.7);
    m_pInputAPF[1]->setDelayLength(ModuleHelper::ms2samples(28.13, getSamplerate()));
    m_pInputAPF[1]->setGain(-0.7);
    m_pCombLeft[0]->setDelayLength(ModuleHelper::ms2samples(31.71, getSamplerate()));
    m_pCombLeft[0]->setGainRT60(m_dReverbTimeRT60);
    m_pCombLeft[1]->setDelayLength(ModuleHelper::ms2samples(37.11, getSamplerate()));
    m_pCombLeft[0]->setGainRT60(m_dReverbTimeRT60);
    m_pCombLPFLeft[0]->setDelayLength(ModuleHelper::ms2samples(40.23, getSamplerate()));
    m_pCombLPFLeft[0]->setGainRT60(m_dReverbTimeRT60);
    m_pCombLPFLeft[1]->setDelayLength(ModuleHelper::ms2samples(44.14, getSamplerate()));
    m_pCombLPFLeft[1]->setGainRT60(m_dReverbTimeRT60);
    m_pCombRight[0]->setDelayLength(ModuleHelper::ms2samples(30.47, getSamplerate()));
    m_pCombRight[0]->setGainRT60(m_dReverbTimeRT60);
    m_pCombRight[1]->setDelayLength(ModuleHelper::ms2samples(33.98, getSamplerate()));
    m_pCombRight[1]->setGainRT60(m_dReverbTimeRT60);
    m_pCombLPFRight[0]->setDelayLength(ModuleHelper::ms2samples(41.41, getSamplerate()));
    m_pCombLPFRight[0]->setGainRT60(m_dReverbTimeRT60);
    m_pCombLPFRight[1]->setDelayLength(ModuleHelper::ms2samples(42.58, getSamplerate()));
    m_pCombLPFRight[1]->setGainRT60(m_dReverbTimeRT60);
    m_pOutputLPF[0]->setGain(0.5);
    m_pOutputLPF[1]->setGain(0.5);
    m_pOutputAPF[0]->setDelayLength(ModuleHelper::ms2samples(9.39, getSamplerate()));
    m_pOutputAPF[0]->setGain(-0.6);
    m_pOutputAPF[1]->setDelayLength(ModuleHelper::ms2samples(11, getSamplerate()));
    m_pOutputAPF[1]->setGain(0.6);
}

double RoomReverb::Process(const double Input)
{
    double dOutput = Input;
    m_dSecondOutput = dOutput;

    if (!m_bBypass)
    {
        double dPreDelayOut = m_pPreDelay->Process(Input);
        double dInputAPF1Out = m_pInputAPF[0]->Process(dPreDelayOut);
        double dInputAPF2Out = m_pInputAPF[1]->Process(dInputAPF1Out);
        double dInputLPFOut = m_pInputLPF->Process(dInputAPF2Out);

        double dFC1 = 0.15 * m_pCombLeft[0]->Process(dInputLPFOut) - 0.15 * m_pCombLeft[1]->Process(dInputLPFOut) + 0.15 * m_pCombLPFLeft[0]->Process(dInputLPFOut) - 0.15 * m_pCombLPFLeft[1]->Process(dInputLPFOut);
        double dFC2 = 0.15 * m_pCombRight[0]->Process(dInputLPFOut) - 0.15 * m_pCombRight[1]->Process(dInputLPFOut) + 0.15 * m_pCombLPFRight[0]->Process(dInputLPFOut) - 0.15 * m_pCombLPFRight[1]->Process(dInputLPFOut);

        double dOutputLPF1Out = m_pOutputLPF[0]->Process(dFC1);
        double dOutputLPF2Out = m_pOutputLPF[1]->Process(dFC2);

        double dOutputAPF1Out = m_pOutputAPF[0]->Process(dOutputLPF1Out);
        double dOutputAPF2Out = m_pOutputAPF[1]->Process(dOutputLPF2Out);

        dOutput = (1.0 - m_dMixLevel) * Input + m_dMixLevel * dOutputAPF1Out;
        m_dSecondOutput = (1.0 - m_dMixLevel) * Input + m_dMixLevel * dOutputAPF2Out;
    }
    return dOutput;
}

double RoomReverb::processConnection(void)
{
    double dInput = 0.0, dOutput = 0.0;

    if (isInputConnected(ROOMREVERB_CONNECTION_BYPASS))
        setBypass(ModuleHelper::double2bool(inputConnections[ROOMREVERB_CONNECTION_BYPASS]->processConnection(), 0.5));
    if (isInputConnected(ROOMREVERB_CONNECTION_INPUT))
        dInput = inputConnections[ROOMREVERB_CONNECTION_INPUT]->processConnection();

    if (!m_bBypass)
        dOutput = Process(dInput);
    else
        dOutput = dInput;
    return dOutput;
}

void RoomReverb::setSamplerate(const double Samplerate)
{
    BaseModule::setSamplerate(Samplerate);

    m_pPreDelay->setDelayLength(ModuleHelper::ms2samples(40.0, getSamplerate()));
    m_pInputAPF[0]->setDelayLength(ModuleHelper::ms2samples(13.28, getSamplerate()));
    m_pInputAPF[1]->setDelayLength(ModuleHelper::ms2samples(28.13, getSamplerate()));
    m_pCombLeft[0]->setDelayLength(ModuleHelper::ms2samples(31.71, getSamplerate()));
    m_pCombLeft[1]->setDelayLength(ModuleHelper::ms2samples(37.11, getSamplerate()));
    m_pCombLPFLeft[0]->setDelayLength(ModuleHelper::ms2samples(40.23, getSamplerate()));
    m_pCombLPFLeft[1]->setDelayLength(ModuleHelper::ms2samples(44.14, getSamplerate()));
    m_pCombRight[0]->setDelayLength(ModuleHelper::ms2samples(30.47, getSamplerate()));
    m_pCombRight[1]->setDelayLength(ModuleHelper::ms2samples(33.98, getSamplerate()));
    m_pCombLPFRight[0]->setDelayLength(ModuleHelper::ms2samples(41.41, getSamplerate()));
    m_pCombLPFRight[1]->setDelayLength(ModuleHelper::ms2samples(42.58, getSamplerate()));
    m_pOutputAPF[0]->setDelayLength(ModuleHelper::ms2samples(9.39, getSamplerate()));
    m_pOutputAPF[1]->setDelayLength(ModuleHelper::ms2samples(11, getSamplerate()));

}

void RoomReverb::setRT60(const double RT60)
{
    m_dReverbTimeRT60 = RT60;

    m_pCombLeft[0]->setGainRT60(m_dReverbTimeRT60);
    m_pCombLeft[0]->setGainRT60(m_dReverbTimeRT60);
    m_pCombLPFLeft[0]->setGainRT60(m_dReverbTimeRT60);
    m_pCombLPFLeft[1]->setGainRT60(m_dReverbTimeRT60);
    m_pCombRight[0]->setGainRT60(m_dReverbTimeRT60);
    m_pCombRight[1]->setGainRT60(m_dReverbTimeRT60);
    m_pCombLPFRight[0]->setGainRT60(m_dReverbTimeRT60);
    m_pCombLPFRight[1]->setGainRT60(m_dReverbTimeRT60);
}
