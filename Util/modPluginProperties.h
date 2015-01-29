#ifndef PLUGINPROPERTIES_H_
#define PLUGINPROPERTIES_H_

#include <Util/modDefines.h>
#include <Util/Xml/modGenericNode.h>
#include <Util/Xml/modXmlParser.h>

#include <string>

namespace eLibV2
{
	namespace VSTBase
	{
		class PluginProperties
		{
			public:
				PluginProperties() { mSynth = false; mId = "NoId"; mVersion = 0x0815; }

				static PluginProperties loadFromXml(std::string filename)
				{
					PluginProperties props;
					XmlParser parser(filename);
					nodeVector *nodes = parser.getNodes();
					if (nodes)
					{
						ModuleLogger::setActive(true);
						ModuleLogger::print("%li elements", nodes->size());

						for (NodeVector::iterator nodeIt = nodes->begin(); nodeIt != nodes->end(); nodeIt++)
						{
							if ((*nodeIt)->getName() == "plugin")
							{
								for (StringMap::iterator attributeIt = (*nodeIt)->getAttributes().begin(); attributeIt != (*nodeIt)->getAttributes().end(); attributeIt++)
								{
									std::string attributeName = (*attributeIt).first;
									std::string attributeValue = (*attributeIt).second;

									if (attributeName == "id")
										props.setId(attributeValue);
									else if (attributeName == "version")
										props.setVersion(atol(attributeValue.c_str()));
									else if (attributeName == "inputs")
										props.setNumInputs(atol(attributeValue.c_str()));
									else if (attributeName == "outputs")
										props.setNumOutputs(atol(attributeValue.c_str()));
									else if (attributeName == "parameters")
										props.setNumOutputs(atol(attributeValue.c_str()));
									else if (attributeName == "programs")
										props.setNumOutputs(atol(attributeValue.c_str()));
									else if (attributeName == "synth")
									{
										if (attributeValue == "true")
											props.setSynth();
									}
								}
								break;
							}
						}
					}
					return props;
				}

				bool isSynth() { return mSynth; }
				bool canProcessReplacing() { return mCanProcessReplacing; }
				bool canDoubleReplacing() { return mCanDoubleReplacing; }
				int getNumPresets() { return mNumPresets; }
				int getNumParameters() { return mNumParameters; }
				int getNumInputs() { return mNumInputs; }
				int getNumOutputs() { return mNumOutputs; }
				std::string getId() { return mId; }
				long getIdAsLong()
				{
					const char *s = mId.c_str();
					long temp;

					temp = (s[0] << 24) + (s[1] << 16) + (s[2] << 8) + s[3];
					return temp;
				}

				int getVersion() { return mVersion; }

				void setSynth() { mSynth = true; }
				void setProcessReplacing() { mCanProcessReplacing = true; }
				void setDoubleReplacing() { mCanDoubleReplacing = true; }
				void setNumPresets(int numPresets) { mNumPresets = numPresets; }
				void setNumParameters(int numParameters) { mNumParameters = numParameters; }
				void setNumInputs(int numInputs) { mNumInputs = numInputs; }
				void setNumOutputs(int numOutputs) { mNumOutputs = numOutputs; }
				void setId(std::string id) { mId = id; }
				void setVersion(int version) { mVersion = version; }

			private:
				bool mSynth;
				bool mCanProcessReplacing, mCanDoubleReplacing;
				std::string mId;
				int mVersion;
				int mNumInputs, mNumOutputs;
				int mNumPresets, mNumParameters;
		};
	}
}

#endif