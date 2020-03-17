#pragma once

#include <chrono>

namespace glem {

    class Timer {
    public:
        Timer () : last_{ std::chrono::steady_clock::now () } {

        }

        ~Timer () = default;

        inline float mark () noexcept {
            const auto old = last_;

            last_ = std::chrono::steady_clock::now ();

            const std::chrono::duration<float> frameTime = last_ - old;

            return frameTime.count ();
        }

        inline float peek () const noexcept {
            return std::chrono::duration<float> (std::chrono::steady_clock::now () - last_).count ();
        }

    private:
        std::chrono::steady_clock::time_point last_;

    };

}
