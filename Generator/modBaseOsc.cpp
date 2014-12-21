#include <Generator/modBaseOsc.h>

BaseOscillator::~BaseOscillator()
{
    if (pBaseWavetable)
        delete pBaseWavetable;
}

void BaseOscillator::Init(void)
{
    pBaseWavetable = new BaseWavetable();

    setWaveform(1);
    setCoarse(0.0);
    setFinetune(0.0);
    setSamplerate(44100.0);
    dPhase = 0.0;

    SetupFreqs();
}

void BaseOscillator::Reset(void)
{
    setSamplerate(dSamplerate);
    dPhase = 0.0;
}

void BaseOscillator::setWaveform(VstInt32 Waveform)
{
    lWaveform = Waveform;
    setSamplerate(dSamplerate);
	adjustScaler();
}

void BaseOscillator::setSamplerate(double Samplerate)
{
    dSamplerate = Samplerate;
    adjustScaler();
}

void BaseOscillator::adjustScaler()
{
    if (pBaseWavetable)
        dScaler = pBaseWavetable->getWaveSize(lWaveform) / dSamplerate;
}

void BaseOscillator::setCoarse(double Coarse)
{
	dCoarse = clamp(Coarse, -BASEOSC_COARSE_RANGE, BASEOSC_COARSE_RANGE);
}

void BaseOscillator::setFinetune(double Finetune)
{
	dFinetune = clamp(Finetune, -BASEOSC_FINE_RANGE, BASEOSC_FINE_RANGE);
}

VstInt32 BaseOscillator::getNumWaveforms(void)
{
    if (pBaseWavetable)
        return pBaseWavetable->getNumLoadedWaveforms();
    else
        return 0;
}

double BaseOscillator::Process(VstInt16 Note)
{
    // modulation in simetones
    double data, dBase, dTune, dFreq;

    data = pBaseWavetable->getWaveData(lWaveform, dPhase);

    dBase = freqtab[(Note + (VstInt16)(dCoarse)) & 0x7f];
    if (dFinetune >= 0.0)
    {
		dTune = (freqtab[(Note + (VstInt16)(dCoarse)+1) & 0x7f] - dBase) * (((dFinetune + BASEOSC_FINE_RANGE) / (2 * BASEOSC_FINE_RANGE)) - 0.5) * 2;
        dFreq = (dBase + dTune) * dScaler;
    }
    else
    {
		dTune = (dBase - freqtab[(Note + (VstInt16)(dCoarse)-1) & 0x7f]) * (0.5 - ((dFinetune + BASEOSC_FINE_RANGE) / (2 * BASEOSC_FINE_RANGE))) * 2;
        dFreq = (dBase - dTune) * dScaler;
    }
    dPhase += dFreq;
    dPhase = pBaseWavetable->adjustPhase(lWaveform, dPhase);

    return data;
}

double BaseOscillator::processIOs(void)
{
	double input = 0.0, res;

	if (isAttached(OSC_INPUT_COARSE))
		setCoarse(controlIOs[OSC_INPUT_COARSE]->processIOs());
	if (isAttached(OSC_INPUT_FINETUNE))
		setFinetune(controlIOs[OSC_INPUT_FINETUNE]->processIOs());
	if (isAttached(OSC_INPUT_WAVEFORM))
		setWaveform((VstInt16)controlIOs[OSC_INPUT_WAVEFORM]->processIOs());
	if (isAttached(OSC_INPUT_NOTE))
		input = controlIOs[OSC_INPUT_NOTE]->processIOs();

	ModuleLogger::print("%s::processIOs %lf/%lf/%ld/%lf", getModuleName().c_str(), getCoarse(), getFinetune(), getWaveform(), input);
	res = Process((VstInt16)input);
	ModuleLogger::print("osc output: %lf", res);

	return res;
}
