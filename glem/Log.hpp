#pragma once

#include <sstream>
#include <string_view>

#include <type_traits>

namespace glem {

    class Log {
    public:
        Log() = delete;
        ~Log() = delete;

        Log(Log&&) = delete;
        Log(const Log&) = delete;

        Log& operator=(Log&&) = delete;
        Log& operator=(const Log&) = delete;

        template<typename ... Ts>
        inline static void i(std::string_view tag, Ts ... args) noexcept {
            std::stringstream ss;

            ((ss << to_string(args)), ...);

            Log::i_(tag, ss.str());
        }

        template<typename ... Ts>
        inline static void w(std::string_view tag, Ts ... args) noexcept {
            std::stringstream ss;

            ((ss << to_string(args)), ...);

            Log::w_(tag, ss.str());
        }

        template<typename ... Ts>
        inline static void e(std::string_view tag, Ts ... args) noexcept {
            std::stringstream ss;

            ((ss << to_string(args)), ...);

            Log::e_(tag, ss.str());
        }

#ifdef NDEBUG
        template<typename ... Ts>
        inline static void d(std::string_view tag, Ts ... args) noexcept {

        }
#else
        template<typename ... Ts>
        inline static void d(std::string_view tag, Ts ... args) noexcept {
            std::stringstream ss;

            ((ss << to_string(args)), ...);

            Log::d_(tag, ss.str());
        }
#endif

    private:
        template<typename T>
        inline static std::string to_string(T&& arg) noexcept {
            if constexpr (std::is_same_v<std::string, std::decay_t<T>>) {
                return arg;
            }
            else
                return std::to_string(arg) + " ";
        }

        inline static std::string to_string(const char* str) noexcept {
            return str;
        }

        static void i_(std::string_view tag, std::string_view msg) noexcept;
        static void w_(std::string_view tag, std::string_view msg) noexcept;
        static void e_(std::string_view tag, std::string_view msg) noexcept;
        static void d_(std::string_view tag, std::string_view msg) noexcept;

    };

}
