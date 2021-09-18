#include "MyProject/Add.h"
#include "gtest/gtest.h"

TEST(FactorialTest, Negative) { ASSERT_DOUBLE_EQ(4, MyProject::Add(1, 3)); }

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
