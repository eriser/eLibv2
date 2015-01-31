#ifndef VSTBASECLASS_H_
#define VSTBASECLASS_H_

#include <sstream>

#include <audioeffectx.h>

#include <Blocks/modInputConnection.h>
#include <Util/modLogger.h>
#include <MIDI/modMidiEventHandler.h>

#include <Data/modPluginProgram.h>
#include <Data/modPluginParameter.h>
#include <Data/modPluginProperties.h>

namespace eLibV2
{
	namespace VSTBase
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
				~VSTBaseClass() {}

				PluginProperties getProperties() { return mProperties; }

				// attach externally provided programs to plugin
				void attachPrograms(PluginPrograms programs);

				// attach midi event handler to process midi-events
				void attachMidiEventHandler(MidiEventHandler *handler);

				// attach parameters
				void attachParameter(PluginParameter param, InputConnection *inputConnection = 0);

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
				virtual bool getVendorString(char* text) { vst_strncpy (text, "SYS AudioResearch", kVstMaxVendorStrLen); return true; }
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

			private:
				// properties for plugin
				PluginProperties mProperties;

				// active midi event handler
				MidiEventHandler *mMidiEventHandler;

				// parameter properties
				std::vector<PluginParameter> mParameters;
				std::vector<InputConnection*> mParameterConnections;

				// parameter values contained in programs
				std::vector<PluginProgram> mPrograms;
				VstInt32 mCurrentProgram;

		};
	}
}

#endif
