/*
 * VSTBaseEditor
 */

#include <VSTBase/VSTBaseEditorProperties.h>

using namespace eLibV2;
using namespace eLibV2::VSTBase;

VSTBaseEditorProperties::VSTBaseEditorProperties()
{
}

void VSTBaseEditorProperties::addBackgroundBitmap(const VstInt32 bitmapId)
{
    mBackgroundBitmaps.push_back(bitmapId);
}

void VSTBaseEditorProperties::addControlBitmap(const VstInt32 bitmapId)
{
    mControlBitmaps.push_back(bitmapId);
}
