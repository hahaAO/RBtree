//
// Created by 12552 on 2022/5/20.
//

#ifndef RBTREE_RBTREENODE_IMPLEMENTATION_H
#define RBTREE_RBTREENODE_IMPLEMENTATION_H


#include "RBtreeNode.h"

#define PP_NODE  parent_->parent_ // 爷节点
#define PPP_NODE  parent_->parent_->parent_ // 爷节点的双亲节点

namespace VgdStd {
    template<typename k_t, typename v_t>
    inline void RBtreeNode<k_t, v_t>::ReplaceParent(NodePtr_T &root_node) {
        // 该函数用于方便地替换父节点，把自己的父节点指向爷节点，爷节点的子指向自己
        // 注意不会改动原父节点和自己的子节点

        assert(!parentIsNull());
        if (parent_->parentIsNull()) {// 爷节点为空
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
    void RBtreeNode<k_t, v_t>::sort(NodePtr_T &rootNode) {
        if (parentIsNull()) { // 自己是根节点
            color_ = BLACK;
            return;
        }
        if (parent_->color_ == BLACK) { // 双亲为黑，不用调整
            return;
        }

        auto res = tryExchangeColor();
        if (res.first) { // 染色成功，继续调整
            res.second->sort(rootNode);
            return;
        }

        res = tryLR(rootNode);
        if (res.first) { // 左旋成功，继续调整
            res.second->sort(rootNode);
            return;
        }

        res = tryRR(rootNode);
        if (res.first) { // 右旋成功，继续调整
            res.second->sort(rootNode);
            return;
        }

        throw "不可能！排序算法写错了！";
    }

    template<typename k_t, typename v_t>
    void RBtreeNode<k_t, v_t>::printKey() const {
        if (parent_ == nullptr) {
            std::cout << " {NULL-" << data_.first << " " << (color_ == 0 ? "红" : "黑") << "} ";
        } else {
            std::cout << " {" << parent_->data_.first << "-" << data_.first << " " << (color_ == 0 ? "红" : "黑") << "} ";
        }
    }

    template<typename k_t, typename v_t>
    void RBtreeNode<k_t, v_t>::loopPrint(std::vector<NodePtr_T> &queque) {
        if (queque.empty()) return;
        std::vector<NodePtr_T> newQueque;

        for (auto node: queque) {
            if (node != nullptr) {
                node->printKey();
                newQueque.emplace_back(node->left_);
                newQueque.emplace_back(node->right_);
            }
        }
        std::cout << std::endl;
        loopPrint(newQueque);
        return;
    }


    template<typename k_t, typename v_t>
    std::pair<bool, typename RBtreeNode<k_t, v_t>::NodePtr_T> RBtreeNode<k_t, v_t>::tryExchangeColor() {
        assert(!parentIsNull() && !parent_->parentIsNull()); // 根据红黑树的性质，默认为真
        // 染色的条件：自身为红色，双亲的兄弟为红色
        if (PP_NODE != nullptr &&
            PP_NODE->left_ != nullptr && PP_NODE->left_->color_ == RED &&
            PP_NODE->right_ != nullptr && PP_NODE->right_->color_ == RED) {
            // 染色:颜色交换,保证双亲的双亲以下满足红黑树
            PP_NODE->color_ = RED;
            PP_NODE->left_->color_ = BLACK;
            PP_NODE->right_->color_ = BLACK;
            // 返回双亲的双亲
            return std::pair<bool, NodePtr_T>({true, PP_NODE});// 染色成功,返回下一个操作节点
        }
        return std::pair<bool, NodePtr_T>({false, nullptr});// 不能染色
    }

    template<typename k_t, typename v_t>
    std::pair<bool, typename RBtreeNode<k_t, v_t>::NodePtr_T> RBtreeNode<k_t, v_t>::tryLR(NodePtr_T &rootNode) {
        assert(!parentIsNull() && !parent_->parentIsNull()); // 根据红黑树的性质，默认为真
        // 左旋的条件：自身为父的右节点
        if (parent_->right_ == this) {// 父为爷的左节点
            if (parent_->AmILeft()) {
                auto nextNode = NoDyeLR(); // 触发非染色左旋
                return std::pair<bool, NodePtr_T>({true, nextNode});
            } else {// 父为爷的右节点
                auto nextNode = DyeLR(rootNode); // 触发染色左旋
                return std::pair<bool, NodePtr_T>({true, nextNode});
            }
        }
        return std::pair<bool, NodePtr_T>({false, nullptr});// 不能左旋
    }

    template<typename k_t, typename v_t>
    typename RBtreeNode<k_t, v_t>::NodePtr_T RBtreeNode<k_t, v_t>::NoDyeLR() {
        // 左子代替自己变父的右子
        if (left_ != nullptr) {
            left_->parent_ = parent_;
        }
        parent_->right_ = left_;

        auto oldP = parent_; //记录父节点

        // 自己代替父节点变爷的左子
        this->parent_ = PP_NODE;
        parent_->left_ = this;

        //父子树代替自己的左子树
        oldP->parent_ = this;
        left_ = oldP;

        return left_;// 左旋成功,返回下一个操作节点
    }

    template<typename k_t, typename v_t>
    typename RBtreeNode<k_t, v_t>::NodePtr_T RBtreeNode<k_t, v_t>::DyeLR(NodePtr_T &rootNode) {
        // 先调整颜色
        parent_->color_ = BLACK;
        PP_NODE->color_ = RED;

        // 记录节点 老节点自身不变，父子指针会变
        auto oldBor = parent_->left_;
        auto oldP = parent_;
        auto oldPP = PP_NODE;
        auto oldPPP = PPP_NODE;

        // 左兄弟代替父变爷的右子
        if (oldBor != nullptr) {
            oldBor->parent_ = oldPP;
        }
        oldPP->right_ = oldBor;

        // 父代替爷变"爷父"的左或右子
        parent_->ReplaceParent(rootNode);

        // 爷代替父的左子
        oldPP->parent_ = oldP;
        oldP->left_ = oldPP;

        return oldP->left_;
    }


    template<typename k_t, typename v_t>
    std::pair<bool, typename RBtreeNode<k_t, v_t>::NodePtr_T> RBtreeNode<k_t, v_t>::tryRR(NodePtr_T &rootNode) {
        assert(!parentIsNull() && !parent_->parentIsNull()); // 根据红黑树的性质，默认为真
        // 右旋的条件：自身为父的左节点
        if (parent_->left_ == this) {// 父为爷的左节点
            if (parent_->AmILeft()) {
                auto nextNode = DyeRR(rootNode); // 触发染色右旋
                return std::pair<bool, NodePtr_T>({true, nextNode});
            } else {// 父为爷的右节点
                auto nextNode = NoDyeRR(); // 触发非染色右旋
                return std::pair<bool, NodePtr_T>({true, nextNode});
            }
        }
        return std::pair<bool, NodePtr_T>({false, nullptr});// 不能右旋
    }


    template<typename k_t, typename v_t>
    typename RBtreeNode<k_t, v_t>::NodePtr_T RBtreeNode<k_t, v_t>::NoDyeRR() {
        // 跟非染色左旋正好对称的操作
        if (right_ != nullptr) {
            right_->parent_ = parent_;
        }
        parent_->left_ = right_;

        auto oldP = parent_;

        this->parent_ = PP_NODE;
        parent_->right_ = this;

        oldP->parent_ = this;
        right_ = oldP;

        return right_;// 右旋成功,返回下一个操作节点
    }


    template<typename k_t, typename v_t>
    typename RBtreeNode<k_t, v_t>::NodePtr_T RBtreeNode<k_t, v_t>::DyeRR(NodePtr_T &rootNode) {
        // 操作同染色左旋对称
        parent_->color_ = BLACK;
        PP_NODE->color_ = RED;

        auto oldPPP = PP_NODE->parent_;// 记录爷的父节点
        auto oldPP = PP_NODE;// 记录爷节点
        auto oldP = parent_;//记录父节点
        auto oldBro = parent_->right_;//记录父节点的右节点，即兄弟节点

        if (oldBro != nullptr) {
            oldBro->parent_ = oldPP;
        }
        oldPP->left_ = oldBro;

        parent_->ReplaceParent(rootNode);

        oldPP->parent_ = oldP;
        oldP->right_ = oldPP;

        return oldP->right_;
    }

    template<typename k_t, typename v_t>
    void RBtreeNode<k_t, v_t>::Erase(RBtreeNode::NodePtr_T &root_node) {
        auto original_color = color_;// 记录被删除的节点的 color，如果是黑色说明破坏了black-height稳定限制，需要修复性质
        NodePtr_T move_node = nullptr;// 记录移动的节点，该节点的红黑树性质有可能被破坏，如有需要从该节点开始修复
        std::function<void()> Fixup;     // 记录要调用的修复函数
        if (parentIsNull()) {
            Fixup = []() {};// 如果根节点被删了，不需要修复
        } else {
            if (AmILeft()) {
                Fixup = std::bind(&RBtreeNode<k_t, v_t>::FixLeft, parent_, std::ref(root_node));
            } else {
                Fixup = std::bind(&RBtreeNode<k_t, v_t>::FixRight, parent_, std::ref(root_node));
            }
        }


        if (left_ == nullptr) {
            if (right_ == nullptr) {
                if (parentIsNull()) {// 就剩一个节点，直接删除
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
                move_node = right_;
                move_node->ReplaceParent(root_node);
                delete this;
            }
        } else if (right_ == nullptr) {
            // 右子树为空,直接把左子树提上来
            move_node = left_;
            move_node->ReplaceParent(root_node);
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

        if (original_color == BLACK) {// 需要修复
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
        if (left_->AmIRed()) {
            left_->color_ = BLACK;
            return;
        }

        if (right_->AmIRed()) {
            right_->color_ = BLACK;
            color_ = RED;
            right_->LR(root_node);
            FixLeft(root_node);//右子左旋后，右子的左子变我右子，右边bh仍然比左边高1,递归修复
        } else {
            auto rla = right_->left_->AmIRed();
            auto rra = right_->right_->AmIRed();
            if (rra) { // 右子的右子为红，符合情况2，直接修复
                right_->ExchangeColorAndLR(root_node);// 修复成功
            } else if (rla) {// 右子的左子为红，换色左旋
                right_->color_ = RED;
                right_->left_->color_ = BLACK;
                right_->left_->RR(root_node);
                FixLeft(root_node);// 递归修复
            } else {// 右子的双子为黑,直接把右子染红
                right_->color_ = RED;
                // 染红后左右子hb平衡了, 但对于父节点来说，我的hb比兄弟hb少1
                if (parentIsNull()) {// 我是根 没有兄弟，直接染黑
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
        if (right_->AmIRed()) {
            right_->color_ = BLACK;
            return;
        }

        if (left_->AmIRed()) {
            left_->color_ = BLACK;
            color_ = RED;
            left_->RR(root_node);
            FixRight(root_node);
        } else {
            auto lla = left_->left_->AmIRed();
            auto lra = left_->right_->AmIRed();
            if (lla) {
                left_->ExchangeColorAndRR(root_node);
            } else if (lra) {
                left_->color_ = RED;
                left_->right_->color_ = BLACK;
                left_->right_->LR(root_node);
                FixRight(root_node);
            } else {
                left_->color_ = RED;
                if (parentIsNull()) {
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
    void RBtreeNode<k_t, v_t>::inorder(std::function<void(const Element_T &)> do_something) {
        if (left_ != nullptr) left_->inorder(do_something);
        do_something(data_);
        if (right_ != nullptr)right_->inorder(do_something);
    }


}// namespace VgdStd
#endif //RBTREE_RBTREENODE_IMPLEMENTATION_H
