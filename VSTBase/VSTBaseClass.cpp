#include <VSTBase/VSTBaseClass.h>

using namespace eLibV2;
using namespace eLibV2::VSTBase;
using namespace eLibV2::MIDI;

VSTBaseClass::VSTBaseClass(audioMasterCallback audioMaster, PluginProperties properties) : AudioEffectX(audioMaster, properties.getNumPrograms(), properties.getNumParameters())
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::constructor");
    ModuleLogger::print(LOG_CLASS_VSTBASE, "%li programs %li parameters", properties.getNumPrograms(), properties.getNumParameters());

    mProperties = properties;
    mCurrentProgram = 0;

    // init
    if (audioMaster)
    {
        setNumInputs(mProperties.getNumInputs());
        setNumOutputs(mProperties.getNumOutputs());
        if (mProperties.isSynth())
            isSynth();
        if (mProperties.canProcessReplacing())
            canProcessReplacing();
        if (mProperties.canDoubleReplacing())
            canDoubleReplacing();
        setUniqueID(mProperties.getIdAsLong());
    }
    suspend();
}

VSTBaseClass::~VSTBaseClass()
{

}

//------------------------------------------------------------------------
// attach externally provided programs to plugin
//------------------------------------------------------------------------
void VSTBaseClass::attachPrograms(PluginPrograms programs)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::attachPrograms");
    mPrograms.clear();
    for (PluginPrograms::iterator it = programs.begin(); it != programs.end(); it++)
        mPrograms.push_back(*it);
}

//------------------------------------------------------------------------
// attach midi event handler to process midi-events
//------------------------------------------------------------------------
void VSTBaseClass::attachMidiEventHandler(MidiEventHandler *handler)
{
    if (handler)
        mMidiEventHandler = handler;
}

//------------------------------------------------------------------------
// attach parameters
//------------------------------------------------------------------------
void VSTBaseClass::attachParameter(PluginParameter *param, Connection::InputConnection *inputConnection)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::attachParameter: %s %s", param->getId().c_str(), param->getLabel().c_str());
    mParameters.push_back(param);
    mParameterConnections.push_back(inputConnection);
}

PluginParameter* VSTBaseClass::getAttachedParameter(VstInt16 index)
{
    if (index < mParameters.size())
        return mParameters.at(index);
    else
        return 0;
}

//------------------------------------------------------------------------
// handling of programms
//------------------------------------------------------------------------

//------------------------------------------------------------------------
void VSTBaseClass::setProgram(VstInt32 program)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::setProgram: %d", program);
    if ((VstInt32)mPrograms.size() > program)
    {
        mCurrentProgram = program;
        for (VstInt16 ParamIndex = 0; ParamIndex < mProperties.getNumParameters(); ParamIndex++)
        {
            double ParamValue = mPrograms[program].getParameter(ParamIndex);
            ModuleLogger::print(LOG_CLASS_VSTBASE, "index: %d %lf", ParamIndex, ParamValue);

            setParameter(ParamIndex, ParamValue);
        }

        AudioEffectX::updateDisplay();
    }
}

//-----------------------------------------------------------------------------------------
void VSTBaseClass::setProgramName(char* name)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::setProgramName: %s", name);

    if (mPrograms.size())
        mPrograms[mCurrentProgram].setName(name);
}

//-----------------------------------------------------------------------------------------
void VSTBaseClass::getProgramName(char* name)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::getProgramName: %s", mPrograms[mCurrentProgram].getName());
    vst_strncpy(name, mPrograms[mCurrentProgram].getName().c_str(), kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::getProgramNameIndexed: %d %d", category, index);

    if ((VstInt32)mPrograms.size() > index)
        vst_strncpy (text, mPrograms[index].getName().c_str(), kVstMaxProgNameLen);
    return true;
}


//------------------------------------------------------------------------
// handling of parameters
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// set value of parameter for internal use in host application
//------------------------------------------------------------------------
void VSTBaseClass::setParameter(VstInt32 index, float value)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "setParameter: %d %f", index, value);

    // set value of internal parameter
    mPrograms[mCurrentProgram].setParameter(index, value);

    // notify subclass of parameter change
    setParameterInvoked(index, value);

    ModuleLogger::print(LOG_CLASS_VSTBASE, "%li %li %p", index, mParameterConnections.size(), mParameterConnections[index]);
    /// TODO process connected inputs
    if (index < mParameterConnections.size() && mParameterConnections[index])
    {
        ModuleLogger::print(LOG_CLASS_VSTBASE, "setValue");
        mParameterConnections[index]->setInput(getParameterScaled(index));
    }

    // set editor as well
    if (mProperties.hasEditor())
        ((AEffGUIEditor*)editor)->setParameter(index, value);
}

//------------------------------------------------------------------------
// get value of parameter for internal use in host application (0.0 - 1.0)
//------------------------------------------------------------------------
float VSTBaseClass::getParameter(VstInt32 index)
{
    double res = mPrograms[mCurrentProgram].getParameter(index);

    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::getParameter: %d %f", index, res);
    return res;
}

//------------------------------------------------------------------------
// get value of parameter for internal use in host application (minValue - maxValue)
//------------------------------------------------------------------------
float VSTBaseClass::getParameterScaled(VstInt32 index)
{
    return mParameters[index]->getValue(getParameter(index));
}

//------------------------------------------------------------------------
// get name of parameter to display in internal editor window
//------------------------------------------------------------------------
void VSTBaseClass::getParameterName(VstInt32 index, char *label)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "getParameterName: %d", index);

    if (!label)
        return;

    if ((VstInt32)mParameters.size() > index)
        sprintf(label, "%s", mParameters[index]->getId().c_str());
    else
        sprintf(label, "P%li", index);
}

//------------------------------------------------------------------------
// get value of parameter to display in internal editor window
//------------------------------------------------------------------------
void VSTBaseClass::getParameterDisplay(VstInt32 index, char *text)
{
    if (!text)
        return;

    if ((VstInt32)mParameters.size() > index)
        sprintf(text, "%s", mParameters[index]->getValueAsString(mPrograms[mCurrentProgram].getParameter(index)).c_str());
    else
        sprintf(text, "%f", mPrograms[mCurrentProgram].getParameter(index));

    ModuleLogger::print(LOG_CLASS_VSTBASE, "getParameterDisplay: %d -> %s", index, text);
}

//------------------------------------------------------------------------
// get unit for parameter to display in interal editor window
//------------------------------------------------------------------------
void VSTBaseClass::getParameterLabel(VstInt32 index, char *label)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "getParameterLabel: %d", index);

    if (!label)
        return;

    if ((VstInt32)mParameters.size() >= index)
        sprintf(label, "%s", mParameters[index]->getLabel().c_str());
    else
        sprintf(label, "L%li", index);
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::canParameterBeAutomated(VstInt32 index)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::canParameterBeAutomated");
    return true;
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getParameterProperties(VstInt32 index, VstParameterProperties* p)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::getParameterProperties");
    return false;
}


//-----------------------------------------------------------------------------------------
// vendor and product identifications
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getEffectName(char* name)
{
    std::stringstream temp;

    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::getEffectName");
    temp << mProperties.getName() << " " << (int)mProperties.getVersion();

    vst_strncpy(name, temp.str().c_str(), kVstMaxEffectNameLen);
    return true;
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getProductString(char* text)
{
    std::stringstream temp;

    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::getProductString");
    temp << mProperties.getId() << " " << (int)mProperties.getVersion();

    vst_strncpy(text, temp.str().c_str(), kVstMaxEffectNameLen);
    return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 VSTBaseClass::getVendorVersion()
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::getVendorVersion: %d", mProperties.getVersion());
    return mProperties.getVersion();
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getVendorString(char* text)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::getVendorString: %s", mProperties.getVendor().c_str());
    vst_strncpy(text, mProperties.getVendor().c_str(), kVstMaxVendorStrLen);

    return true;
}

//-----------------------------------------------------------------------------------------
// general processing setup
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
void VSTBaseClass::setSampleRate(float sampleRate)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::setSampleRate: %f", sampleRate);
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getInputProperties(VstInt32 index, VstPinProperties* properties)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::getInputProperties");
    return false;
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getOutputProperties(VstInt32 index, VstPinProperties* properties)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::getOutputProperties");
    if (index < mProperties.getNumOutputs())
    {
        vst_strncpy (properties->label, "O ", 63);
        char temp[11] = {0};
        int2string (index + 1, temp, 10);
        vst_strncat (properties->label, temp, 63);

        properties->flags = kVstPinIsActive;
        if (index < 2)
            properties->flags |= kVstPinIsStereo;    // make channel 1+2 stereo
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::setProcessPrecision(VstInt32 precision)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::setProcessPrecision: %d", precision);
    return false;
}

//-----------------------------------------------------------------------------------------
VstInt32 VSTBaseClass::getNumMidiInputChannels()
{
    return NUM_MIDI_INPUT_CHANNELS;
}

//-----------------------------------------------------------------------------------------
VstInt32 VSTBaseClass::getNumMidiOutputChannels()
{
    return NUM_MIDI_OUTPUT_CHANNELS;
}

//-----------------------------------------------------------------------------------------
VstInt32 VSTBaseClass::canDo(char* text)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::canDo");
    if (!strcmp(text, "receiveVstEvents"))
        return 1;
    if (!strcmp(text, "receiveVstMidiEvent"))
        return 1;
    if (!strcmp(text, "receiveVstTimeInfo"))
        return 1;
    if (!strcmp(text, "sendVstEvents"))
        return 1;
    if (!strcmp(text, "sendVstMidiEvent"))
        return 1;
    return -1;    // explicitly can't do; 0 => don't know
}


//---------------------------------------------------------------------------
// audio processing
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void VSTBaseClass::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
    double outval1 = 0.0f, outval2 = 0.0f;
    float inval1, inval2;
    float* in1 = inputs[0];
    float* in2 = inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

    while (--sampleFrames >= 0)
    {
        inval1 = *in1++;
        inval2 = *in2++;

        process(inval1, inval2, &outval1, &outval2);

        *out1++ = (float)outval1;
        *out2++ = (float)outval2;
    }
}

//---------------------------------------------------------------------------
void VSTBaseClass::processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
{
    double* in1 = inputs[0];
    double* in2 = inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
    double inval1, inval2;
    double outval1 = 0.0f, outval2 = 0.0f;

    while (--sampleFrames >= 0)
    {
        inval1 = *in1++;
        inval2 = *in2++;

        process(inval1, inval2, &outval1, &outval2);

        *out1++ = outval1;
        *out2++ = outval2;
    }
}


//---------------------------------------------------------------------------
// event processing
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
VstInt32 VSTBaseClass::processEvents(VstEvents* ev)
{
    for (VstInt32 i = 0; i < ev->numEvents; i++)
    {
        if ((ev->events[i])->type == kVstMidiType)
        {
            VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
            char* midiData = event->midiData;
            VstInt16 channel = midiData[0] & 0x0f;
            VstInt16 status = midiData[0] & 0xf0;
            VstInt16 note = midiData[1]; // &0x7f;
            VstInt16 velocity = midiData[2]; // &0x7f;

            if (status == 0x80)
                velocity = 0;

            ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::processEvents: mididata[3] = %d", midiData[3]);

            if (!processMidiEvent(channel, status, note, velocity))
                return 0;
        }
        else if ((ev->events[i])->type == kVstSysExType)
        {
            VstMidiSysexEvent* event = (VstMidiSysexEvent*)ev->events[i];
            if (!processSysexEvent(event->dumpBytes, event->sysexDump))
                return 0;
        }
        else
        {
            ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::processEvents: unknown event type %d", (ev->events[i])->type);
        }
    }
    return 1;
}

//---------------------------------------------------------------------------
VstInt32 VSTBaseClass::processMidiEvent(VstInt16 channel, VstInt16 status, VstInt16 note, VstInt16 velocity)
{
    std::stringstream s;

    if (!mMidiEventHandler)
        return 0;

    switch (status)
    {
        // note off
        case 0x80:
        // note on
        case 0x90:
            if (velocity > 0)
            {
                mMidiEventHandler->insertEvent(channel, MidiEvent(note, velocity));
                ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::processMidiEvents: inserting note %d/%d", note, velocity);
            }
            else
            {
                bool i = mMidiEventHandler->deleteEvent(channel, MidiEvent(note, 0));
                ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::processMidiEvents: deleting note %d %s", note, i ? "success" : "failure");
            }
            break;

        // aftertouch
        case 0xa0:
            ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::processMidiEvents: aftertouch");
            break;

        // control change
        case 0xb0:
            ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::processMidiEvents: control change");
            break;

        // program change
        case 0xc0:
            ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::processMidiEvents: program change");
            break;

        // chanel pressure
        case 0xd0:
            ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::processMidiEvents: channel pressure");
            break;

        // pitch bend change
        case 0xe0:
            ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::processMidiEvents: pitch bend");
            break;

        // system exclusive
        case 0xf0:
            ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::processMidiEvents: sysex");
            break;
    }

    s << "VSTBaseClass::processMidiEvents: Midi Data: ";
    s.setf(std::ios::hex, std::ios::basefield);
    s.setf(std::ios::showbase);
    s << "channel: " << (int)channel << " Status: " << (int)status << " Byte2: " << (int)note << " Byte3: " << (int)velocity;

    ModuleLogger::print(LOG_CLASS_VSTBASE, s.str().c_str());
    return 1;
}

//---------------------------------------------------------------------------
VstInt32 VSTBaseClass::processSysexEvent(VstInt32 size, char *data)
{
    std::stringstream s;

    s << "Sysex Data: Size: " << (int)size;
    s.setf(std::ios::hex, std::ios::basefield);
    s.setf(std::ios::showbase);
    for (VstInt16 i = 0; i < size; i++)
        s << (int)data[i];

    ModuleLogger::print(LOG_CLASS_VSTBASE, s.str().c_str());
    return 1;
}

void VSTBaseClass::resume()
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::resume");
}
