#pragma once

#include <chrono>

namespace glem::util {

    class Timer {
    public:
        Timer();
        ~Timer();

        /**
         * @brief mark
         * @return
         */
        float mark() noexcept;

        /**
         * @brief peek
         * @return
         */
        float peek() const noexcept;

    private:
        std::chrono::steady_clock::time_point last_;

    };

}
