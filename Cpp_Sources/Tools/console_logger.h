#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class ConsoleLogger {
private:
    // Making constructor private to prevent instantiation from outside the class
    ConsoleLogger() {
        m_buffer = "";
    }

    String m_buffer;

    static ConsoleLogger* ptrInstance;

public:
    // Deleting copy constructor and assignment operator
    ConsoleLogger(const ConsoleLogger&) = delete;
    ConsoleLogger& operator=(const ConsoleLogger&) = delete;

    void clear_buffer() { m_buffer = ""; };
    void append_to_buffer(const String& string) { m_buffer += string; };
    virtual void print() { UtilityFunctions::print(m_buffer); };

    // Static method to get the instance
    static ConsoleLogger& instance();

    static ConsoleLogger& logger() { return instance(); }

    // Manipulator keyword
    static ConsoleLogger& endLine(ConsoleLogger& logger);

    friend ConsoleLogger& operator<<(ConsoleLogger& logger, const String& string);
    friend ConsoleLogger& operator<<(ConsoleLogger& logger, const float& number);
    friend ConsoleLogger& operator<<(ConsoleLogger& logger, const int& number);
    friend ConsoleLogger& operator<<(ConsoleLogger& logger, const uint& number);
    friend ConsoleLogger& operator<<(ConsoleLogger& logger, ConsoleLogger& (*manip)(ConsoleLogger&));
};

#endif