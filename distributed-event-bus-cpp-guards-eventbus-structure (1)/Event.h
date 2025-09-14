#pragma once
#include <string>
#include <iostream>

enum class EventType {
    INFO,
    WARNING
};

class Event {
public:
    Event(const std::string& publisher, EventType type, const std::string& description)
        : publisher_(publisher), type_(type), description_(description) {}

    void print() const {
        std::cout << "[" << publisher_ << "] ";
        if (type_ == EventType::INFO) std::cout << "INFO: ";
        else std::cout << "WARNING: ";
        std::cout << description_ << std::endl;
    }

private:
    std::string publisher_;
    EventType type_;
    std::string description_;
};
