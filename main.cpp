#include <audioeffectx.h>
#include <eLib2.h>

using namespace eLibV2;
using namespace eLibV2::VSTBase;

#define kNumParams 16
#define kNumPrograms 128

//------------------------------------------------------------------------
class VSTBaseTest : public VSTBase::VSTBaseClass
{
public:
    VSTBaseTest(audioMasterCallback audioMaster, VstInt32 numPresets, VstInt32 numParameters, VSTBase::PluginProperties properties) : VSTBaseClass(audioMaster, numPresets, numParameters, properties) {}
    virtual ~VSTBaseTest() {}

    void setParameterInvoked(VstInt32 index, float value) {}
    void process(double in1, double in2, double *out1, double *out2) {}
};

VSTBaseTest *cVSTBaseTest;

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    PluginProperties props;
    XmlParser parser("E:\\test.xml");

    nodeVector *nodes = parser.getNodes();
    if (nodes)
    {
        props.createFromNodeVector(*nodes);
        ModuleLogger::print("%li elements", nodes->size());
    }
    cVSTBaseTest = new VSTBaseTest(audioMaster, props.getNumPresets(), props.getNumParameters(), props);
    return cVSTBaseTest;
}

int main(void)
{
    return 0;
}