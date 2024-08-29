#include "logger.h"

Logger::Logger()
{
    this->logFile.open("log.txt");
}

Logger::~Logger()
{
    this->logFile.close();
}

void Logger::setLog(const std::string& str)
{
    shared_mutex.lock();
    this->logFile << str << std::endl;
    shared_mutex.unlock();
}

void Logger::getLog()  
{
    if(!this->logFile.is_open()){
        std::cout << "Error open log.txt" << std::endl;
        exit(1);
    }
    std::string str;
    shared_mutex.lock_shared();
    while(this->logFile.good()){
        this->logFile >> str; 
    }
    if(this->logFile.eof()){
        std::cout << "End of File" << std::endl;
    }
    shared_mutex.unlock_shared();
}