#include "Log.hpp"

#include <time.h>
#include <iomanip>
#include <iostream>

#include <iomanip>
#include <iostream>

namespace {
#if __WIN32__
    std::string timestamp () noexcept {
        std::stringstream ss;

        std::tm     timetsamp;
        std::time_t time = std::time (nullptr);

        localtime_s (&timetsamp, &time);

        ss << std::put_time (&timetsamp, "%d-%b-%Y %H:%M:%S");

        return ss.str ();
    }
#else
    std::string timestamp () noexcept {
        std::stringstream ss;

        auto t = std::time(nullptr);

        ss << std::put_time (std::localtime(&t), "%d-%b-%Y %H:%M:%S");

        return ss.str ();
    }
#endif
}

namespace glem {

    void Log::i_ (std::string_view tag, std::string_view msg) noexcept
    {
        std::cout << timestamp () << " [" << tag << "/I]: " << msg << std::endl;
    }

    void Log::w_ (std::string_view tag, std::string_view msg) noexcept
    {
        std::cout << timestamp () << " [" << tag << "/W]: " << msg << std::endl;
    }

    void Log::e_ (std::string_view tag, std::string_view msg) noexcept
    {
        std::cerr << timestamp () << " [" << tag << "/E]: " << msg << std::endl;
    }

    void Log::d_ (std::string_view tag, std::string_view msg) noexcept
    {
        std::cerr << timestamp () << " [" << tag << "/D]: " << msg << std::endl;
    }

}
