//
// Created by 12552 on 2022/5/20.
//

#ifndef RBTREE_RBTREE_IMPLEMENTATION_H
#define RBTREE_RBTREE_IMPLEMENTATION_H

#include "RBtree.h"

namespace VgdStd {
    template<typename k_t, typename v_t>
    std::pair<bool, typename RBtree<k_t, v_t>::NodePtr_T> RBtree<k_t, v_t>::Find(const k_t &k) const {
        using res_t = std::pair<bool, NodePtr_T>; // 返回值类型
        NodePtr_T current = root_; // 当前指针
        while (current != nullptr) {
            if (current->data_.first == k) {  // 找到节点
                return res_t({true, current});
            } else if (current->data_.first > k) {// 比当前节点小，找左子结点
                current = current->left_;
            } else {
                current = current->right_; // 比当前节点大，找右子结点
            }
        }
        // 遍历完没找到
        return res_t({false, nullptr});
    }

    template<typename k_t, typename v_t>
    bool RBtree<k_t, v_t>::Insert(std::pair<k_t, v_t> k) {

        using node_t = RBtreeNode<k_t, v_t>;

        if (root_ == nullptr) {
            root_ = new node_t(k.first, k.second, node_t::BLACK);
            return true;
        }

        NodePtr_T current = root_; // 当前指针
        while (true) {
            if (current->DataKeyEqual(k)) {  // 找到相同节点，插入失败
                return false;
            } else if (current->DataKeyComp(k)) {// 比当前节点大，找右子结点
                if (current->right_ == nullptr) { // 右子结点已为空，可插入
                    current->right_ = new node_t(k.first, k.second, node_t::RED);
                    current->right_->parent_ = current;
                    current = current->right_;
                    break;
                }
                current = current->right_; // 右子结点不为空，继续搜寻
            } else {// 比当前节点小，找左子结点
                if (current->left_ == nullptr) { // 左子结点已为空，可插入
                    current->left_ = new node_t(k.first, k.second, node_t::RED);
                    current->left_->parent_ = current;
                    current = current->left_;
                    break;
                }
                current = current->left_; // 左子结点不为空，继续搜寻
            }
        }

        current->FixInsert(root_);// 插入节点开始修复
        return true;
    }


    template<typename k_t, typename v_t>
    void RBtree<k_t, v_t>::Print() const {
        std::vector<NodePtr_T> queque({root_});
        auto element_in_func = [&](const NodePtr_T &node_ptr) {
            node_ptr->PrintKey();
        };

        RBtreeNode<k_t, v_t>::LevelTraversal(queque, element_in_func);
    }

    template<typename k_t, typename v_t>
    bool RBtree<k_t, v_t>::Erase(const k_t &key) {
        auto find_result = Find(key);
        if (!find_result.first) {// 没找到，删不了
            return false;
        }
        find_result.second->Erase(root_);
        return true;
    }

    template<typename k_t, typename v_t>
    std::vector<typename RBtree<k_t, v_t>::Element_T> RBtree<k_t, v_t>::OutElement() const {
        std::vector<Element_T> elements;
        if (root_ != nullptr) {
            auto element_in_func = [&](const Element_T &element) {
                elements.template emplace_back(element);
            };
            root_->InorderTraversal(element_in_func);
        }
        return elements;
    }
}

#endif //RBTREE_RBTREE_IMPLEMENTATION_H
