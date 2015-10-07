#include "gtest/gtest.h"
#include <Effect/Mixer.h>

using namespace eLibV2::Effect;

TEST(TestEffectMixer, Mode)
{
    Mixer mixer;
    mixer.setMode(Mixer::MixerMode::MIXER_MODE_ADD);

    ASSERT_EQ(Mixer::MixerMode::MIXER_MODE_ADD, mixer.getMode());
}

TEST(TestEffectMixer, Volume)
{
    Mixer mixer;
    mixer.setVolume(0.345);

    ASSERT_EQ(0.345, mixer.getVolume());
}

TEST(TestEffectMixer, ProcessModeAdd)
{
    Mixer mixer;
    mixer.setMode(Mixer::MixerMode::MIXER_MODE_ADD);

    ASSERT_EQ(0.579, mixer.Process2(0.123, 0.456));
    ASSERT_EQ(0.333, mixer.Process2(-0.123, 0.456));
    ASSERT_EQ(-0.333, mixer.Process2(0.123, -0.456));
    ASSERT_EQ(-0.579, mixer.Process2(-0.123, -0.456));
}

TEST(TestEffectMixer, ProcessModeSubtract)
{
    Mixer mixer;
    mixer.setMode(Mixer::MixerMode::MIXER_MODE_SUBTRACT);

    ASSERT_EQ(0.333, mixer.Process2(0.456, 0.123));
}

TEST(TestEffectMixer, ProcessModeInvSubtract)
{
    Mixer mixer;
    mixer.setMode(Mixer::MixerMode::MIXER_MODE_INVSUBTRACT);

    ASSERT_EQ(0.333, mixer.Process2(0.123, 0.456));
}
