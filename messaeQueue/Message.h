 #pragma once
#include <string>
#include <iostream>

class Message {
public:
    Message(const std::string& sender, const std::string& content)
        : sender(sender), content(content) {}

    void print() const {
        std::cout << "[" << sender << "] " << content << std::endl;
    }

    std::string getContent() const { return content; }

private:
    std::string sender;
    std::string content;
};
