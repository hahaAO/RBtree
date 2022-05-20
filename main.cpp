#include "pch.h"
#include "RBtree.h"
#include "tool.h"


void correctnessTest() {
    std::map<int, int> t1;
    vgd::RBtree<int, int> t2;

    std::default_random_engine random;
    std::uniform_int_distribution<int> dis1(0, 100);

    for (int i = 0; i < 20; ++i) {
        auto randomNum = dis1(random);

        t1.insert({randomNum, 1});
        t2.insert({randomNum, 1});
    }

    t2.print();

    for (auto kv: t1) {
        std::cout << " " << kv.first << " ";
    }

    std::cout << std::endl;

}

void profermanceTest() {
    constexpr int testNum = 100000;
    int *arr = new int[testNum];// 直接在栈里分配空间会崩溃,window 的函数栈空间大小为1MB, Linux为8MB
    std::default_random_engine random;
    std::uniform_int_distribution<int> dis1(0, testNum);

    for (int i = 0; i < testNum; ++i) {
        arr[i] = dis1(random);
    }

    std::map<int, int> t1;
    vgd::RBtree<int, int> t2;

    {
        vgd::timer timer1;
        for (int i = 0; i < testNum; ++i) {
            t1.insert({arr[i], 1});
        }
    }

    {
        vgd::timer timer1;
        for (int i = 0; i < testNum; ++i) {
            t2.insert({arr[i], 1});
        }
    }

}

int main() {
    correctnessTest();

    profermanceTest();
    return 0;
}
