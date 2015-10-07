#include "gtest/gtest.h"
#include <Effect/Clip.h>

using namespace eLibV2::Effect;

TEST(TestEffectClip, ClipLevel)
{
    Clip clip;
    clip.setClipLevel(1.0);

    ASSERT_EQ(1.0, clip.getClipLevel());
}

TEST(TestEffectClip, ClipMode)
{
    Clip clip;
    clip.setClipMode(Clip::ClipMode::CLIP_MODE_BOTH);

    ASSERT_EQ(Clip::ClipMode::CLIP_MODE_BOTH, clip.getClipMode());
}

TEST(TestEffectClip, ProcessPositive)
{
    Clip clip;
    clip.setClipLevel(1.0);
    clip.setClipMode(Clip::ClipMode::CLIP_MODE_POSITIVE);

    ASSERT_EQ(0.5, clip.Process(0.5));
    ASSERT_EQ(1.0, clip.Process(2.0));
}

TEST(TestEffectClip, ProcessNegative)
{
    Clip clip;
    clip.setClipLevel(1.0);
    clip.setClipMode(Clip::ClipMode::CLIP_MODE_NEGATIVE);

    ASSERT_EQ(-0.5, clip.Process(-0.5));
    ASSERT_EQ(-1.0, clip.Process(-2.0));
}

TEST(TestEffectClip, ProcessBoth)
{
    Clip clip;
    clip.setClipLevel(1.0);
    clip.setClipMode(Clip::ClipMode::CLIP_MODE_BOTH);

    ASSERT_EQ(0.5, clip.Process(0.5));
    ASSERT_EQ(-0.5, clip.Process(-0.5));
    ASSERT_EQ(1.0, clip.Process(2.0));
    ASSERT_EQ(-1.0, clip.Process(-2.0));
}

