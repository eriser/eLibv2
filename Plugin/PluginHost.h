#ifndef __PLUGINHOST_H__
#define __PLUGINHOST_H__

#include "aeffectx.h"

#include "PluginInterface.h"
#include <Util/ManagedBuffer.h>
#include <Util/Threads/EventManager.h>
#include <MIDI/MidiEvent.h>

#include <map>
#include <vector>

#define kBlockSize 512
#define kSampleRate 44000.f

using namespace eLibV2::Util;
using namespace eLibV2::MIDI;

namespace eLibV2
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
            void CloseAll();
            void StartAll();
            void StopAll();

            // Timer methods
            void StartTimer();
            void StopTimer();

            VstInt32 CheckCanDo(char *canDo);

            PluginInterfaceList GetLoadedPlugins();

            void InsertMidiEvent(int channel, int status, int data1, int data2);

            // Static methods
            static unsigned int GetBlocksize() { return ms_uiBlocksize; }
            static double GetTempo() { return ms_dTempo; }
            static double GetSampleRate() { return ms_dSamplerate; }
            static double GetTimeSignatureBeatsPerMeasure() { return ms_uiTimeSignatureBeatsPerMeasure; }
            static double GetTimeSignatureNoteValue() { return ms_uiTimeSignatureNoteValue; }
            static bool IsTransportPlaying() { return ms_bTransportPlaying; }

            static void SetManagedBuffer(ManagedBuffer *buffer) { ms_managedBuffer = buffer; }
            static ManagedBuffer* GetManagedBuffer() { return ms_managedBuffer; }
            static void SetBufferFillsize(int bufferSize) { ms_iBufferRequestedSize = bufferSize; }

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
            typedef struct ProcessThreadParameters
            {
                AEffect* plugin;
                float** inputs;
                float** outputs;
            };

            /// the following variables have to be static to be accessed from HostCallback
            static VstTimeInfo _VstTimeInfo;
            static unsigned int ms_uiBlocksize;
            static double ms_dTempo;
            static double ms_dSamplerate;
            static unsigned int ms_uiTimeSignatureBeatsPerMeasure;
            static unsigned int ms_uiTimeSignatureNoteValue;
            static bool ms_bTransportPlaying;
            static LARGE_INTEGER ms_liElapsedMicroseconds;
            static ManagedBuffer *ms_managedBuffer;
            static int ms_iBufferRequestedSize;
            static long ms_SamplesProcessed;

        private:
            PluginInterfaceList GetPluginsForMidiChannel(int channel);
            void ClosePlugin(std::string pluginID);

        private:
            PluginMap m_LoadedPlugins;
            unsigned int m_NumLoadedPlugins;
            LARGE_INTEGER m_liStartingTime, m_liEndingTime, m_liFrequency;
            std::map<int, PluginInterfaceList> m_PluginsForMIDIChannel;
        };
    }
}
#endif
