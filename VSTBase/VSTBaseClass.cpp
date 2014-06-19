/*
 * VSTBaseClass
 */

#include <VSTBase/VSTBaseClass.h>

using namespace eLibV2;
using namespace eLibV2::VSTBase;

VSTBaseClass::VSTBaseClass(audioMasterCallback audioMaster, VstInt16 numPresets, VstInt16 numParameters, VSTBaseProperties properties) : AudioEffectX(audioMaster, numPresets, numParameters)
{
    ModuleLogger::print("constructor");
    mProperties = properties;
    mCurrentProgram = 0;

    // init
    if (audioMaster)
    {
        ModuleLogger::print("audiomaster");
        setNumInputs(mProperties.getNumInputs());
        setNumOutputs(mProperties.getNumOutputs());
        canProcessReplacing();
        if (mProperties.isSynth())
            isSynth();
        setUniqueID(properties.getPluginIdAsVstInt32());
    }
    suspend();
}

//------------------------------------------------------------------------
VSTBaseClass::~VSTBaseClass()
{
}

//------------------------------------------------------------------------
// attach externally provided programs to plugin
//------------------------------------------------------------------------
void VSTBaseClass::attachPrograms(VSTBasePrograms programs)
{
    ModuleLogger::print("attachPrograms");
    mPrograms.clear();
    for (VSTBasePrograms::iterator it = programs.begin(); it != programs.end(); it++)
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
// handling of programms
//------------------------------------------------------------------------

//------------------------------------------------------------------------
void VSTBaseClass::setProgram(VstInt32 program)
{
    ModuleLogger::print("setProgram: %d", program);
    if (mPrograms.size())
    {
        for (VstInt16 ParamIndex = 0; ParamIndex < numParameters; ParamIndex++)
            setParameter(ParamIndex, mPrograms[program].getParameter(ParamIndex));
        mCurrentProgram = program;
        AudioEffectX::updateDisplay();
    }
}

//-----------------------------------------------------------------------------------------
void VSTBaseClass::setProgramName(char* name)
{
    ModuleLogger::print("setProgramName: %s", name);
    if (mPrograms.size())
        mPrograms[mCurrentProgram].setName(name);
}

//-----------------------------------------------------------------------------------------
void VSTBaseClass::getProgramName(char* name)
{
    ModuleLogger::print("getProgramName: %s", mPrograms[mCurrentProgram].getName());
    vst_strncpy(name, mPrograms[mCurrentProgram].getName().c_str(), kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    ModuleLogger::print("getProgramNameIndexed: %d %d", category, index);
    if ((int)mPrograms.size() >= index + 1)
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
    mPrograms[mCurrentProgram].setParameter(index, value);
    setParameterInvoked(index, value);
}

//------------------------------------------------------------------------
// get value of parameter for internal use in host application
//------------------------------------------------------------------------
float VSTBaseClass::getParameter(VstInt32 index)
{
    return mPrograms[mCurrentProgram].getParameter(index);
}

//------------------------------------------------------------------------
// get name of parameter to display in internal editor window
//------------------------------------------------------------------------
void VSTBaseClass::getParameterName(VstInt32 index, char *label)
{
    if (!label)
        return;

    sprintf(label, "P%li", index);
}

//------------------------------------------------------------------------
// get value of parameter to display in internal editor window
//------------------------------------------------------------------------
void VSTBaseClass::getParameterDisplay(VstInt32 index, char *text)
{
    if (!text)
        return;

    sprintf(text, "%f", mPrograms[mCurrentProgram].getParameter(index));
}

//------------------------------------------------------------------------
// get unit for parameter to display in interal editor window
//------------------------------------------------------------------------
void VSTBaseClass::getParameterLabel(VstInt32 index, char *label)
{
    if (!label)
        return;

    sprintf(label, "L%li", index);
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::canParameterBeAutomated(VstInt32 index)
{
    ModuleLogger::print("canParameterBeAutomated");
    return true;
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getParameterProperties(VstInt32 index, VstParameterProperties* p)
{
    ModuleLogger::print("getParameterProperties");
    return false;
}


//-----------------------------------------------------------------------------------------
// vendor and product identifications
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getEffectName(char* name)
{
    std::stringstream temp;

    ModuleLogger::print("getEffectName");
    temp << mProperties.getPluginId() << " " << (int)mProperties.getVersion();

    vst_strncpy (name, temp.str().c_str(), kVstMaxEffectNameLen);
    return true;
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getProductString(char* text)
{
    std::stringstream temp;

    ModuleLogger::print("getProductString");
    temp << mProperties.getPluginId() << " " << (int)mProperties.getVersion();

    vst_strncpy (text, temp.str().c_str(), kVstMaxEffectNameLen);
    return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 VSTBaseClass::getVendorVersion()
{
    ModuleLogger::print("getVendorVersion: %d", mProperties.getVersion());
    return mProperties.getVersion();
}


//-----------------------------------------------------------------------------------------
// general processing setup
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
void VSTBaseClass::setSampleRate(float sampleRate)
{
    ModuleLogger::print("setSampleRate: %f", sampleRate);
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getInputProperties(VstInt32 index, VstPinProperties* properties)
{
    ModuleLogger::print("getInputProperties");
    return false;
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::getOutputProperties(VstInt32 index, VstPinProperties* properties)
{
    ModuleLogger::print("getOutputProperties");
    if (index < mProperties.getNumOutputs())
    {
        vst_strncpy (properties->label, "O ", 63);
        char temp[11] = {0};
        int2string (index + 1, temp, 10);
        vst_strncat (properties->label, temp, 63);

        properties->flags = kVstPinIsActive;
        if (index < 2)
            properties->flags |= kVstPinIsStereo;	// make channel 1+2 stereo
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
bool VSTBaseClass::setProcessPrecision(VstInt32 precision)
{
    ModuleLogger::print("setProcessPrecision");
    return false;
}

//-----------------------------------------------------------------------------------------
VstInt32 VSTBaseClass::canDo(char* text)
{
    ModuleLogger::print("canDo");
    if (!strcmp(text, "receiveVstEvents"))
        return 1;
    if (!strcmp(text, "receiveVstMidiEvent"))
        return 1;
    if (!strcmp(text, "receiveVstTimeInfo"))
        return 1;
    if (!strcmp(text, "sendVstEvents"))
        return 1;
    return -1;	// explicitly can't do; 0 => don't know
}


//---------------------------------------------------------------------------
// event and audio processing
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

void VSTBaseClass::process(double in1, double in2, double *out1, double *out2)
{
    *out1 = 0.0;

    *out2 = *out1;
}

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
            ModuleLogger::print("unknown event type %d", (ev->events[i])->type);
        }
    }
    return 1;
}

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
            ModuleLogger::print("inserting note %d/%d", note, velocity);
        }
        else
        {
            mMidiEventHandler->deleteEvent(channel, MidiEvent(note, 0));
            ModuleLogger::print("deleting note %d", note);
        }
    }

    s << "Midi Data: ";
    s << "channel: " << (int)channel << " Status: " << (int)status << " Note: " << (int)note << " Velocity: " << (int)velocity;

    ModuleLogger::print(s.str().c_str());
    return 1;
}

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
    ModuleLogger::print("resume");
}
