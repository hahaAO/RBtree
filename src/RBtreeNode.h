//
// Created by 12552 on 2022/5/19.
//

#ifndef RBTREE_RBTREENODE_H
#define RBTREE_RBTREENODE_H
namespace VgdStd {

    enum class RBtreeNodeColor {
        RED, BLACK
    };

    template<typename k_t, typename v_t>
    class RBtree;

    template<typename k_t, typename v_t>
    class RBtreeNode {
    private:
        using NodePtr_T = RBtreeNode<k_t, v_t> *; // 自身指针类型
        using Element_T = std::pair<k_t, v_t>; // 元素类型
        using Color = RBtreeNodeColor; // 可避免多份模板实例后产生多个枚举
#define RED Color::RED
#define BLACK Color::BLACK


    public:
        std::pair<k_t, v_t> GetPair() { return data_; }

        RBtreeNode() = delete;

        RBtreeNode(const RBtreeNode &) = delete;

        RBtreeNode &operator=(const RBtreeNode &) = delete;

    private:
        static void LevelTraversal(std::vector<NodePtr_T> &, std::function<void(const NodePtr_T &)>);//层序遍历，可定制对元素的操作


        inline static bool IsRed(NodePtr_T node_ptr);// 空节点等于黑色，判断颜色时用这个判断


        void InorderTraversal(std::function<void(const Element_T &)>);//中序遍历，即从小到大输出,可定制对元素的操作

        friend class RBtree<k_t, v_t>;


        RBtreeNode(k_t k, v_t v, Color c) : data_({k, v}),
                                            color_(c),
                                            left_(nullptr),
                                            right_(nullptr),
                                            parent_(nullptr) {};

        inline bool DataKeyEqual(const Element_T &e) const { return data_.first == e.first; }

        inline bool DataKeyComp(const Element_T &e) const { return data_.first < e.first; }

        inline bool ParentIsNull() { return parent_ == nullptr; }

        inline bool AmILeft() {
            assert(!ParentIsNull());
            return parent_->left_ == this;
        }

        inline void ReplaceParent(NodePtr_T &root_node);

        void PrintKey() const;


        // 以下是关键功能的实现
        // 五个成员： 左子 右子 双亲 数据 颜色
        NodePtr_T left_;
        NodePtr_T right_;
        NodePtr_T parent_;
        Element_T data_;
        Color color_;

        NodePtr_T LR(NodePtr_T &root_node); // 通用左旋,不检查旋转后是否破坏性质
        NodePtr_T RR(NodePtr_T &root_node); // 通用右旋,不检查旋转后是否破坏性质

        void FixInsert(NodePtr_T &root_node);// 用于插入后的修复
        std::pair<bool, NodePtr_T> TryExchangeColor(); // 尝试交换颜色
        void tryLR(NodePtr_T &root_node); // 尝试左旋
        inline void FixInsertDyeLR(NodePtr_T &root_node); // 红底染色左旋,不改变红黑树性质
        void tryRR(NodePtr_T &root_node); // 尝试右旋
        inline void FixInsertDyeRR(NodePtr_T &root_node); // 红底染色右旋,不改变红黑树性质

        void Erase(NodePtr_T &root_node);// 删除节点
        void FixLeft(NodePtr_T &root_node);// 左边比右边hb少一，需要修复
        void FixRight(NodePtr_T &root_node);// 右边比左边hb少一，需要修复
        inline void FixEraseDyeLR(NodePtr_T &root_node);// 黑底染色左旋，使兄弟节点hb加一
        inline void FixEraseDyeRR(NodePtr_T &root_node);// 黑底染色右旋，使兄弟节点hb加一
    };
} // namespace VgdStd

#include "RBtreeNode_implementation.h"

#endif //RBTREE_RBTREENODE_H
