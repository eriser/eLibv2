#include "gtest/gtest.h"
#include <Effect/SignalAdd.h>

using namespace eLibV2::Effect;

TEST(TestFxSignalAdd, Process)
{
    FxSignalAdd adder;

    ASSERT_NEAR(-0.43, adder.Process(-0.25, -0.25), 0.01);
    ASSERT_NEAR(0.56, adder.Process(0.25, 0.25), 0.01);
    ASSERT_NEAR(-0.07, adder.Process(-0.25, 0.25), 0.01);
    ASSERT_NEAR(-0.07, adder.Process(0.25, -0.25), 0.01);
    ASSERT_NEAR(-1.0, adder.Process(-1.0, -1.0), 0.01);
    ASSERT_NEAR(3.0, adder.Process(1.0, 1.0), 0.01);
}
