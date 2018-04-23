#ifndef UTILS_H_
#define UTILS_H_

#include <time.h>
#include <chrono>

#define FPS                 60
#define FRAME_TIME_UNIT     1000000000 / FPS
#define ONE_MILLISECOND     1000000

class Timer {
public:
  Timer() {}

  void sleep(long nanosec) {
    if (nanosec > 0) {
      struct timespec req = { 0 };
      req.tv_sec = 0;
      req.tv_nsec = nanosec;
      nanosleep(&req, (struct timespec *)0);
    }
  }

  void resetTimer() {
    m_start = std::chrono::high_resolution_clock::now();
  }

  long getTimerInNanoseconds() {
    auto finish = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(finish - m_start).count();
  }

  void sleepUntilNextFrame() {
    sleep(FRAME_TIME_UNIT - getTimerInNanoseconds());
  }

private:
  std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::duration<long long int, std::ratio<1ll, 1000000000ll> > > m_start;
};


#endif /* UTILS_H_ */