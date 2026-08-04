#include "executor.h"
#include<unistd.h>//for(fork,execvp)
#include<sys/wait.h>//for(waitpid,WIFEXITED,WEXITSTATUS)
#include<cstdlib>//for(exit)
#include<iostream>

int Executor::execute(const std::vector<std::string>& args)const{
    if (args.empty()) return 0;
    std::vector<char*> argv;
    for(const auto& it:args){
        argv.emplace_back(const_cast<char*>(it.c_str()));
    }
    argv.emplace_back(nullptr);
    pid_t pid = fork();
    if(pid == 0){
        execvp(argv[0],argv.data());
        std::cerr << "posh: command not found: " << args[0] << "\n";
        exit(127);
    }
    
    int status;
    waitpid(pid,&status,0);
    if(WIFEXITED(status)){
        return WEXITSTATUS(status);
    }
    return 1;
    
}