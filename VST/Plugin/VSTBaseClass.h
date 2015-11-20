#ifndef VSTBASECLASS_H_
#define VSTBASECLASS_H_

// std includes
#include <sstream>

// VST includes
#include <audioeffectx.h>
#include <aeffguieditor.h>

// eLib includes
#include <Connection/InputConnection.h>
#include <Util/Defines.h>
#include <Util/Logger.h>
#include <MIDI/MidiEventHandler.h>

#include <Data/PluginProgram.h>
#include <Data/PluginParameter.h>
#include <Data/PluginProperties.h>

using namespace eLibV2::Data;
using namespace eLibV2::Util;

namespace eLibV2
{
    namespace VST
    {
        namespace Plugin
        {
            enum
            {
                NUM_MIDI_INPUT_CHANNELS = 16,
                NUM_MIDI_OUTPUT_CHANNELS = 16
            };

            class VSTBaseClass : public AudioEffectX
            {
            public:
                VSTBaseClass(audioMasterCallback audioMaster, PluginProperties properties);
                virtual ~VSTBaseClass();

                PluginProperties getProperties() { return mProperties; }

                // attach externally provided programs to plugin
                void attachPrograms(PluginPrograms programs);

                // attach midi event handler to process midi-events
                void attachMidiEventHandler(MIDI::MidiEventHandler *handler);

                // attach parameters
                void attachParameter(PluginParameter *param, Connection::InputConnection *inputConnection = 0);
                PluginParameter* getAttachedParameter(VstInt16 id);

                // virtual functions from AudioEffectX
            protected:

                // handling of programms
                virtual void setProgram(VstInt32 program);
                virtual void setProgramName(char* name);
                virtual void getProgramName(char* name);
                virtual bool getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text);

                // handling of parameters
                virtual void setParameter(VstInt32 index, float value);
                virtual float getParameter(VstInt32 index);
                virtual float getParameterScaled(VstInt32 index);
                virtual void getParameterName(VstInt32 index, char* text);
                virtual void getParameterDisplay(VstInt32 index, char* text);
                virtual void getParameterLabel(VstInt32 index, char* label);
                virtual bool canParameterBeAutomated(VstInt32 index);
                virtual bool getParameterProperties(VstInt32 index, VstParameterProperties* p);

                // vendor and product identifications
                virtual bool getEffectName(char* name);
                virtual bool getVendorString(char* text);
                virtual bool getProductString(char* text);
                virtual VstInt32 getVendorVersion();

                // general processing setup
                virtual void setSampleRate(float sampleRate);
                virtual bool getInputProperties(VstInt32 index, VstPinProperties* properties);
                virtual bool getOutputProperties(VstInt32 index, VstPinProperties* properties);
                virtual bool setProcessPrecision(VstInt32 precision);
                virtual VstInt32 getNumMidiInputChannels();
                virtual VstInt32 getNumMidiOutputChannels();
                virtual VstInt32 canDo(char* text);

                virtual VstInt32 startProcess() { ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::startProcess"); return 0; }
                virtual VstInt32 stopProcess() { ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::stopProcess"); return 0; }

                virtual void open() { ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::open"); }
                virtual void close() { ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::close"); }
                virtual void suspend() { ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseClass::suspend"); }

                // audio processing
                virtual void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames);
                virtual void processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames);
                virtual void process(double in1, double in2, double *out1, double *out2) = 0;
                virtual void resume();

                // event processing
                virtual VstInt32 processEvents(VstEvents* events);
                virtual VstInt32 processMidiEvent(VstInt16 channel, VstInt16 status, VstInt16 note, VstInt16 velocity);
                virtual VstInt32 processSysexEvent(VstInt32 size, char *data);

                // invoked abstract methods
                virtual void setParameterInvoked(VstInt32 index, float value) = 0;

                // 
                virtual bool beginEdit(VstInt32 index) { ModuleLogger::print(LOG_CLASS_VSTBASE, "beginEdit()"); return true; }
                virtual bool endEdit(VstInt32 index) { ModuleLogger::print(LOG_CLASS_VSTBASE, "endEdit()"); return true; }

            private:
                // properties for plugin
                PluginProperties mProperties;

                // active midi event handler
                MIDI::MidiEventHandler *mMidiEventHandler;

                // parameter properties
                std::vector<PluginParameter*> mParameters;
                std::vector<Connection::InputConnection*> mParameterConnections;

                // parameter values contained in programs
                std::vector<PluginProgram> mPrograms;
                VstInt32 mCurrentProgram;

            };
        }
    }
}

#endif
