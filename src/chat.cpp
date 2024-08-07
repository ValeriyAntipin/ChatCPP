#include "chat.h"

Chat::Chat()
{
    driver = get_driver_instance();
    con.reset(driver->connect("tcp://127.0.0.1:3306", "root", "new_password"));
    con->setSchema("chat_db");
}

Chat::~Chat()
{ 
    con->close();
}


bool Chat::registration(const std::string& login, const std::string& password, const std::string& nick_name)
{
    try {
        std::unique_ptr<sql::PreparedStatement> pr_stmt;
        pr_stmt.reset(con->prepareStatement("INSERT INTO users (login, password, name) VALUE (?, ?, ?)"));

        pr_stmt->setString(1, login);
        pr_stmt->setString(2, password);
        pr_stmt->setString(3, nick_name);
        pr_stmt->executeUpdate();
        return true;
    }   catch (sql::SQLException& e){
        std::cerr << "SQL error: " << e.what() << std::endl;
        return false;
    }
}

bool Chat::sign_in(const std::string &login, const std::string &password)
{
    try {
        std::unique_ptr<sql::PreparedStatement> pr_stmt;
        pr_stmt.reset(con->prepareStatement("SELECT * FROM users WHERE login = ? AND password = ?"));

        pr_stmt->setString(1, login);
        pr_stmt->setString(2, password);
        std::unique_ptr<sql::ResultSet> res (pr_stmt->executeQuery());

        return res->next();
    }   catch(sql::SQLException& e){
        std::cerr << "SQL error: " << e.what() << std::endl;
        return false;
    }
}

bool Chat::sendMessage(const std::string& sender, const std::string& receiver, const std::string& text) {
    try {
        std::unique_ptr<sql::PreparedStatement> pr_stmt (con->prepareStatement("SELECT ID FROM users WHERE login = ?"));
        pr_stmt->setString(1, sender);
        std::unique_ptr<sql::ResultSet> res (pr_stmt->executeQuery());

        if(!res->next()){
            return false;
        }
        int sender_id = res->getInt("id");

        pr_stmt.reset(con->prepareStatement("SELECT ID FROM users WHERE login = ?"));
        pr_stmt->setString(1, receiver);
        res.reset(pr_stmt->executeQuery());

        if(!res->next()){
            return false;
        }
        int receiver_id = res->getInt("id");

        pr_stmt.reset(con->prepareStatement("INSERT INTO messages (user_id, recipient_id, message) VALUES (?, ?, ?)"));
        pr_stmt->setInt(1, sender_id);
        pr_stmt->setInt(2, receiver_id);
        pr_stmt->setString(3, text);
        pr_stmt->executeUpdate();

        return true;
    } catch (sql::SQLException& e){
        std::cerr << "SQL error: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> Chat::getMessages(const std::string& reciever) {
    std::vector<std::string> messages; 
    try {
        std::unique_ptr<sql::PreparedStatement> pr_stmt (con->prepareStatement(
            "SELECT users.name, messages.message, messages.timestamp "
            "FROM messages JOIN users ON messages.user_id = users.id "
            "WHERE recipient_id = (SELECT id FROM users WHERE login = ?) "
            "ORDER BY messages.timestamp"
        ));
        pr_stmt->setString(1, reciever);
        
        std::unique_ptr<sql::ResultSet> res (pr_stmt->executeQuery());

        while(res->next()){
            std::string sender = res->getString("name");
            std::string content = res->getString("message");
            std::string timestamp = res->getString("timestamp");

            messages.push_back(sender + " (" + timestamp + "): " + content);
        }
    } catch(sql::SQLException& e){
        std::cerr << "SQL error: " << e.what() << std::endl;
    }

    return messages;
}