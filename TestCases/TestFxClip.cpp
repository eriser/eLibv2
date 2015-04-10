#include "gtest/gtest.h"
#include <Effect/modClip.h>

using namespace eLibV2;
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
    clip.setClipMode(ClipModes::CLIP_MODE_BOTH);

    ASSERT_EQ(ClipModes::CLIP_MODE_BOTH, clip.getClipMode());
}
