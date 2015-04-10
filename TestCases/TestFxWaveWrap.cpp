#include "gtest/gtest.h"
#include <Effect/modWaveWrap.h>

using namespace eLibV2;
using namespace eLibV2::Effect;

TEST(TestFxWaveWrap, WrapLevel)
{
    FxWaveWrap wavewrap;

    wavewrap.setWrapLevel(1234.56);
    ASSERT_EQ(1234.56, wavewrap.getWrapLevel());
}
