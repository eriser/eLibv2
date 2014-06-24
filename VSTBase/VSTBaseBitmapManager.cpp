/*
 * VSTBaseBitmapManager
 */

#include <VSTBase/VSTBaseBitmapManager.h>

using namespace eLibV2;
using namespace eLibV2::VSTBase;

void VSTBaseBitmapManager::addBitmap(VstInt32 id, CBitmap *bitmap)
{
    ModuleLogger::print("%d", id);

    mBitmaps[id] = bitmap;
}

void VSTBaseBitmapManager::forgetBitmap(VstInt32 id)
{
    if (mBitmaps[id])
    {
        mBitmaps[id]->forget();
        mBitmaps[id] = 0;
    }
}
