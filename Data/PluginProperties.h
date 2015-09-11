#ifndef PLUGINPROPERTIES_H_
#define PLUGINPROPERTIES_H_

#include <Util/Defines.h>
#include <Util/Xml/GenericNode.h>
#include <Util/Xml/XmlParser.h>

#include <string>

using namespace eLibV2::Util;
using namespace eLibV2::XML;

namespace eLibV2
{
    namespace Data
    {
        class PluginProperties
        {
        public:
            PluginProperties() :
                mSynth(false),
                mEditor(false),
                mId("NoId"),
                mName("NoName"),
                mVendor("NoVendor"),
                mVersion(0x0815),
                mNumInputs(2),
                mNumOutputs(2),
                mNumParameters(0),
                mNumPrograms(0)
            {}

            static PluginProperties loadFromXml(std::string filename)
            {
                PluginProperties props;
                XmlParser parser(filename);
                nodeVector *nodes = parser.getNodes();
                if (nodes)
                {
                    for (NodeVector::iterator nodeIt = nodes->begin(); nodeIt != nodes->end(); nodeIt++)
                    {
                        if ((*nodeIt)->getName() == "plugin")
                        {
                            StringMap attributes = (*nodeIt)->getAttributes();
                            ModuleLogger::print(LOG_CLASS_DATA, "%li attributes", attributes.size());
                            for (StringMap::iterator attributeIt = attributes.begin(); attributeIt != attributes.end(); attributeIt++)
                            {
                                std::string attributeName = (*attributeIt).first;
                                std::string attributeValue = (*attributeIt).second;

                                if (attributeName == "id")
                                    props.setId(attributeValue);
                                else if (attributeName == "name")
                                    props.setName(attributeValue);
                                else if (attributeName == "vendor")
                                    props.setVendor(attributeValue);
                                else if (attributeName == "version")
                                    props.setVersion(atol(attributeValue.c_str()));
                                else if (attributeName == "inputs")
                                    props.setNumInputs(atol(attributeValue.c_str()));
                                else if (attributeName == "outputs")
                                    props.setNumOutputs(atol(attributeValue.c_str()));
                                else if (attributeName == "parameters")
                                    props.setNumParameters(atol(attributeValue.c_str()));
                                else if (attributeName == "programs")
                                    props.setNumPrograms(atol(attributeValue.c_str()));
                                else if (attributeName == "synth")
                                    props.setSynth((attributeValue == "true") ? true : false);
                                else if (attributeName == "editor")
                                    props.setEditor((attributeValue == "true") ? true : false);
                                else if (attributeName == "double")
                                    props.setDoubleReplacing((attributeValue == "true") ? true : false);
                            }
                        }
                    }
                }
                return props;
            }

            bool isSynth() { return mSynth; }
            bool hasEditor() { return mEditor; }
            bool canProcessReplacing() { return mCanProcessReplacing; }
            bool canDoubleReplacing() { return mCanDoubleReplacing; }
            int getNumPrograms() { return mNumPrograms; }
            int getNumParameters() { return mNumParameters; }
            int getNumInputs() { return mNumInputs; }
            int getNumOutputs() { return mNumOutputs; }
            std::string getId() { return mId; }
            std::string getName() { return mName; }
            std::string getVendor() { return mVendor; }
            long getIdAsLong()
            {
                const char *s = mId.c_str();
                long temp;

                temp = (s[0] << 24) + (s[1] << 16) + (s[2] << 8) + s[3];
                return temp;
            }

            int getVersion() { return mVersion; }

        private:
            void setNumPrograms(int numPrograms) { mNumPrograms = numPrograms; }
            void setNumParameters(int numParameters) { mNumParameters = numParameters; }
            void setNumInputs(int numInputs) { mNumInputs = numInputs; }
            void setNumOutputs(int numOutputs) { mNumOutputs = numOutputs; }
            void setId(std::string id) { mId = id; }
            void setName(std::string name) { mName = name; }
            void setVendor(std::string vendor) { mVendor = vendor; }
            void setVersion(int version) { mVersion = version; }
            void setSynth(bool synth) { mSynth = synth; }
            void setEditor(bool active) { mEditor = active; }
            void setProcessReplacing(bool processReplacing) { mCanProcessReplacing = processReplacing; }
            void setDoubleReplacing(bool doubleReplacing) { mCanDoubleReplacing = doubleReplacing; }

        private:
            bool mSynth;
            bool mCanProcessReplacing, mCanDoubleReplacing;
            bool mEditor;
            std::string mId;
            std::string mName;
            std::string mVendor;
            int mVersion;
            int mNumInputs, mNumOutputs;
            int mNumPrograms, mNumParameters;
        };
    }
}

#endif
