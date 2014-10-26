/*
 * VSTBaseBitmapManager
 */

#include <VSTBase/VSTBaseBitmapManager.h>

using namespace eLibV2;
using namespace eLibV2::VSTBase;

VSTBaseBitmapManager::~VSTBaseBitmapManager()
{
    if (mBitmaps.size())
    {
        for (std::map<VstInt32, CBitmap *>::iterator it = mBitmaps.begin(); it != mBitmaps.end(); it++)
        {
            it->second->forget();
        }
    }
    mBitmaps.clear();
}

void VSTBaseBitmapManager::addBitmap(const VstInt32 bitmapId, CBitmap *bitmap)
{
    ModuleLogger::print("add id: %d", bitmapId);
    mBitmaps[bitmapId] = bitmap;
    ModuleLogger::print("size: %d", mBitmaps.size());
}

void VSTBaseBitmapManager::forgetBitmap(const VstInt32 bitmapId)
{
    ModuleLogger::print("erase id: %d", bitmapId);
    if (mBitmaps[bitmapId])
    {
        mBitmaps[bitmapId]->forget();
        mBitmaps.erase(bitmapId);
    }
    ModuleLogger::print("size: %d", mBitmaps.size());
}
