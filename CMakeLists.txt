cmake_minimum_required(VERSION 3.10)
project(ChatApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Укажите путь к директории, где находятся заголовочные файлы
include_directories(${CMAKE_SOURCE_DIR}/include)

# Укажите путь к MySQL Connector/C++
set(MYSQL_CONNECTOR_CPP_INCLUDE_DIR "/path/to/mysql-connector-c++/include")
set(MYSQL_CONNECTOR_CPP_LIB_DIR "/path/to/mysql-connector-c++/lib")

include_directories(${MYSQL_CONNECTOR_CPP_INCLUDE_DIR})
link_directories(${MYSQL_CONNECTOR_CPP_LIB_DIR})

# Определите исполняемые файлы
add_executable(ChatClient src/TcpClient.cpp src/chat.cpp)
add_executable(ChatServer src/TcpServer.cpp src/chat.cpp)

# Линкуем библиотеки MySQL Connector/C++
target_link_libraries(ChatClient mysqlcppconn)
target_link_libraries(ChatServer mysqlcppconn)