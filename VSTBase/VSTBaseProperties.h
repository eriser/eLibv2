#ifndef VSTBASEPROPERTIES_H_
#define VSTBASEPROPERTIES_H_

#include <string>

namespace eLibV2
{
    namespace VSTBase
    {
        class VSTBaseProperties
        {
            public:
                VSTBaseProperties() { mSynth = false; mId = "NoId"; mVersion = 0x0815; }

                bool isSynth() { return mSynth; }
                VstInt16 getNumPresets() { return mNumPresets; }
                VstInt16 getNumParameters() { return mNumParameters; }
                VstInt16 getNumInputs() { return mNumInputs; }
                VstInt16 getNumOutputs() { return mNumOutputs; }
                std::string getPluginId() { return mId; }
                VstInt32 getPluginIdAsVstInt32()
                {
                    const char *s = mId.c_str();
                    VstInt32 temp;

                    temp = (s[0] << 24) + (s[1] << 16) + (s[2] << 8) + s[3];
                    return temp;
                }

                VstInt16 getVersion() { return mVersion; }

                void setSynth() { mSynth = true; }
                void setNumPresets(VstInt16 numPresets) { mNumPresets = numPresets; }
                void setNumParameters(VstInt16 numParameters) { mNumParameters = numParameters; }
                void setNumInputs(VstInt16 numInputs) { mNumInputs = numInputs; }
                void setNumOutputs(VstInt16 numOutputs) { mNumOutputs = numOutputs; }
                void setPluginId(std::string id) { mId = id; }
                void setVersion(VstInt16 version) { mVersion = version; }

            private:
                bool mSynth;
                std::string mId;
                VstInt16 mVersion;
                VstInt16 mNumInputs;
                VstInt16 mNumOutputs;
                VstInt16 mNumPresets;
                VstInt16 mNumParameters;
        };
    }
}

#endif