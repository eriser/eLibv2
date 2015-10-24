#include <VSTBase/VSTBaseBitmapManager.h>

using namespace eLibV2;
using namespace eLibV2::Util;
using namespace eLibV2::VSTBase;

VSTBaseBitmapManager::~VSTBaseBitmapManager()
{
    clearAll();
}

void VSTBaseBitmapManager::clearAll()
{
    if (mBitmaps.size())
    {
        for (std::map<std::string, CBitmap *>::iterator it = mBitmaps.begin(); it != mBitmaps.end(); it++)
            it->second->forget();
        mBitmaps.clear();
    }

    if (mBackgroundBitmaps.size())
    {
        for (std::map<unsigned int, CBitmap *>::iterator it = mBackgroundBitmaps.begin(); it != mBackgroundBitmaps.end(); it++)
            it->second->forget();
        mBackgroundBitmaps.clear();
    }
}

void VSTBaseBitmapManager::addBackgroundBitmap(const unsigned int pageIndex, CBitmap *bitmap)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseBitmapManager::addBitmap: add id: %d", pageIndex);
    mBackgroundBitmaps[pageIndex] = bitmap;
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseBitmapManager::addBitmap: size: %d", mBitmaps.size());
}

void VSTBaseBitmapManager::addBitmap(const std::string bitmapId, CBitmap *bitmap)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseBitmapManager::addBitmap: add id: %d", bitmapId);
    mBitmaps[bitmapId] = bitmap;
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseBitmapManager::addBitmap: size: %d", mBitmaps.size());
}

void VSTBaseBitmapManager::forgetBitmap(const std::string bitmapId)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseBitmapManager::forgetBitmap: erase id: %d", bitmapId);
    if (mBitmaps[bitmapId])
    {
        mBitmaps[bitmapId]->forget();
        mBitmaps.erase(bitmapId);
    }
    ModuleLogger::print(LOG_CLASS_VSTBASE, "VSTBaseBitmapManager::forgetBitmap: size: %d", mBitmaps.size());
}
