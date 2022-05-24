//
// Created by 12552 on 2022/5/20.
//

#ifndef RBTREE_RBTREENODE_IMPLEMENTATION_H
#define RBTREE_RBTREENODE_IMPLEMENTATION_H


#include "RBtreeNode.h"

#define PP_NODE  parent_->parent_ // 爷节点

namespace VgdStd {
    template<typename k_t, typename v_t>
    inline void RBtreeNode<k_t, v_t>::ReplaceParent(NodePtr_T &root_node) {
        // 该函数用于方便地替换父节点，把自己的父节点指向爷节点，爷节点的子指向自己
        // 注意不会改动原父节点和自己的子节点

        assert(!ParentIsNull());
        if (parent_->ParentIsNull()) {// 爷节点为空
            parent_ = nullptr;
            root_node = this;
        } else if (parent_->AmILeft()) {// 父为爷节点的左子
            parent_ = PP_NODE;
            parent_->left_ = this;
        } else {
            parent_ = PP_NODE;
            parent_->right_ = this;
        }
    }

    template<typename k_t, typename v_t>
    typename RBtreeNode<k_t, v_t>::NodePtr_T RBtreeNode<k_t, v_t>::LR(RBtreeNode::NodePtr_T &root_node) {
        if (left_ != nullptr) {
            left_->parent_ = parent_;
        }
        parent_->right_ = left_;

        auto oldp = parent_;

        ReplaceParent(root_node);

        oldp->parent_ = this;
        left_ = oldp;

        return left_;
    }

    template<typename k_t, typename v_t>
    typename RBtreeNode<k_t, v_t>::NodePtr_T RBtreeNode<k_t, v_t>::RR(RBtreeNode::NodePtr_T &root_node) {
        if (right_ != nullptr) {
            right_->parent_ = parent_;
        }
        parent_->left_ = right_;

        auto oldp = parent_;

        ReplaceParent(root_node);

        oldp->parent_ = this;
        right_ = oldp;

        return right_;
    }

    template<typename k_t, typename v_t>
    void RBtreeNode<k_t, v_t>::FixInsert(NodePtr_T &rootNode) {
        if (ParentIsNull()) { // 自己是根节点
            color_ = BLACK;
            return;
        }
        if (parent_->color_ == BLACK) { // 父节点为黑，不用调整
            return;
        }

        auto res = TryExchangeColor();
        if (res.first) { // 染色成功，继续调整
            res.second->FixInsert(rootNode);
            return;
        }

        if (AmILeft()) {// 右旋的条件：自身为父的左节点
            tryRR(rootNode);
        } else {// 左旋的条件：自身为父的右节点
            tryLR(rootNode);
        }
    }

    template<typename k_t, typename v_t>
    void RBtreeNode<k_t, v_t>::PrintKey() const {
        if (parent_ == nullptr) {
            std::cout << " {NULL-" << data_.first << " " << (color_ == RED ? "红" : "黑") << "} ";
        } else {
            std::cout << " {" << parent_->data_.first << "-" << data_.first << " " << (color_ == RED ? "红" : "黑")
                      << "} ";
        }
    }

    template<typename k_t, typename v_t>
    void RBtreeNode<k_t, v_t>::LevelTraversal(std::vector<NodePtr_T> &queque,
                                              std::function<void(const NodePtr_T &)> do_something) {

        if (queque.empty()) return;
        std::vector<NodePtr_T> newQueque;

        for (NodePtr_T node: queque) {
            if (node != nullptr) {
                do_something(node);
                newQueque.emplace_back(node->left_);
                newQueque.emplace_back(node->right_);
            }
        }
        LevelTraversal(newQueque, do_something);
        return;
    }


    template<typename k_t, typename v_t>
    std::pair<bool, typename RBtreeNode<k_t, v_t>::NodePtr_T> RBtreeNode<k_t, v_t>::TryExchangeColor() {
        assert(!ParentIsNull() && !parent_->ParentIsNull()); // 根据红黑树的性质，默认为真
        // 染色的条件：自身为红色，父节点的兄弟为红色
        if (PP_NODE->left_ != nullptr && PP_NODE->left_->color_ == RED &&
            PP_NODE->right_ != nullptr && PP_NODE->right_->color_ == RED) {
            // 染色:颜色交换,保证爷节点以下满足红黑树
            PP_NODE->color_ = RED;
            PP_NODE->left_->color_ = BLACK;
            PP_NODE->right_->color_ = BLACK;
            // 爷节点需要修复
            return std::pair<bool, NodePtr_T>({true, PP_NODE});// 染色成功,返回下一个操作节点
        }
        return std::pair<bool, NodePtr_T>({false, nullptr});// 不能染色
    }

    template<typename k_t, typename v_t>
    void RBtreeNode<k_t, v_t>::tryLR(NodePtr_T &root_node) {
        assert(!ParentIsNull() && !parent_->ParentIsNull()); // 根据红黑树的性质，默认为真

        if (!parent_->AmILeft()) {// 爷父子同向结构，
            FixInsertDyeLR(root_node); // 触发染色左旋，直接修复
        } else {// 爷父子逆向结构
            auto nextNode = LR(root_node); // 触发非染色左旋，修复成同向结构
            nextNode->FixInsertDyeRR(root_node);// 触发染色右旋，直接修复
        }
    }

    template<typename k_t, typename v_t>
    inline void RBtreeNode<k_t, v_t>::FixInsertDyeLR(NodePtr_T &root_node) {
        // 先调整颜色
        parent_->color_ = BLACK;
        PP_NODE->color_ = RED;
        parent_->LR(root_node);
    }


    template<typename k_t, typename v_t>
    void RBtreeNode<k_t, v_t>::tryRR(NodePtr_T &root_node) {
        assert(!ParentIsNull() && !parent_->ParentIsNull()); // 根据红黑树的性质，默认为真

        if (parent_->AmILeft()) {// 爷父子同向结构，
            FixInsertDyeRR(root_node); // 触发染色右旋，直接修复
        } else {// 爷父子逆向结构
            auto nextNode = RR(root_node); // 非染色右旋，修复成同向结构
            nextNode->FixInsertDyeLR(root_node);// 触发染色左旋，直接修复
        }
    }

    template<typename k_t, typename v_t>
    inline void RBtreeNode<k_t, v_t>::FixInsertDyeRR(NodePtr_T &root_node) {
        // 操作同染色左旋对称
        parent_->color_ = BLACK;
        PP_NODE->color_ = RED;
        parent_->RR(root_node);
    }

    template<typename k_t, typename v_t>
    void RBtreeNode<k_t, v_t>::Erase(RBtreeNode::NodePtr_T &root_node) {
        auto original_color = color_;// 记录被删除的节点的 color，如果是黑色说明破坏了black-height稳定限制，需要修复性质
        std::function<void()> Fixup = nullptr;     // 记录要调用的修复函数
        if (!ParentIsNull()) {// 如果自己是根节点被删，不需要修复
            if (AmILeft()) {
                Fixup = std::bind(&RBtreeNode<k_t, v_t>::FixLeft, parent_, std::ref(root_node));
            } else {
                Fixup = std::bind(&RBtreeNode<k_t, v_t>::FixRight, parent_, std::ref(root_node));
            }
        }


        if (left_ == nullptr) {
            if (right_ == nullptr) {
                if (ParentIsNull()) {// 就剩一个节点，直接删除
                    delete this;
                    root_node = nullptr;
                    return;
                }
                // 双空且有父节点,把空节点提上来
                if (AmILeft()) {
                    parent_->left_ = nullptr;
                } else {
                    parent_->right_ = nullptr;
                }
                delete this;
            } else {// 左子树为空,直接把右子树提上来
                right_->ReplaceParent(root_node);
                delete this;
            }
        } else if (right_ == nullptr) {
            // 右子树为空,直接把左子树提上来
            left_->ReplaceParent(root_node);
            delete this;
        } else {// 左右不为空，在右子树找个替死鬼
            auto min_node = right_;
            while (min_node->left_ != nullptr) {
                min_node = min_node->left_;
            }
            // 替死鬼是右子树最小的那个,可以把他的数据移到该节点而不影响红黑树左小右大的限制
            // 替死鬼的颜色不跟着移过来，所以该节点看起来跟原来一样，只需要删除替死鬼即可
            data_ = min_node->data_;
            min_node->Erase(root_node);
            return;
        }

        if (original_color == BLACK && Fixup != nullptr) {// 需要修复
            Fixup();
        }
    }

    template<typename k_t, typename v_t>
    void RBtreeNode<k_t, v_t>::FixLeft(RBtreeNode::NodePtr_T &root_node) {
        // 当前节点的左子本来是黑色的被删了，要修复
        // 有两条定理可利用： 1、右子一定存在且 hb>=1    2、右子为红则我一定黑

        // 修复手段如下：
        // 1、如果新节点(左子)是红色，直接染黑就修复了
        // 2、左子是黑色 右子为黑 且右子的右子存在是红色，可以直接修复
        // 3、左子是黑色 右子为黑 且右子的左子为红，右为非红，可修复至情况2
        // 4、左子是黑色 右子为黑 且右子的左右都为非红，直接把右节点hb-1，向上修复
        // 5、左子是黑色 右子为红 ,可修复至情况234
        if (IsRed(left_)) {
            left_->color_ = BLACK;
            return;
        }

        if (IsRed(right_)) {// 情况5
            right_->color_ = BLACK;
            color_ = RED;
            right_->LR(root_node);
            FixLeft(root_node);//右子左旋后，右子的左子变我右子，右边bh仍然比左边高1,递归修复
        } else {
            auto rla = IsRed(right_->left_);
            auto rra = IsRed(right_->right_);
            if (rra) { // 情况2，直接修复
                right_->FixEraseDyeLR(root_node);// 修复成功
            } else if (rla) {// 情况3
                right_->color_ = RED;
                right_->left_->color_ = BLACK;
                right_->left_->RR(root_node);
                FixLeft(root_node);// 递归修复
            } else {// 情况4
                right_->color_ = RED;
                // 染红后左右子hb平衡了, 但对于父节点来说，我的hb比兄弟hb少1
                if (ParentIsNull()) {// 我是根 没有兄弟，直接染黑
                    color_ = BLACK;
                } else if (AmILeft()) {// 我是左子
                    parent_->FixLeft(root_node);// 递归修复
                } else {// 我是右子
                    parent_->FixRight(root_node);// 递归修复
                }
            }
        }
    }

    template<typename k_t, typename v_t>
    void RBtreeNode<k_t, v_t>::FixRight(RBtreeNode::NodePtr_T &root_node) {
        // 当前节点的右子本来是黑色的被删了，要修复
        // 操作与FixLeft对称
        if (IsRed(right_)) {
            right_->color_ = BLACK;
            return;
        }

        if (IsRed(left_)) {
            left_->color_ = BLACK;
            color_ = RED;
            left_->RR(root_node);
            FixRight(root_node);
        } else {
            auto lla = IsRed(left_->left_);
            auto lra = IsRed(left_->right_);
            if (lla) {
                left_->FixEraseDyeRR(root_node);
            } else if (lra) {
                left_->color_ = RED;
                left_->right_->color_ = BLACK;
                left_->right_->LR(root_node);
                FixRight(root_node);
            } else {
                left_->color_ = RED;
                if (ParentIsNull()) {
                    color_ = BLACK;
                } else if (AmILeft()) {
                    parent_->FixLeft(root_node);
                } else {
                    parent_->FixRight(root_node);
                }
            }
        }
    }

    template<typename k_t, typename v_t>
    void RBtreeNode<k_t, v_t>::InorderTraversal(std::function<void(const Element_T &)> do_something) {
        if (left_ != nullptr) left_->InorderTraversal(do_something);
        do_something(data_);
        if (right_ != nullptr)right_->InorderTraversal(do_something);
    }

    template<typename k_t, typename v_t>
    void RBtreeNode<k_t, v_t>::FixEraseDyeLR(RBtreeNode::NodePtr_T &root_node) {
        color_ = parent_->color_;
        right_->color_ = BLACK;
        parent_->color_ = BLACK;
        LR(root_node);
        root_node->color_ = BLACK;
    }

    template<typename k_t, typename v_t>
    void RBtreeNode<k_t, v_t>::FixEraseDyeRR(RBtreeNode::NodePtr_T &root_node) {
        color_ = parent_->color_;
        left_->color_ = BLACK;
        parent_->color_ = BLACK;
        RR(root_node);
        root_node->color_ = BLACK;
    }

    template<typename k_t, typename v_t>
    bool RBtreeNode<k_t, v_t>::IsRed(RBtreeNode::NodePtr_T node_ptr) {
        if (node_ptr == nullptr) {
            return false;
        }
        return node_ptr->color_ == RED;
    }


}// namespace VgdStd
#endif //RBTREE_RBTREENODE_IMPLEMENTATION_H
