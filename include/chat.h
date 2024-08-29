#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <memory.h>
#include "logger.h"



class Chat {
private:
            Logger logger;
            sql::Driver *driver;
            std::unique_ptr<sql::Connection> con;
public:
            Chat();
            ~Chat();
            bool registration(const std::string& login, const std::string& password, const std::string&);
            bool sign_in(const std::string& login, const std::string& password);
            bool sendMessage(const std::string& sender, const std::string& receiver, const std::string& text);
            std::vector<std::string> getMessages(const std::string& reciever);
};