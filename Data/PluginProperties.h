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

#if defined(WIN32)
            static PluginProperties loadFromResource(HINSTANCE instance, UInt16 resourceId)
            {
                PluginProperties res;
                HRSRC hResource = NULL;
                HGLOBAL hRData = NULL;
                void *pRData;

                hResource = FindResource(instance, MAKEINTRESOURCE(resourceId), "RAW");
                if (hResource)
                {
                    UInt16 bufferSize = SizeofResource(instance, hResource);
                    hRData = LoadResource(instance, hResource);
                    if (hRData)
                    {
                        pRData = LockResource(hRData);
                        XmlParser parser(pRData, bufferSize);
                        nodeVector *nodes = parser.getNodes();

                        res = load(nodes);
                    }
                }
                return res;
            }
#endif

            static PluginProperties loadFromXml(std::string filename)
            {
                XmlParser parser(filename);
                nodeVector* nodes = parser.getNodes();

                return load(nodes);
            }

            static PluginProperties load(nodeVector* nodes)
            {
                PluginProperties props;
                if (nodes)
                {
                    for (NodeVector::iterator nodeIt = nodes->begin(); nodeIt != nodes->end(); nodeIt++)
                    {
                        if ((*nodeIt)->getName() == "plugin")
                        {
                            StringMap attributes = (*nodeIt)->getAttributes();
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

            bool isSynth() const { return mSynth; }
            bool hasEditor() const { return mEditor; }
            bool canProcessReplacing() const { return mCanProcessReplacing; }
            bool canDoubleReplacing() const { return mCanDoubleReplacing; }
            UInt16 getNumPrograms() const { return mNumPrograms; }
            UInt16 getNumParameters() const { return mNumParameters; }
            UInt8 getNumInputs() const { return mNumInputs; }
            UInt8 getNumOutputs() const { return mNumOutputs; }
            std::string getId() const { return mId; }
            std::string getName() const { return mName; }
            std::string getVendor() const { return mVendor; }
            SInt32 getIdAsLong()
            {
                const char *s = mId.c_str();
                SInt32 temp;

                temp = (s[0] << 24) + (s[1] << 16) + (s[2] << 8) + s[3];
                return temp;
            }

            UInt16 getVersion() const { return mVersion; }

        private:
            void setNumPrograms(UInt16 numPrograms) { mNumPrograms = numPrograms; }
            void setNumParameters(UInt16 numParameters) { mNumParameters = numParameters; }
            void setNumInputs(UInt8 numInputs) { mNumInputs = numInputs; }
            void setNumOutputs(UInt8 numOutputs) { mNumOutputs = numOutputs; }
            void setId(std::string id) { mId = id; }
            void setName(std::string name) { mName = name; }
            void setVendor(std::string vendor) { mVendor = vendor; }
            void setVersion(UInt16 version) { mVersion = version; }
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
            UInt16 mVersion;
            UInt8 mNumInputs, mNumOutputs;
            UInt16 mNumPrograms, mNumParameters;
        };
    }
}

#endif
