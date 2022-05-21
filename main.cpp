#include "pch.h"
#include "RBtree.h"
#include "tool.h"

template<typename k_t, typename v_t>
void EqualTest(const std::map<k_t, v_t> &t1, const VgdStd::RBtree<k_t, v_t> &t2) {// 这个测试程序时间复杂度是log(n)，不要放在循环里
    auto elements = t2.OutElement();
    int element_index = 0;
    for (auto kv: t1) {
        assert(kv.first == elements[element_index].first);
        ++element_index;
    }
}

void correctnessTest() {
    constexpr int kTestNum = 20000;

    std::vector<int> init_array(kTestNum);
    std::vector<int> test_array(kTestNum);

    std::default_random_engine random;
    std::uniform_int_distribution<int> dis1(0, kTestNum);
    std::uniform_int_distribution<int> dis2(0, kTestNum);

    for (int i = 0; i < kTestNum; ++i) {
        init_array[i] = dis1(random);
        test_array[i] = dis2(random);
    }

    std::map<int, int> t1;
    VgdStd::RBtree<int, int> t2;

    for (int i = 0; i < kTestNum; ++i) {
        t1.insert({init_array[i], 1});
        t2.insert({init_array[i], 1});
    }
    EqualTest(t1, t2);

    std::cout << "插入功能测试通过" << std::endl;

    for (int i = 0; i < kTestNum; ++i) {
        auto result_1 = t1.find(test_array[i]);
        auto result_2 = t2.find(test_array[i]);
        assert((result_1 != t1.end()) == result_2.first);
    }
    EqualTest(t1, t2);

    std::cout << "查找功能测试通过" << std::endl;

    for (int i = 0; i < kTestNum; ++i) {
        t1.erase(test_array[i]);
        t2.erase(test_array[i]);
    }
    EqualTest(t1, t2);

    std::cout << "删除功能测试通过" << std::endl;
}

void profermanceTest() {
    constexpr int kTestNum = 2000;
    std::vector<int> init_array(kTestNum);
    std::vector<int> test_array(kTestNum);
    std::default_random_engine random;
    std::uniform_int_distribution<int> dis1(0, kTestNum);
    std::uniform_int_distribution<int> dis2(0, kTestNum);

    for (int i = 0; i < kTestNum; ++i) {
        init_array[i] = dis1(random);
        test_array[i] = dis2(random);
    }

    std::map<int, int> t1;
    VgdStd::RBtree<int, int> t2;

    {
        VgdStd::Timer timer("std插入" + std::to_string(kTestNum) + "次");
        for (int i = 0; i < kTestNum; ++i) {
            t1.insert({init_array[i], 1});
        }
    }

    {
        VgdStd::Timer timer("VgdStd插入" + std::to_string(kTestNum) + "次");
        for (int i = 0; i < kTestNum; ++i) {
            t2.insert({init_array[i], 1});
        }
    }


    {
        VgdStd::Timer timer("std删除" + std::to_string(kTestNum) + "次");
        for (int i = 0; i < kTestNum; ++i) {
            t1.erase(test_array[i]);
        }
    }

    {
        VgdStd::Timer timer("VgdStd删除" + std::to_string(kTestNum) + "次");
        for (int i = 0; i < kTestNum; ++i) {
            t2.erase(test_array[i]);
        }
    }
}

int main() {
    correctnessTest();
    profermanceTest();
    return 0;
}
