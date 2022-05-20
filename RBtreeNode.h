//
// Created by 12552 on 2022/5/19.
//

#ifndef RBTREE_RBTREENODE_H
#define RBTREE_RBTREENODE_H
namespace vgd {

    template<typename k_t, typename v_t>
    class RBtree;

    template<typename k_t, typename v_t>
    class RBtreeNode {
    public:
        std::pair<k_t, v_t> GetPair() { return this->_data; }

        RBtreeNode() = delete;

        RBtreeNode(const RBtreeNode &) = delete;

        RBtreeNode &operator=(const RBtreeNode &) = delete;

    private:


        using NodePtr_T = RBtreeNode<k_t, v_t> *; // 自身指针类型
        using Element_T = std::pair<k_t, v_t>; // 元素类型
        static void loopPrint(std::vector<NodePtr_T> &);

        friend class RBtree<k_t, v_t>;

        enum Color {
            RED, BLACK
        };

        RBtreeNode(k_t k, v_t v, Color c) : _data({k, v}),
                                            _color(c),
                                            _left(nullptr),
                                            _right(nullptr),
                                            _parent(nullptr) {};

        inline bool dataKeyEqual(const Element_T &e) const { return _data.first == e.first; }

        inline bool dataKeyComp(const Element_T &e) const { return _data.first < e.first; }

        inline bool parentIsNull() { return _parent == nullptr; }

        inline bool amILeft() { return _parent->_left == this; }

        void printKey() const;

        void sort(NodePtr_T &);

        // 五个成员： 左子 右子 双亲 数据 颜色
        NodePtr_T _left;
        NodePtr_T _right;
        NodePtr_T _parent;
        Element_T _data;
        Color _color;

        std::pair<bool, NodePtr_T> tryExchangeColor(); // 尝试交换颜色色
        std::pair<bool, NodePtr_T> tryLR(NodePtr_T &); // 尝试左旋
        NodePtr_T NoDyeLR(); // 非染色左旋
        NodePtr_T DyeLR(NodePtr_T &); // 染色左旋

        std::pair<bool, NodePtr_T> tryRR(NodePtr_T &); // 尝试右旋
        NodePtr_T NoDyeRR(); // 非染色右旋
        NodePtr_T DyeRR(NodePtr_T &); // 染色右旋
    };

    template<typename k_t, typename v_t>
    void RBtreeNode<k_t, v_t>::sort(NodePtr_T &rootNode) {
        if (parentIsNull()) { // 自己是根节点
            _color = BLACK;
            return;
        }
        if (_parent->_color == BLACK) { // 双亲为黑，不用调整
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
        if (_parent == nullptr) {
            std::cout << " {NULL-" << _data.first << " " << (_color == 0 ? "红" : "黑") << "} ";
        } else {
            std::cout << " {" << _parent->_data.first << "-" << _data.first << " " << (_color == 0 ? "红" : "黑") << "} ";
        }
    }

    template<typename k_t, typename v_t>
    void RBtreeNode<k_t, v_t>::loopPrint(std::vector<NodePtr_T> &queque) {
        if (queque.empty()) return;
        std::vector<NodePtr_T> newQueque;

        for (auto node: queque) {
            if (node != nullptr) {
                node->printKey();
                newQueque.emplace_back(node->_left);
                newQueque.emplace_back(node->_right);
            }
        }
        std::cout << std::endl;
        loopPrint(newQueque);
        return;
    }

#ifndef PP_AND_PPP_NODE
#define PP_AND_PPP_NODE
#define PP_NODE  _parent->_parent // 爷节点
#define PPP_NODE  _parent->_parent->_parent // 爷节点的双亲节点
#endif // PP_NODE

    template<typename k_t, typename v_t>
    std::pair<bool, typename RBtreeNode<k_t, v_t>::NodePtr_T> RBtreeNode<k_t, v_t>::tryExchangeColor() {
        assert(!parentIsNull() && !_parent->parentIsNull()); // 根据红黑树的性质，默认为真
        // 染色的条件：自身为红色，双亲的兄弟为红色
        if (PP_NODE != nullptr &&
            PP_NODE->_left != nullptr && PP_NODE->_left->_color == RED &&
            PP_NODE->_right != nullptr && PP_NODE->_right->_color == RED) {
            // 染色:颜色交换,保证双亲的双亲以下满足红黑树
            PP_NODE->_color = RED;
            PP_NODE->_left->_color = BLACK;
            PP_NODE->_right->_color = BLACK;
            // 返回双亲的双亲
            return std::pair<bool, NodePtr_T>({true, PP_NODE});// 染色成功,返回下一个操作节点
        }
        return std::pair<bool, NodePtr_T>({false, nullptr});// 不能染色
    }

    template<typename k_t, typename v_t>
    std::pair<bool, typename RBtreeNode<k_t, v_t>::NodePtr_T> RBtreeNode<k_t, v_t>::tryLR(NodePtr_T &rootNode) {
        assert(!parentIsNull() && !_parent->parentIsNull()); // 根据红黑树的性质，默认为真
        // 左旋的条件：自身为父的右节点
        if (_parent->_right == this) {// 父为爷的左节点
            if (_parent->amILeft()) {
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
        if (_left != nullptr) {
            _left->_parent = _parent;
        }
        _parent->_right = _left;

        auto oldP = _parent; //记录父节点

        // 自己代替父节点变爷的左子
        this->_parent = PP_NODE;
        _parent->_left = this;

        //父子树代替自己的左子树
        oldP->_parent = this;
        _left = oldP;

        return _left;// 左旋成功,返回下一个操作节点
    }

    template<typename k_t, typename v_t>
    typename RBtreeNode<k_t, v_t>::NodePtr_T RBtreeNode<k_t, v_t>::DyeLR(NodePtr_T &rootNode) {
        // 先调整颜色
        _parent->_color = BLACK;
        PP_NODE->_color = RED;

        // 记录节点 老节点自身不变，父子指针会变
        auto oldBor = _parent->_left;
        auto oldP = _parent;
        auto oldPP = PP_NODE;
        auto oldPPP = PPP_NODE;

        // 左兄弟代替父变爷的右子
        if (oldBor != nullptr) {
            oldBor->_parent = oldPP;
        }
        oldPP->_right = oldBor;

        // 父代替爷变"爷父"的左或右子
        if (oldPPP != nullptr) {
            if (oldPPP->_left == oldPP) {
                oldPPP->_left = oldP;
            } else {
                oldPPP->_right = oldP;
            }
        } else {
            rootNode = oldP;
        }
        oldP->_parent = oldPPP;

        // 爷代替父的左子
        oldPP->_parent = oldP;
        oldP->_left = oldPP;

        return oldP->_left;
    }


    template<typename k_t, typename v_t>
    std::pair<bool, typename RBtreeNode<k_t, v_t>::NodePtr_T> RBtreeNode<k_t, v_t>::tryRR(NodePtr_T &rootNode) {
        assert(!parentIsNull() && !_parent->parentIsNull()); // 根据红黑树的性质，默认为真
        // 右旋的条件：自身为父的左节点
        if (_parent->_left == this) {// 父为爷的左节点
            if (_parent->amILeft()) {
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
        if (_right != nullptr) {
            _right->_parent = _parent;
        }
        _parent->_left = _right;

        auto oldP = _parent;

        this->_parent = PP_NODE;
        _parent->_right = this;

        oldP->_parent = this;
        _right = oldP;

        return _right;// 右旋成功,返回下一个操作节点
    }


    template<typename k_t, typename v_t>
    typename RBtreeNode<k_t, v_t>::NodePtr_T RBtreeNode<k_t, v_t>::DyeRR(NodePtr_T &rootNode) {
        // 操作同染色左旋对称
        _parent->_color = BLACK;
        PP_NODE->_color = RED;

        auto oldPPP = PP_NODE->_parent;// 记录爷的父节点
        auto oldPP = PP_NODE;// 记录爷节点
        auto oldP = _parent;//记录父节点
        auto oldBro = _parent->_right;//记录父节点的右节点，即兄弟节点

        if (oldBro != nullptr) {
            oldBro->_parent = oldPP;
        }
        oldPP->_left = oldBro;

        if (oldPPP != nullptr) {
            if (oldPPP->_left == oldPP) {
                oldPPP->_left = oldP;
            } else {
                oldPPP->_right = oldP;
            }
        } else {
            rootNode = oldP;
        }
        oldP->_parent = oldPPP;

        oldPP->_parent = oldP;
        oldP->_right = oldPP;

        return oldP->_right;
    }


} // namespace vgd


#endif //RBTREE_RBTREENODE_H
