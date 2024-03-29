#include "gtest/gtest.h"
#include <Effect/Overdrive.h>

using namespace eLibV2::Effect;

TEST(TestEffectOverdrive, ClipLevel)
{
    Overdrive overdrive;

    overdrive.setLevel(-10.0);
    ASSERT_EQ(0.0, overdrive.getLevel());
    overdrive.setLevel(20.0);
    ASSERT_EQ(20.0, overdrive.getLevel());
    overdrive.setLevel(100.0);
    ASSERT_EQ(100.0, overdrive.getLevel());
    overdrive.setLevel(200.0);
    ASSERT_EQ(100.0, overdrive.getLevel());
}

TEST(TestFxOverdrive, Process)
{
    // TODO create test case involving internal processing logic
}
