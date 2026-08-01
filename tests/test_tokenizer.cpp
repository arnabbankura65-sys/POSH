#include "tokenizer.h"
#include "test_helpers.h"
#include<iostream>

int main(){

    Tokenizer t;
    
    EXPECT_EQ(t.tokenize("ls -la /tmp"),(std::vector<std::string>{"ls", "-la", "/tmp"}));
    EXPECT_EQ(t.tokenize("  ls  -la  "),(std::vector<std::string>{"ls", "-la"}));
    EXPECT_EQ(t.tokenize(""),(std::vector<std::string>{}));
    EXPECT_EQ(t.tokenize("ls\t-la"), (std::vector<std::string>{"ls", "-la"}));
    EXPECT_EQ(t.tokenize("echo \"hello world\""), (std::vector<std::string>{"echo", "hello world"}));
    EXPECT_EQ(t.tokenize("echo hello\\ world"), (std::vector<std::string>{"echo", "hello world"}));
    EXPECT_EQ(t.tokenize("   "), (std::vector<std::string>{}));
    EXPECT_EQ(t.tokenize("  echo \"a b\"  c  "), (std::vector<std::string>{"echo", "a b", "c"}));
    
    std::cout << "Tests: " << tests_run << ", Failed: " << tests_failed << "\n";
    return tests_failed == 0 ? 0 : 1;
    
}