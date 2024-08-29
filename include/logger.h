#pragma once
#include <iostream>
#include <thread>
#include <fstream>
#include <shared_mutex>


class Logger{
    public:
    Logger();
    ~Logger();
    void setLog(const std::string& str);
    void getLog();

    private:
    std::fstream logFile;
    std::shared_mutex shared_mutex;
};