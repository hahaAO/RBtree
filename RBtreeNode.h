//
// Created by 12552 on 2022/5/19.
//

#ifndef RBTREE_RBTREENODE_H
#define RBTREE_RBTREENODE_H
namespace VgdStd {

    template<typename k_t, typename v_t>
    class RBtree;

    template<typename k_t, typename v_t>
    class RBtreeNode {
    public:
        std::pair<k_t, v_t> GetPair() { return this->data_; }

        RBtreeNode() = delete;

        RBtreeNode(const RBtreeNode &) = delete;

        RBtreeNode &operator=(const RBtreeNode &) = delete;

    private:


        using NodePtr_T = RBtreeNode<k_t, v_t> *; // 自身指针类型
        using Element_T = std::pair<k_t, v_t>; // 元素类型

        static void loopPrint(std::vector<NodePtr_T> &);

        void inorder(std::function<void(const Element_T &)>);//中序遍历，即从小到大输出 ,可定制对元素的操作

        friend class RBtree<k_t, v_t>;

        enum Color {
            RED, BLACK
        };

        RBtreeNode(k_t k, v_t v, Color c) : data_({k, v}),
                                            color_(c),
                                            left_(nullptr),
                                            right_(nullptr),
                                            parent_(nullptr) {};

        inline bool dataKeyEqual(const Element_T &e) const { return data_.first == e.first; }

        inline bool dataKeyComp(const Element_T &e) const { return data_.first < e.first; }

        inline bool parentIsNull() { return parent_ == nullptr; }
#pragma GCC push_options
#pragma GCC optimize ("O0")
        inline bool AmIRed() { // 空节点等于黑色，判断颜色时用这个判断
            if (this == nullptr) {
                return false;
            }
            return color_ == RED;// todo: 禁用color的==符号
        }
#pragma GCC pop_options

        inline bool AmILeft() {
            assert(!parentIsNull());
            return parent_->left_ == this;
        }

        inline void ReplaceParent(NodePtr_T &root_node);

        void printKey() const;

        void sort(NodePtr_T &);


        // 以下是关键功能的实现
        // 五个成员： 左子 右子 双亲 数据 颜色
        NodePtr_T left_;
        NodePtr_T right_;
        NodePtr_T parent_;
        Element_T data_;
        Color color_;

        std::pair<bool, NodePtr_T> tryExchangeColor(); // 尝试交换颜色
        std::pair<bool, NodePtr_T> tryLR(NodePtr_T &); // 尝试左旋
        NodePtr_T NoDyeLR(); // 非染色左旋
        NodePtr_T DyeLR(NodePtr_T &); // 染色左旋

        std::pair<bool, NodePtr_T> tryRR(NodePtr_T &); // 尝试右旋
        NodePtr_T NoDyeRR(); // 非染色右旋
        NodePtr_T DyeRR(NodePtr_T &); // 染色右旋

        NodePtr_T LR(NodePtr_T &root_node); // 通用左旋,不检查旋转后是否破坏性质
        NodePtr_T RR(NodePtr_T &root_node); // 通用右旋,不检查旋转后是否破坏性质

        inline NodePtr_T CheckLR(NodePtr_T &root_node) {// 左旋，我与父同色时可用,保证红黑树性质不被破坏
            assert(!parentIsNull() && parent_->color_ == color_);
            LR(root_node);
        }

        inline NodePtr_T CheckRR(NodePtr_T &root_node) {  // 右旋，我与父同色时可用,保证红黑树性质不被破坏
            assert(!parentIsNull() && parent_->color_ == color_);
            RR(root_node);
        }

        inline void ExchangeColorAndLR(NodePtr_T &root_node) {// 换色左旋
            color_ = parent_->color_;
            right_->color_ = BLACK;
            parent_->color_ = BLACK;
            LR(root_node);
            root_node->color_ = BLACK;
        }

        inline void ExchangeColorAndRR(NodePtr_T &root_node) { // 换色右旋
            color_ = parent_->color_;
            left_->color_ = BLACK;
            parent_->color_ = BLACK;
            RR(root_node);
            root_node->color_ = BLACK;
        }

        void Erase(NodePtr_T &root_node);

        // 删除后，不平衡节点的修复
        void FixLeft(NodePtr_T &root_node);

        void FixRight(NodePtr_T &root_node);
    };
} // namespace VgdStd

#include "RBtreeNode_implementation.h"

#endif //RBTREE_RBTREENODE_H
