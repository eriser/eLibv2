#include "gtest/gtest.h"
#include <Effect/WaveWrap.h>

using namespace eLibV2::Effect;

/// @todo create testcase for process

TEST(TestEffectWaveWrap, WrapLevel)
{
    WaveWrap wavewrap;

    wavewrap.setWrapLevel(1.0);
    ASSERT_EQ(1.0, wavewrap.getWrapLevel());
    wavewrap.setWrapLevel(10.0);
    ASSERT_EQ(10.0, wavewrap.getWrapLevel());
    wavewrap.setWrapLevel(-1.0);
    ASSERT_EQ(1.0, wavewrap.getWrapLevel());
    wavewrap.setWrapLevel(50.0);
    ASSERT_EQ(10.0, wavewrap.getWrapLevel());
}
