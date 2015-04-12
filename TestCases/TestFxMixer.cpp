#include "gtest/gtest.h"
#include <Effect/modMixer.h>

using namespace eLibV2::Effect;

TEST(TestFxMixer, Mode)
{
    FxMixer mixer;
    mixer.setMode(MixerMode::MIXER_MODE_ADD);

    ASSERT_EQ(MixerMode::MIXER_MODE_ADD, mixer.getMode());
}

TEST(TestFxMixer, Volume)
{
    FxMixer mixer;
    mixer.setVolume(0.345);

    ASSERT_EQ(0.345, mixer.getVolume());
}

TEST(TestFxMixer, ProcessModeAdd)
{
    FxMixer mixer;
    mixer.setMode(MixerMode::MIXER_MODE_ADD);

    ASSERT_EQ(0.579, mixer.Process2(0.123, 0.456));
}

TEST(TestFxMixer, ProcessModeSubtract)
{
    FxMixer mixer;
    mixer.setMode(MixerMode::MIXER_MODE_SUBTRACT);

    ASSERT_EQ(0.333, mixer.Process2(0.456, 0.123));
}

TEST(TestFxMixer, ProcessModeInvSubtract)
{
    FxMixer mixer;
    mixer.setMode(MixerMode::MIXER_MODE_INVSUBTRACT);

    ASSERT_EQ(0.333, mixer.Process2(0.123, 0.456));
}
