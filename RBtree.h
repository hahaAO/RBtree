//
// Created by 12552 on 2022/5/19.
//

#ifndef RBTREE_RBTREE_H
#define RBTREE_RBTREE_H


#include <utility>
#include "RBtreeNode.h"

namespace vgd {


    template<typename k_t, typename v_t>
    class RBtree {

    public:
        using NodePtr_T = typename RBtreeNode<k_t, v_t>::NodePtr_T;

        void print() const;

        bool insert(std::pair<k_t, v_t>);

        bool insert(k_t k, v_t v) { return insert(std::pair<k_t, v_t>(k, v)); }

        std::pair<bool, NodePtr_T> find(k_t) const;

        bool erase(k_t);

        bool erase(NodePtr_T);

    private:
        NodePtr_T _root = nullptr;
    };


    template<typename k_t, typename v_t>
    std::pair<bool, typename RBtree<k_t, v_t>::NodePtr_T> RBtree<k_t, v_t>::find(k_t k) const {
        using res_t = std::pair<bool, NodePtr_T>; // 返回值类型
        NodePtr_T current = this->_root; // 当前指针
        while (current != nullptr) {
            if (current->_data.first == k) {  // 找到节点
                return res_t({true, nullptr});
            } else if (current->_data.first > k) {// 比当前节点小，找左子结点
                current = current->_left;
            }
            current = current->_right; // 比当前节点大，找右子结点
        };
        // 遍历完没找到
        return res_t({false, nullptr});
    }

    template<typename k_t, typename v_t>
    bool RBtree<k_t, v_t>::insert(std::pair<k_t, v_t> k) {

        using node_t = RBtreeNode<k_t, v_t>;

        if (_root == nullptr) {
            _root = new node_t(k.first, k.second, node_t::BLACK);
            return true;
        }

        NodePtr_T current = this->_root; // 当前指针
        while (true) {
            if (current->dataKeyEqual(k)) {  // 找到相同节点，插入失败
                return false;
            } else if (current->dataKeyComp(k)) {// 比当前节点大，找右子结点
                if (current->_right == nullptr) { // 右子结点已为空，可插入
                    current->_right = new node_t(k.first, k.second, node_t::RED);
                    current->_right->_parent = current;
                    current = current->_right;
                    break;
                }
                current = current->_right; // 右子结点不为空，继续搜寻
            } else {// 比当前节点小，找左子结点
                if (current->_left == nullptr) { // 左子结点已为空，可插入
                    current->_left = new node_t(k.first, k.second, node_t::RED);
                    current->_left->_parent = current;
                    current = current->_left;
                    break;
                }
                current = current->_left; // 左子结点不为空，继续搜寻
            }
        }

        current->sort(_root);// 指定节点开始重排
        _root->_color = RBtreeNode<k_t, v_t>::BLACK;
        return true;
    }


    template<typename k_t, typename v_t>
    void RBtree<k_t, v_t>::print() const {
        std::vector<NodePtr_T> queque({_root});
        RBtreeNode<k_t, v_t>::loopPrint(queque);
    }


} // namespace vgd


#endif //RBTREE_RBTREE_H
