#define _CRT_SECURE_NO_WARNINGS

#include "log.hpp"

#include <ctime>
#include <iomanip>
#include <iostream>

namespace glem::util {

    void Log::i_(std::string_view tag, std::string_view msg) noexcept
    {
        auto t = std::time(nullptr);

        std::cout << std::put_time(std::localtime(&t), "%F %H:%M:%S") << " [" << tag << "/I]: " << msg << std::endl;
    }

    void Log::w_(std::string_view tag, std::string_view msg) noexcept
    {
        auto t = std::time(nullptr);

        std::cerr << std::put_time(std::localtime(&t), "%F %H:%M:%S") << " [" << tag << "/W]: " << msg << std::endl;
    }

    void Log::e_(std::string_view tag, std::string_view msg) noexcept
    {
        auto t = std::time(nullptr);

        std::cerr << std::put_time(std::localtime(&t), "%F %H:%M:%S") << " [" << tag << "/E]: " << msg << std::endl;
    }

    void Log::d_(std::string_view tag, std::string_view msg) noexcept
    {
        auto t = std::time(nullptr);

        std::cout << std::put_time(std::localtime(&t), "%F %H:%M:%S") << " [" << tag << "/D]: " << msg << std::endl;
    }

}
