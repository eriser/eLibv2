#include <Effect/Phaser.h>

using namespace eLibV2::Effect;

double Phaser::Process(const double Input)
{
    double dOutput = 0.0, dFilterOutput = 0.0;

    // set filter input to input-signal + feedback
    m_pInput->setInput(Input * 0.5 + m_dLastFeedback);
    if (m_pFilterStages[5])
        dFilterOutput = m_pFilterStages[5]->processConnection();

    // feedback is output of filter-chain
    m_dLastFeedback = dFilterOutput;

    // calc new cutoff-frequencies for filters
    double filterFreq = 0.0;
    if (m_pLFO)
        filterFreq = (m_pLFO->processConnection() / 2.0) + 0.5;

    for (unsigned char stageIndex = 0; stageIndex < PHASER_FILTERSTAGES_NUM; ++stageIndex)
        m_pFilterStages[stageIndex]->setCutoff(m_dMaxCutoffs[stageIndex] * filterFreq + m_dMinCutoffs[stageIndex]);

    // output is mixed input and filter-output depending on depth
    dOutput = (m_dDepth * dFilterOutput) + ((1.0 - m_dDepth) * Input);
    return dOutput;
}

double Phaser::processConnection(void)
{
    double dInput = 0.0, dOutput = 0.0;

    if (isInputConnected(PHASER_CONNECTION_BYPASS))
        m_bBypass = ModuleHelper::double2bool(inputConnections[PHASER_CONNECTION_BYPASS]->processConnection(), 0.5);
    if (isInputConnected(PHASER_CONNECTION_INPUT))
        dInput = inputConnections[PHASER_CONNECTION_INPUT]->processConnection();
    if (isInputConnected(PHASER_CONNECTION_LFOFREQ))
    {
        if (m_pLFO)
            m_pLFO->setFreq(inputConnections[PHASER_CONNECTION_LFOFREQ]->processConnection());
    }

    if (!m_bBypass)
        dOutput = Process(dInput);
    else
        dOutput = dInput;
    return dOutput;
}

void Phaser::setSamplerate(const double Samplerate)
{
    BaseModule::setSamplerate(Samplerate);
    if (m_pLFO)
        m_pLFO->setSamplerate(Samplerate);
    for (unsigned char stageIndex = 0; stageIndex < PHASER_FILTERSTAGES_NUM; ++stageIndex)
    {
        if (m_pFilterStages[stageIndex])
            m_pFilterStages[stageIndex]->setSamplerate(Samplerate);
    }
}

void Phaser::setLFOFreq(const double Freq)
{
    if (m_pLFO)
        m_pLFO->setFreq(Freq);
}
