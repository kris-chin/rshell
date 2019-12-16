#ifndef __TEST_COMMAND_HPP__
#define __TEST_COMMAND_HPP__

#include "gtest/gtest.h"

#include "../header/Word.hpp"
#include "../header/Command.hpp"
#include "../header/Arg.hpp"
#include "../header/Exec.hpp"

#include <vector>

using namespace std;

TEST(EchoTest, EchoSingleArg) {
    vector<Word*> testSequence;

    Exec* test_exec = new Exec("echo");
    testSequence.push_back(test_exec);

    Arg* test_arg = new Arg("test");
    testSequence.push_back(test_arg);
    
    Command* test_command = new Command(testSequence);
    
    test_command->execute();
    EXPECT_EQ(1, 1);
}

TEST(catTest, catMainDirFilet) {
    vector<Word*> testSequence;

    Exec* test_exec = new Exec("cat");
    testSequence.push_back(test_exec);

    Arg* test_arg = new Arg("test_cat.txt");
    testSequence.push_back(test_arg);
    
    Command* test_command = new Command(testSequence);
    
    test_command->execute();
    EXPECT_EQ(1, 1);
}

TEST(lsTest, lsWorkingDir) {
    vector<Word*> testSequence;

    Exec* test_exec = new Exec("ls");
    testSequence.push_back(test_exec);

    Command* test_command = new Command(testSequence);
    
    test_command->execute();
    EXPECT_EQ(1, 1);
}

TEST(gitTest, gitStatus) {
    vector<Word*> testSequence;

    Exec* test_exec = new Exec("git");
    testSequence.push_back(test_exec);

    Arg* test_arg = new Arg("status");
    testSequence.push_back(test_arg);

    Command* test_command = new Command(testSequence);
    
    test_command->execute();
    EXPECT_EQ(1, 1);
}

#endif //__TEST_COMMAND_HPP__
