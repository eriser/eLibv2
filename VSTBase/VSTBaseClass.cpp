#include <VSTBase/VSTBaseClass.h>

using namespace eLibV2;
using namespace eLibV2::VSTBase;

VSTBaseClass::VSTBaseClass(audioMasterCallback audioMaster, PluginProperties properties) : AudioEffectX(audioMaster, properties.getNumPrograms(), properties.getNumParameters())
{
    ModuleLogger::print("VSTBaseClass::constructor");
    ModuleLogger::print("%li programs %li parameters", properties.getNumPrograms(), properties.getNumParameters());

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
        setUniqueID(properties.getIdAsLong());
    }
    suspend();
}

//------------------------------------------------------------------------
// attach externally provided programs to plugin
//------------------------------------------------------------------------
void VSTBaseClass::attachPrograms(PluginPrograms programs)
{
    ModuleLogger::print("VSTBaseClass::attachPrograms");
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
    ModuleLogger::print("VSTBaseClass::attachParameter");
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
    ModuleLogger::print("VSTBaseClass::setProgram: %d", program);
    if ((VstInt32)mPrograms.size() > program)
    {
        for (VstInt16 ParamIndex = 0; ParamIndex < mProperties.getNumParameters(); ParamIndex++)
            setParameter(ParamIndex, mPrograms[program].getParameter(ParamIndex));
        mCurrentProgram = program;
        AudioEffectX::updateDisplay();
    }
}

//-----------------------------------------------------------------------------------------
void VSTBaseClass::setProgramName(char* name)
{
    ModuleLogger::print("VSTBaseClass::setProgramName: %s", name);

    if (mPrograms.size())
        mPrograms[mCurrentProgram].setName(name);
}

//-----------------------------------------------------------------------------------------
void VSTBaseClass::getProgramName(char* name)
{
    ModuleLogger::print("VSTBaseClass::getProgramName: %s", mPrograms[mCurrentProgram].getName());
    vst_strncpy(name, mPrograms[mCurrentProgram].getName().c_str(), kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    ModuleLogger::print("VSTBaseClass::getProgramNameIndexed: %d %d", category, index);

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
    ModuleLogger::print("setParameter: %d %f", index, value);

    // set value of internal parameter
    mPrograms[mCurrentProgram].setParameter(index, value);

    // notify subclass of parameter change
    setParameterInvoked(index, value);

    ModuleLogger::print("%li %li %p", index, mParameterConnections.size(), mParameterConnections[index]);
    // process connected inputs
    if (index < mParameterConnections.size() && mParameterConnections[index])
    {
        ModuleLogger::print("setValue");
        mParameterConnections[index]->setInput(getParameterScaled(index));
    }
}

//------------------------------------------------------------------------
// get value of parameter for internal use in host application (0.0 - 1.0)
//------------------------------------------------------------------------
float VSTBaseClass::getParameter(VstInt32 index)
{
    double res = mPrograms[mCurrentProgram].getParameter(index);

    ModuleLogger::print("VSTBaseClass::getParameter: %d %f", index, res);
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
    ModuleLogger::print("getParameterName: %d", index);

    if (!label)
        return;

    if ((VstInt32)mParameters.size() > index)
        sprintf(label, "%s", mParameters[index]->getName().c_str());
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

    ModuleLogger::print("getParameterDisplay: %d -> %s", index, text);
}

//------------------------------------------------------------------------
// get unit for parameter to display in interal editor window
//------------------------------------------------------------------------
void VSTBaseClass::getParameterLabel(VstInt32 index, char *label)
{
    ModuleLogger::print("getParameterLabel: %d", index);

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
    ModuleLogger::print("VSTBaseClass::canParameterBeAutomated");
    return true;
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getParameterProperties(VstInt32 index, VstParameterProperties* p)
{
    ModuleLogger::print("VSTBaseClass::getParameterProperties");
    return false;
}


//-----------------------------------------------------------------------------------------
// vendor and product identifications
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getEffectName(char* name)
{
    std::stringstream temp;

    ModuleLogger::print("VSTBaseClass::getEffectName");
    temp << mProperties.getId() << " " << (int)mProperties.getVersion();

    vst_strncpy(name, temp.str().c_str(), kVstMaxEffectNameLen);
    return true;
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getProductString(char* text)
{
    std::stringstream temp;

    ModuleLogger::print("VSTBaseClass::getProductString");
    temp << mProperties.getId() << " " << (int)mProperties.getVersion();

    vst_strncpy(text, temp.str().c_str(), kVstMaxEffectNameLen);
    return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 VSTBaseClass::getVendorVersion()
{
    ModuleLogger::print("VSTBaseClass::getVendorVersion: %d", mProperties.getVersion());
    return mProperties.getVersion();
}


//-----------------------------------------------------------------------------------------
// general processing setup
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
void VSTBaseClass::setSampleRate(float sampleRate)
{
    ModuleLogger::print("VSTBaseClass::setSampleRate: %f", sampleRate);
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getInputProperties(VstInt32 index, VstPinProperties* properties)
{
    ModuleLogger::print("VSTBaseClass::getInputProperties");
    return false;
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getOutputProperties(VstInt32 index, VstPinProperties* properties)
{
    ModuleLogger::print("VSTBaseClass::getOutputProperties");
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
    ModuleLogger::print("VSTBaseClass::setProcessPrecision: %d", precision);
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
    ModuleLogger::print("VSTBaseClass::canDo");
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
            VstInt32 status = midiData[0] & 0xf0;
            VstInt32 note = midiData[1] & 0x7f;
            VstInt32 velocity = midiData[2] & 0x7f;

            if (status == 0x80)
                velocity = 0;

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
            ModuleLogger::print("VSTBaseClass::processEvents: unknown event type %d", (ev->events[i])->type);
        }
    }
    return 1;
}

//---------------------------------------------------------------------------
VstInt32 VSTBaseClass::processMidiEvent(VstInt16 channel, VstInt16 status, VstInt16 note, VstInt16 velocity)
{
    std::stringstream s;
    bool noteIsOn = false;

    if (status == 0x90)
        noteIsOn = (velocity > 0) ? true : false;

    if (mMidiEventHandler)
    {
        if (noteIsOn)
        {
            mMidiEventHandler->insertEvent(channel, MidiEvent(note, velocity));
            ModuleLogger::print("VSTBaseClass::processMidiEvents: inserting note %d/%d", note, velocity);
        }
        else
        {
            mMidiEventHandler->deleteEvent(channel, MidiEvent(note, 0));
            ModuleLogger::print("VSTBaseClass::processMidiEvents: deleting note %d", note);
        }
    }

    s << "VSTBaseClass::processMidiEvents: Midi Data: ";
    s << "channel: " << (int)channel << " Status: " << (int)status << " Note: " << (int)note << " Velocity: " << (int)velocity;

    ModuleLogger::print(s.str().c_str());
    return 1;
}

//---------------------------------------------------------------------------
VstInt32 VSTBaseClass::processSysexEvent(VstInt32 size, char *data)
{
    std::stringstream s;

    s << "Sysex Data: Size: " << (int)size;
    s.setf(std::ios::hex, std::ios::basefield);
    s.setf (std::ios::showbase);
    for (VstInt16 i = 0; i < size; i++)
        s << (int)data[i];

    ModuleLogger::print(s.str().c_str());
    return 1;
}

void VSTBaseClass::resume()
{
    ModuleLogger::print("VSTBaseClass::resume");
}
