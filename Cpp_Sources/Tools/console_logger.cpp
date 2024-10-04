#include "console_logger.h"

// Initialize the static instance to null
ConsoleLogger* ConsoleLogger::ptrInstance = nullptr;

ConsoleLogger& ConsoleLogger::instance() {
    if (!ptrInstance) {
        ptrInstance = new ConsoleLogger();
    }
    return *ptrInstance;
}

ConsoleLogger& ConsoleLogger::endLine(ConsoleLogger& logger) {
    logger.print();
    logger.clear_buffer();
    return logger;
}

ConsoleLogger& operator<<(ConsoleLogger& logger, const String& string)
{
    logger.append_to_buffer(string);
    return logger;
}

ConsoleLogger& operator<<(ConsoleLogger& logger, const float& number)
{
    logger.append_to_buffer(String::num(number));
    return logger;
}

ConsoleLogger& operator<<(ConsoleLogger& logger, const int& number)
{
    logger.append_to_buffer(String::num_int64(number));
    return logger;
}

ConsoleLogger& operator<<(ConsoleLogger& logger, const uint& number)
{
    logger.append_to_buffer(String::num_uint64(number));
    return logger;
}

// endLine manipulator case
ConsoleLogger& operator<<(ConsoleLogger& logger, ConsoleLogger& (*manip)(ConsoleLogger&)) {
    ConsoleLogger::endLine(logger);
    return logger;
}
