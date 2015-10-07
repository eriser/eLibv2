#include "gtest/gtest.h"
#include <Effect/SignalAdd.h>

using namespace eLibV2::Effect;

TEST(TestEffectSignalAdd, Process)
{
    SignalAdd adder;

    ASSERT_NEAR(-0.5, adder.Process2(-0.25, -0.25), 0.01);
    ASSERT_NEAR(0.5, adder.Process2(0.25, 0.25), 0.01);
    ASSERT_NEAR(0.0, adder.Process2(-0.25, 0.25), 0.01);
    ASSERT_NEAR(0.0, adder.Process2(0.25, -0.25), 0.01);

    ASSERT_NEAR(-0.86, adder.Process2(-0.8, -0.3), 0.01);
    ASSERT_NEAR(0.86, adder.Process2(0.8, 0.3), 0.01);
    ASSERT_NEAR(-0.5, adder.Process2(-0.8, 0.3), 0.01);
    ASSERT_NEAR(0.5, adder.Process2(0.8, -0.3), 0.01);

    ASSERT_NEAR(-1.0, adder.Process2(-1.0, -1.0), 0.01);
    ASSERT_NEAR(1.0, adder.Process2(1.0, 1.0), 0.01);
}
