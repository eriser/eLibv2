#ifndef __PLUGINLOADER_H__
#define __PLUGINLOADER_H__

#include "aeffectx.h"

#include <iostream>
#include <string>

#if _WIN32
#include <windows.h>
#elif TARGET_API_MAC_CARBON
#include <CoreFoundation/CoreFoundation.h>
#endif

#define kNumProcessCycles 50

namespace eLibV2
{
    namespace Host
    {
        typedef AEffect* (*PluginEntryProc) (audioMasterCallback audioMaster);

        enum PluginType
        {
            PLUGIN_TYPE_UNSET = 0,
            PLUGIN_TYPE_INSTRUMENT,
            PLUGIN_TYPE_EFFECT,
            PLUGIN_TYPE_SHELL,
        };

        /**
        This class provides an abstraction for VST plugins.
        It uses the filename of the plugin as an input and encapsulates all further things.
        Loads the plugin's library and connects it to the host-callback

        Handles:
        - Plugintype (Instrument, Effect, Shell-Plugin)
        - MidiInput channel (if applicable)
        - AudioOutput channel


        */
        class PluginInterface
        {
        public:
            PluginInterface();
            ~PluginInterface();

            bool Load(const std::string fileName, audioMasterCallback callback);
            void Unload();
            AEffect* GetEffect() const { return m_pEffect; }
            std::string GetPluginID() { return m_PluginID; }
            PluginType GetPluginType() { return m_ePluginType; }
            bool CanReceiveMidi() { return m_bCanReceiveMidi; }

            unsigned int GetMidiChannel() { return m_uiMidiChannel; }
            unsigned int GetAudioChannl() { return m_uiAudioChannel; }

            void Start();
            void Stop();
            void SendMidi(int channel, int status, int data1, int data2);

            void PrintProperties();
            void PrintPrograms();
            void PrintParameters();
            void PrintCapabilities();
            void ProcessReplacing();

            static inline void GetPluginStringFromLong(VstInt32 id, char* pluginID)
            {
                for (int i = 0; i < 4; i++)
                    pluginID[i] = (char)(id >> ((3 - i) * 8) & 0xff);
            }

        private:
            PluginEntryProc GetMainEntry();

            /**
            this function calls the main entry function of the plugin
            */
            bool CallPluginEntry();

            /**
            set up internal plugin data (e.g. samplerate and blocksize)
            */
            void Setup();

            void SetupProcessingMemory();
            void FreeProcessingMemory();

        private:
            void*               m_pModule;          ///< low-level module for LoadLibrary
            AEffect*            m_pEffect;          ///< the AEffect main entry
            std::string         m_FileName;         ///< associated filename
            audioMasterCallback m_pHostCallback;    ///< host callback-function
            double              m_fSamplerate;      ///< current samplerate
            unsigned int        m_uiBlocksize;      ///< current blocksize
            std::string         m_PluginID;         ///< pluginID
            unsigned int        m_uiMidiChannel;    ///< Midi channel to receive messages
            unsigned int        m_uiAudioChannel;   ///< Audio channel to use for output
            PluginType          m_ePluginType;      ///< Type of Plugin
            bool                m_bCanReceiveMidi;

            float**             m_ppInputs;
            float**             m_ppOutputs;
            VstInt32            m_uiNumInputs;
            VstInt32            m_uiNumOutputs;

            // these variables have to be non-local to have plugins
            // access them after the send-method has been left
            // TODO: maybe some better solution?
            VstEvents           events;
            VstMidiEvent        midiEvent = {};
        };
    }
}

#endif
