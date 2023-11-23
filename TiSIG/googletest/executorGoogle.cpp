#include "gtest/gtest.h"
#include "../test_db/executor.h"


TEST(ExecutorTest, test_exec){
    Executor executor;
    std::string result = executor.exec("echo 'hello world!'");
    EXPECT_EQ(result, "hello world!\n");
}
