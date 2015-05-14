#ifndef __PLUGINHOST_H__
#define __PLUGINHOST_H__

#include "aeffectx.h"

#include "PluginInterface.h"
#include <map>
#include <vector>

#define kBlockSize 512
#define kSampleRate 44000.f

namespace eLibV2
{
    namespace Host
    {
        typedef std::vector<std::string> StringList;
        typedef std::map<std::string, PluginInterface*> PluginMap;
        typedef std::vector<PluginInterface*> PluginInterfaceList;

        class PluginHost
        {
        public:
            PluginHost();
            ~PluginHost() {}

            bool OpenPlugin(std::string fileName);
            void CloseAll();
            void StartAll();
            void StopAll();

            PluginInterface *GetPluginByID(std::string pluginID);
            PluginInterface *GetPluginByIndex(int pluginIndex);
            PluginInterfaceList GetLoadedPlugins();

            void InsertMidiEvent(int channel, int status, int data1, int data2);

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

        private:
            PluginInterfaceList GetPluginsForMidiChannel(int channel);
            void ClosePlugin(std::string pluginID);

        private:
            PluginMap m_LoadedPlugins;
            unsigned int m_NumLoadedPlugins;
        };
    }
}
#endif
