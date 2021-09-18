#include "MyProject/Subtract.h"
#include "gtest/gtest.h"

TEST(MyProjectSubtraction, Subtraction) {
  ASSERT_DOUBLE_EQ(-2, MyProject::Subtract(1, 3));
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
