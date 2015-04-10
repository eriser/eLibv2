#include "gtest/gtest.h"
#include <Effect/modDither.h>

using namespace eLibV2;
using namespace eLibV2::Effect;

TEST(TestFxDither, BitSize)
{
    FxDither dither;

    dither.setBitsize(12);
    ASSERT_EQ(12, dither.getBitsize());
}

TEST(TestFxDither, SampleRate)
{
    FxDither dither;

    dither.setSamplerate(22050.0);
    ASSERT_EQ(22050.0, dither.getSamplerate());
}
