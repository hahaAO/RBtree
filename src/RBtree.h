//
// Created by 12552 on 2022/5/19.
//

#ifndef RBTREE_RBTREE_H
#define RBTREE_RBTREE_H


#include <utility>
#include "RBtreeNode.h"

namespace VgdStd {

    template<typename k_t, typename v_t>
    class RBtree {

    public:
        using NodePtr_T = typename RBtreeNode<k_t, v_t>::NodePtr_T;
        using Element_T = typename RBtreeNode<k_t, v_t>::Element_T;

        std::vector<Element_T> OutElement() const;

        void Print() const;

        bool Insert(std::pair<k_t, v_t>);

        bool Insert(k_t key, v_t value) { return Insert(std::pair<k_t, v_t>(key, value)); }

        std::pair<bool, NodePtr_T> Find(const k_t &) const;

        bool Erase(const k_t &);

    private:
        NodePtr_T root_ = nullptr;
        // TODO: 把空节点换成实际的节点,修改指针指向时不用判断是否空
    };


} // namespace VgdStd

#include "RBtree_implementation.h"

#endif //RBTREE_RBTREE_H
