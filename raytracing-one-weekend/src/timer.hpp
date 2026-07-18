#ifndef INCLUDE_SRC_TIMER_HPP_
#define INCLUDE_SRC_TIMER_HPP_
#include <chrono>
#include <iomanip>
#include <ios>
#include <ostream>

class timer {
public:
    template<typename func_t>
    timer(func_t function) {
        using std::chrono::system_clock;
        start = system_clock::now();
        function();
        end = system_clock::now();
    }

    auto delta() const { return end - start; }

    double seconds() {
        using std::chrono::duration_cast;
        using std::chrono::nanoseconds;
        return duration_cast<nanoseconds>(delta()).count() / 1000000000.0;
    }

    std::chrono::time_point<std::chrono::system_clock> start, end;
};

inline std::ostream& operator<<(std::ostream& os, timer& t) {
    return os << std::fixed << std::setprecision(6) << t.seconds();
}

#endif  // INCLUDE_SRC_TIMER_HPP_
