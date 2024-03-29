#include "gtest/gtest.h"
#include <Effect/Dither.h>

using namespace eLibV2::Effect;

TEST(TestEffectDither, BitSize)
{
    Dither dither;

    dither.setBitsize(DITHER_BITSIZE_DEFAULT);
    ASSERT_EQ(DITHER_BITSIZE_DEFAULT, dither.getBitsize());
}

TEST(TestEffectDither, Process2Bits)
{
    Dither dither;

    dither.setBitsize(2);
    ASSERT_EQ(1.0, dither.Process(1.0));
    ASSERT_EQ(-1.0, dither.Process(-1.0));
    ASSERT_NEAR(0.0, dither.Process(0.187), 0.01);
    ASSERT_NEAR(0.0, dither.Process(-0.187), 0.01);
    ASSERT_NEAR(0.5, dither.Process(0.5), 0.01);
    ASSERT_NEAR(-0.5, dither.Process(-0.5), 0.01);
    ASSERT_NEAR(0.75, dither.Process(0.885), 0.01);
    ASSERT_NEAR(-0.75, dither.Process(-0.885), 0.01);
}

TEST(TestEffectDither, Process4Bits)
{
    Dither dither;

    dither.setBitsize(4);
    ASSERT_EQ(1.0, dither.Process(1.0));
    ASSERT_EQ(-1.0, dither.Process(-1.0));
    ASSERT_NEAR(0.125, dither.Process(0.187), 0.01);
    ASSERT_NEAR(-0.125, dither.Process(-0.187), 0.01);
    ASSERT_NEAR(0.5, dither.Process(0.5), 0.01);
    ASSERT_NEAR(-0.5, dither.Process(-0.5), 0.01);
    ASSERT_NEAR(0.875, dither.Process(0.885), 0.01);
    ASSERT_NEAR(-0.875, dither.Process(-0.885), 0.01);
}

TEST(TestEffectDither, Process8Bits)
{
    Dither dither;

    dither.setBitsize(8);
    ASSERT_EQ(1.0, dither.Process(1.0));
    ASSERT_EQ(-1.0, dither.Process(-1.0));
    ASSERT_NEAR(0.183, dither.Process(0.187), 0.01);
    ASSERT_NEAR(-0.183, dither.Process(-0.187), 0.01);
    ASSERT_NEAR(0.5, dither.Process(0.5), 0.01);
    ASSERT_NEAR(-0.5, dither.Process(-0.5), 0.01);
    ASSERT_NEAR(0.883, dither.Process(0.885), 0.01);
    ASSERT_NEAR(-0.883, dither.Process(-0.885), 0.01);
}
