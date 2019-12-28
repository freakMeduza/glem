#include "timer.hpp"

namespace glem::util {

    Timer::Timer() :
        last_ {std::chrono::steady_clock::now()}
    {

    }

    Timer::~Timer()
    {

    }

    float Timer::mark() noexcept
    {
        const auto old = last_;

        last_ = std::chrono::steady_clock::now();

        const auto delta = std::chrono::duration<float>(last_ - old);

        return delta.count();
    }

    float Timer::peek() const noexcept
    {
        return std::chrono::duration<float>(std::chrono::steady_clock::now() - last_).count();
    }

}
