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

        class PluginInterface
        {
        public:
            PluginInterface();
            ~PluginInterface();

            bool Load(const std::string fileName, audioMasterCallback callback);
            void Unload();
            AEffect* GetEffect() const { return m_pEffect; }
            std::string GetPluginID() { return m_PluginID; }

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
        };
    }
}

#endif
