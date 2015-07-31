#include <VSTBase/VSTBaseBitmapManager.h>

using namespace eLibV2;
using namespace eLibV2::Util;
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
    ModuleLogger::print(LOG_CLASS_VST, "VSTBaseBitmapManager::addBitmap: add id: %d", bitmapId);
    mBitmaps[bitmapId] = bitmap;
    ModuleLogger::print(LOG_CLASS_VST, "VSTBaseBitmapManager::addBitmap: size: %d", mBitmaps.size());
}

void VSTBaseBitmapManager::forgetBitmap(const VstInt32 bitmapId)
{
    ModuleLogger::print(LOG_CLASS_VST, "VSTBaseBitmapManager::forgetBitmap: erase id: %d", bitmapId);
    if (mBitmaps[bitmapId])
    {
        mBitmaps[bitmapId]->forget();
        mBitmaps.erase(bitmapId);
    }
    ModuleLogger::print(LOG_CLASS_VST, "VSTBaseBitmapManager::forgetBitmap: size: %d", mBitmaps.size());
}
