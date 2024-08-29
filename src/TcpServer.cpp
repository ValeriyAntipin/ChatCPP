#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <thread>
#include "chat.h"
#include "logger.h"

#include <mysql/mysql.h>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <memory.h>

Chat chat;

void commandsClient(int client_socket){
    char buffer[1024];
    while(true){
        memset(buffer, 0, 1024);
        int bytes_read = read(client_socket, buffer, 1024);
        if(bytes_read <= 0)
            break;
        std::cout << "file were get in func commandClient" << std::endl;
        std::string command(buffer);
        std::string response;

        if(command.rfind("REGISTR ", 0) == 0) {
            std::istringstream iss(command.substr(8));
            std::string login;
            std::string password;
            std::string name;
            if(iss >> login >> password >> name){
                if(chat.registration(login, password, name))
                    response = "Registered\n";
                else
                    response = "Registration error\n";
            }
            else 
                response = "Invalid input format\n";
        }

        else if(command.rfind("LOGIN ", 0) == 0) {
            std::istringstream iss(command.substr(6));
            std::string login;
            std::string password;
            if(iss >> login >> password){
                if(chat.sign_in(login, password))
                    response = "Sign in done\n";
                else 
                    response = "Sign in error\n";
            }
            else
                response = "Invalid input format\n";
        }

        else if(command.rfind("SEND ", 0) == 0) {
            std::istringstream iss(command.substr(5));
            std::string sender;
            std::string receiver;
            std::string content;
            if(iss >> receiver){
                if(iss >> sender){
                    std::getline(iss, content);
                    if(!content.empty() && content[0] == ' ')
                    {
                        content.erase(0, 1);
                    }
                    if(chat.sendMessage(sender, receiver, content))
                    {
                        response = "Message was sent\n";
                    }
                    else
                        response = "Message wan't sent\n";
                }
            }
            else
                response = "Invalid input format\n";
        }

        else if(command.rfind("GET MESSAGE ", 0) == 0) {
            std::istringstream iss(command.substr(12));
            std::string receiver; 
            if(iss >> receiver){
                std::vector<std::string> message = chat.getMessages(receiver);
                for(const auto msg : message) {
                    response += msg + "\n";
                }
            }
            else
                response = "Invalid input format\n";
        }

        write(client_socket, response.c_str(), response.size());
    }

    close(client_socket);
}

int main() {

    using namespace std;

    struct sockaddr_in addr;
    int sock, listener;
    char buf[1024];
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0){
        cout << "error listener" << endl;
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(listener, (struct sockaddr *)& addr, sizeof(addr)) < 0){
        cout << "error bind" << endl;
        exit(2);
    }
    listen(listener, 4);
    cout << "Listening..." << endl;

    while(1){

        sock = accept(listener, NULL, NULL);
        if(sock < 0){
            cout << "accept error" << endl;
            exit(3);
        }
        cout << "accept..." << endl;
        std::thread(commandsClient, sock).detach();
    }

    close(listener);
    return 0;
}