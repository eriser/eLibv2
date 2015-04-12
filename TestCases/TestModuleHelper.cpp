#include "gtest/gtest.h"
#include <Util/modHelper.h>

using namespace eLibV2::Util;

TEST(TestModuleHelper, minval)
{
    ASSERT_EQ(1.0, ModuleHelper::minval(0.0, 1.0));
    ASSERT_EQ(0.0, ModuleHelper::minval(0.0, -1.0));
    ASSERT_EQ(1.0, ModuleHelper::minval(1.0, 0.0));
    ASSERT_EQ(0.0, ModuleHelper::minval(-1.0, 0.0));
}

TEST(TestModuleHelper, maxval)
{
    ASSERT_EQ(0.0, ModuleHelper::maxval(0.0, 1.0));
    ASSERT_EQ(-1.0, ModuleHelper::maxval(0.0, -1.0));
    ASSERT_EQ(0.0, ModuleHelper::maxval(1.0, 0.0));
    ASSERT_EQ(-1.0, ModuleHelper::maxval(-1.0, 0.0));
}

TEST(TestModuleHelper, clamp)
{
    // within interval
    ASSERT_EQ(0.0, ModuleHelper::clamp(0.0, -1.0, 1.0));
    ASSERT_EQ(0.0, ModuleHelper::clamp(0.0, 0.0, 1.0));
    ASSERT_EQ(0.0, ModuleHelper::clamp(0.0, -1.0, 0.0));
    // out of interval
    ASSERT_EQ(-1.0, ModuleHelper::clamp(-2.0, -1.0, 1.0));
    ASSERT_EQ(1.0, ModuleHelper::clamp(2.0, -1.0, 1.0));
    ASSERT_EQ(0.0, ModuleHelper::clamp(-2.0, 0.0, 1.0));
    ASSERT_EQ(1.0, ModuleHelper::clamp(2.0, 0.0, 1.0));
    ASSERT_EQ(-1.0, ModuleHelper::clamp(-2.0, -1.0, 0.0));
    ASSERT_EQ(0.0, ModuleHelper::clamp(2.0, -1.0, 0.0));
}
