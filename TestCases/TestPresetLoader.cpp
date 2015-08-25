#include "gtest/gtest.h"
#include <Loader/PresetLoader.h>

#include "TestGlobals.h"

using namespace eLibV2::Loader;

TEST(TestPresetLoader, loadfxb)
{
    PresetLoader loader;
    ASSERT_EQ(loader.Load(TestDataFolder + "presetloader.fxb"), PresetLoader::ERR_PRESET_NO_ERROR);
}

TEST(TestPresetLoader, loadfxp)
{
    PresetLoader loader;
    ASSERT_EQ(loader.Load(TestDataFolder + "presetloader.fxp"), PresetLoader::ERR_PRESET_NO_ERROR);
}

TEST(TestPresetLoader, loadtxt)
{
    PresetLoader loader;
    ASSERT_EQ(loader.Load(TestDataFolder + "presetloader.txt"), PresetLoader::ERR_PRESET_NO_ERROR);
}

TEST(TestPresetLoader, loadunknown)
{
    PresetLoader loader;
    ASSERT_EQ(loader.Load(TestDataFolder + "presetloader.err"), PresetLoader::ERR_PRESET_UNKNOWN_TYPE);
}
