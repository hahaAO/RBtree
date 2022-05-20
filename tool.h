//
// Created by 12552 on 2022/5/20.
//

#ifndef RBTREE_TOOL_H
#define RBTREE_TOOL_H

#include <chrono>

namespace vgd {
    class timer {
    private:
        decltype(std::chrono::system_clock::now()) startTime;
    public:
        timer() : startTime(std::chrono::system_clock::now()) {}

        ~timer() {
            auto endTime = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
            std::cout << "花费了"
                      << double(duration.count()) * std::chrono::microseconds::period::num /
                         std::chrono::microseconds::period::den
                      << "秒" << std::endl;
        }
    };
}// namespace vgd

#endif //RBTREE_TOOL_H
