#ifndef __PLUGINLOADER_H__
#define __PLUGINLOADER_H__

#include "aeffectx.h"

#include <Util/Defines.h>
#include <Util/ManagedBuffer.h>

#include <iostream>
#include <sstream>
#include <string>

#if WIN32
#include <windows.h>
#elif TARGET_API_MAC_CARBON
#include <CoreFoundation/CoreFoundation.h>
#endif

using namespace eLibV2::Util;

namespace eLibV2
{
    namespace Host
    {
        typedef AEffect* (*PluginEntryProc) (audioMasterCallback audioMaster);

        /// type of loaded plugin
        enum PluginType
        {
            PLUGIN_TYPE_UNSET = 0,  ///< not yet set
            PLUGIN_TYPE_INSTRUMENT, ///< VST instrument
            PLUGIN_TYPE_EFFECT,     ///< VST effect
            PLUGIN_TYPE_SHELL,      ///< VST shell plugin
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
            PluginInterface::PluginInterface()
                : m_pModule(NULL),
                m_pEffect(NULL),
                m_pHostCallback(NULL),
                m_fSamplerate(44100.0),
                m_uiBlocksize(512),
                m_uiMidiChannel(0),
                m_uiAudioChannel(0),
                m_ePluginType(PluginType::PLUGIN_TYPE_UNSET),
                m_bCanReceiveMidi(false),
                m_bHasEditor(false),
                m_bCanReplacing(false),
                m_uiVstVersion(0),
                m_bPluginRunning(false),
                m_uiNumInputs(0),
                m_uiNumOutputs(0),
                m_ppInputs(NULL),
                m_ppOutputs(NULL)
            {
            }

            PluginInterface::~PluginInterface()
            {
            }

            /**
            Loads the plugin and connects to the specified callback-function
            @param fileName location of the plugin to load
            @param callback callback-function which receives all plugin requests
            @return true if successful, otherwise false
            */
            bool Load(const std::string fileName, audioMasterCallback callback);

            /**
            Unload the attached plugin
            */
            void Unload();

            /**
            Get ID of loaded plugin. This is a unique 4 characters long identification registered at Steinberg
            @return ID of plugin as string
            */
            std::string GetPluginID() { return m_PluginID; }

            /**
            Get type of plugin @see PluginType
            @return type of plugin
            */
            PluginType GetPluginType() { return m_ePluginType; }

            /**
            Information whether loaded plugin supports receiving of MIDI events
            @return true if plugin can receive MIDI events
            */
            bool CanReceiveMidi() { return m_bCanReceiveMidi; }

            /**
            Information whether loaded plugin has own editor
            @return true if plugin has editor
            */
            bool HasEditor() { return m_bHasEditor; }

            /**
            Returns the MIDI channel the plugin receives data
            @return MIDI channel 0-15
            */
            unsigned int GetMidiChannel() { return m_uiMidiChannel; }

            /**
            Returns the audio channel the plugin uses to send data to
            @return audio channel
            */
            unsigned int GetAudioChannel() { return m_uiAudioChannel; }

            std::string GetEffectName();

            ERect* GetEditorRect();

            /**
            Start plugin. This just calls the resume-method. It does not process any audio data.
            */
            void Start();

            /**
            Stop plugin. This just calls the suspend-method. In this state the plugin will not receive any calls to processReplacing.
            */
            void Stop();

            /**
            Open plugin's editor
            @param pointer to platform-dependent window handle
            */
            void OpenEditor(void* window);

            /**
            Close plugin's editor
            */
            void CloseEditor();

            /**
            Send idle message to plugin's editor
            */
            void IdleEditor();

            /**
            Send MIDI events to plugin
            @param channel MIDI channel
            @param status first byte of MIDI event
            @param data1 second byte of MIDI event
            @param data2 third byte of MIDI event
            */
            void SendMidi(int channel, int status, int data1, int data2);

            /**
            Output properties as number of inputs/outputs, number of presets/parameters to the console
            */
            void PrintProperties();

            /**
            Output all program-names to the console
            */
            void PrintPrograms();

            /**
            Output all parameters to the console
            */
            void PrintParameters();

            /**
            Output all capabilities (CanDoXXX) to the console
            */
            void PrintCapabilities();

            /**
            This calls the processReplacing-method of the plugin
            @param sampleFrames number of frames to process
            */
            void ProcessReplacing(VstInt32 sampleFrames);

            /**
            Return number of input channels the plugin is using
            @return number of input channels
            */
            VstInt32 GetNumberOfInputs() { return m_uiNumInputs; }

            /**
            Return number of output channels the plugin is using
            @return number of output channels
            */
            VstInt32 GetNumberOfOutputs() { return m_uiNumOutputs; }

            /**
            Return VST-version the plugin is implementing
            @return VST-version (e.g. 2400 for 2.4)
            */
            VstInt16 GetVstVersion() { return m_uiVstVersion; }

            /**
            Sync contents of managed buffer with input to use in processReplacing
            @param managedBuffer source of the data
            @param dataSize size of data to write (usually number of floats)
            */
            void SyncInputBuffers(ManagedBuffer* managedBuffer, VstInt32 dataSize);

            /**
            Sync contents of output buffer with managed buffer
            @param managedBuffer destination of the data
            @param dataSize size of data to write (usually number of floats)
            */
            void SyncOutputBuffers(ManagedBuffer* managedBuffer, VstInt32 dataSize);

            /**
            Convert the 32-bit-integer used for the plugin-ID to a character array
            @param id the plugin-ID as a 32-bit-integer
            @param pluginID character array to store readable form of plugin-ID. Must have at least 5 characters of space (including '\0')
            */
            static inline void GetPluginStringFromLong(VstInt32 id, char* pluginID)
            {
                for (int i = 0; i < 4; i++)
                    pluginID[i] = (char)(id >> ((3 - i) * 8) & 0xff);
                pluginID[4] = '\0';
            }

        private:
            /**
            Return the internal main-function of the plugin. Either VSTPlugMain or main
            */
//            PluginEntryProc GetMainEntry();

            /**
            This function calls the main entry function of the plugin
            */
            bool CallPluginEntry();

            /**
            Set up internal plugin data (e.g. samplerate and blocksize)
            */
            void Setup();

            /**
            Set up processing memory of the plugin for calls to processReplacing
            */
            void SetupProcessingMemory();

            /**
            Free up the reserved memory used to call processReplacing
            */
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
            bool                m_bCanReceiveMidi;  ///< Plugin can receive MIDI events
            bool                m_bHasEditor;        ///< Plugin has editor available
            bool                m_bCanReplacing;    ///< Plugin has processReplacing
            unsigned int        m_uiVstVersion;        ///< VST-Version of plugin
            bool                m_bPluginRunning;    ///< indicates if the plugin is currently running

            float**             m_ppInputs;         ///< Allocated memory for the inputs of the plugin
            float**             m_ppOutputs;        ///< Allocated memory for the outputs of the plugin
            VstInt32            m_uiNumInputs;      ///< Number of inputs used by the plugin
            VstInt32            m_uiNumOutputs;     ///< Number of outputs used by the plugin

            /**
            these variables have to be non-local to have plugins
            access them after the send-method has been left
            TODO: maybe some better solution?
            */
            VstEvents           events;
            VstMidiEvent        midiEvent = {};
        };
    }
}

#endif
