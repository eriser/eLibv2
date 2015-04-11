#include "gtest/gtest.h"
#include <Effect/modWaveWrap.h>

using namespace eLibV2::Effect;

/// @todo create testcase for process

TEST(TestFxWaveWrap, WrapLevel)
{
    FxWaveWrap wavewrap;

    wavewrap.setWrapLevel(1.0);
    ASSERT_EQ(1.0, wavewrap.getWrapLevel());
    wavewrap.setWrapLevel(10.0);
    ASSERT_EQ(10.0, wavewrap.getWrapLevel());
    wavewrap.setWrapLevel(-1.0);
    ASSERT_EQ(1.0, wavewrap.getWrapLevel());
    wavewrap.setWrapLevel(50.0);
    ASSERT_EQ(10.0, wavewrap.getWrapLevel());
}