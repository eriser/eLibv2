#ifndef __PLUGINHOST_H__
#define __PLUGINHOST_H__

#include "aeffectx.h"

#include <VST/Host/VSTPluginInterface.h>
#include <Util/Defines.h>
#include <Util/ManagedBuffer.h>
#include <Util/Threads/EventManager.h>
#include <MIDI/MidiEvent.h>

#include <map>
#include <vector>

#define kBlockSize 512
#define kSampleRate 44000.f

using namespace eLibV2::Util;
using namespace eLibV2::MIDI;

#define OUTPUT_PLUGIN_PROPERTIES 1

namespace eLibV2
{
    namespace VST
    {
        namespace Host
        {
            typedef std::vector<std::string> StringList;
            typedef std::map<std::string, PluginInterface*> PluginMap;
            typedef std::vector<PluginInterface*> PluginInterfaceList;

            typedef AEffect *(*Vst2xPluginEntryFunc)(audioMasterCallback host);
            typedef VstIntPtr(*Vst2xPluginDispatcherFunc)(AEffect *effect, VstInt32 opCode, VstInt32 index, VstIntPtr value, void *ptr, float opt);
            typedef float(*Vst2xPluginGetParameterFunc)(AEffect *effect, VstInt32 index);
            typedef void(*Vst2xPluginSetParameterFunc)(AEffect *effect, VstInt32 index, float value);
            typedef void(*Vst2xPluginProcessFunc)(AEffect *effect, float **inputs, float **outputs, VstInt32 sampleFrames);

            class PluginHost
            {
            public:
                PluginHost();
                ~PluginHost();

                // Plugin Management
                bool OpenPlugin(std::string fileName);
                void UnloadAll();
                void StartAll();
                void StopAll();

                // Timer methods
                void StartTimer();
                void StopTimer();

                VstInt32 CheckCanDo(char *canDo);

                PluginInterfaceList GetLoadedPlugins();

                void InsertMidiEvent(VstInt16 channel, VstInt16 status, VstInt16 data1, VstInt16 data2);

                // Static methods
                static UInt16 GetBlocksize() { return ms_uiBlocksize; }
                static double GetTempo() { return ms_dTempo; }
                static double GetSampleRate() { return ms_dSamplerate; }
                static double GetTimeSignatureBeatsPerMeasure() { return ms_uiTimeSignatureBeatsPerMeasure; }
                static double GetTimeSignatureNoteValue() { return ms_uiTimeSignatureNoteValue; }
                static bool IsTransportPlaying() { return ms_bTransportPlaying; }

                static void SetInputBuffer(ManagedBuffer *buffer) { ms_inputBuffer = buffer; }
                static void SetOutputBuffer(ManagedBuffer *buffer) { ms_outputBuffer = buffer; }
                static ManagedBuffer* GetInputBuffer() { return ms_inputBuffer; }
                static ManagedBuffer* GetOutputBuffer() { return ms_outputBuffer; }
                static void SetBufferFillsize(VstInt16 bufferSize) { ms_iBufferRequestedSize = bufferSize; }

                /**
                callback function to receive messages from the plugin
                warning this is a static function, so it cannot access internal members (except when using singleton)
                @param effect the plugin which was sending the request (used when dealing with multiple plugins)
                @param opcode VST-opcode of the request
                @param index depends on opcode
                @param value depends on opcode
                @param ptr depends on opcode
                @param opt depends on opcode
                @return return value of action to send to plugin
                */
                static VstIntPtr VSTCALLBACK HostCallback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt);
#ifdef WIN32
                static DWORD WINAPI ProcessReplacing(LPVOID lpParam);
#endif

            public:
                struct ProcessThreadParameters
                {
                    ManagedBuffer* inputBuffer;
                    ManagedBuffer* outputBuffer;
                    PluginInterface* plugin;
                };

                /// the following variables have to be static to be accessed from HostCallback
                static VstTimeInfo _VstTimeInfo;
                static UInt16 ms_uiBlocksize;
                static double ms_dTempo;
                static double ms_dSamplerate;
                static UInt16 ms_uiTimeSignatureBeatsPerMeasure;
                static UInt16 ms_uiTimeSignatureNoteValue;
                static bool ms_bTransportPlaying;
                static LARGE_INTEGER ms_liElapsedMicroseconds;
                static ManagedBuffer *ms_inputBuffer;
                static ManagedBuffer *ms_outputBuffer;
                static VstInt16 ms_iBufferRequestedSize;
                static VstInt32 ms_SamplesProcessed;

            private:
                PluginInterfaceList GetPluginsForMidiChannel(VstInt16 channel);
                void ClosePlugin(std::string pluginID);

            private:
                PluginMap m_LoadedPlugins;
                LARGE_INTEGER m_liStartingTime, m_liEndingTime, m_liFrequency;
                std::map<VstInt16, PluginInterfaceList> m_PluginsForMIDIChannel;
            };
        }
    }
}
#endif
