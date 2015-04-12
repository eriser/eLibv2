#include "gtest/gtest.h"
#include <Effect/modClip.h>

using namespace eLibV2::Effect;

TEST(TestFxClip, ClipLevel)
{
    FxClip clip;
    clip.setClipLevel(1.0);

    ASSERT_EQ(1.0, clip.getClipLevel());
}

TEST(TestFxClip, ClipMode)
{
    FxClip clip;
    clip.setClipMode(ClipMode::CLIP_MODE_BOTH);

    ASSERT_EQ(ClipMode::CLIP_MODE_BOTH, clip.getClipMode());
}

TEST(TestFxClip, ProcessPositive)
{
    FxClip clip;
    clip.setClipLevel(1.0);
    clip.setClipMode(ClipMode::CLIP_MODE_POSITIVE);

    ASSERT_EQ(1.0, clip.Process(2.0));
}

TEST(TestFxClip, ProcessNegative)
{
    FxClip clip;
    clip.setClipLevel(1.0);
    clip.setClipMode(ClipMode::CLIP_MODE_NEGATIVE);

    ASSERT_EQ(-1.0, clip.Process(-2.0));
}

TEST(TestFxClip, ProcessBoth)
{
    FxClip clip;
    clip.setClipLevel(1.0);
    clip.setClipMode(ClipMode::CLIP_MODE_BOTH);

    ASSERT_EQ(1.0, clip.Process(2.0));
    ASSERT_EQ(-1.0, clip.Process(-2.0));
}

