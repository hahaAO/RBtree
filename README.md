# C++实现红黑树

* c++14

* 拥有插入、删除、查找功能

* 暂未支持范围查找

不开优化情况下时间性能比标准库的快不到一倍,偶尔不相上下，内存占用未测试。
----
插入：

|        | 插入两千次 | 插入二十万次 | 插入两百万次 |
| :----- | :--------: | :----------: | :----------: |
| std    | 0.002351秒 |  0.599616秒  |  4.61869秒   |
| VgdStd | 0.001193秒 |  0.389696秒  |  3.28635秒   |

查找：

|        | 查找两千次 | 查找二十万次 | 查找两百万次 |
| ------ | :--------: | :----------: | :----------: |
| std    | 0.000922秒 |  0.598996秒  |  4.00556秒   |
| VgdStd | 0.008848秒 |  0.209669秒  |  2.49703秒   |

删除：

|        | 删除两千次 | 删除二十万次 | 删除两百万次 |
| ------ | :--------: | :----------: | :----------: |
| std    | 0.004074秒 |  0.517413秒  |  4.58272秒   |
| VgdStd | 0.001235秒 |  0.384714秒  |  3.47427秒   |

----
主要参考算法导论13章的红黑树实现，具体细节有所不同。

#### 红黑树特性

1.  节点左小右大（二叉排序树）
2. 节点颜色为红或黑
3. 根节点为黑
4. 每个节点到所有叶子节点的路径上的黑色节点数量相等，即 左BH(x)==右BH(x)
5. 红色节点之间不直接接触（红色节点的父子必为黑节点）
6. 空节点视为黑，为方便实现可以把空节点设为一个特殊的黑色的节点（本项目并没有使用这种方法）

这里不作数学证明，只提供实现。总之，保证以上特性可以使查找、插入、删除操作都有log（N）的时间复杂度。

#### 重要概念

两个父子关系的节点可以做左旋、右旋，保证不破坏大小排序，但会破坏颜色特性，所以要通过染色、换色达到维持或修复红黑树性质的目的

* 左旋：原本子是父的右节点，旋转后子变父，父变子的左节点
* 右旋：原本子是父的左节点，旋转后子变父，父变子的右节点

#### 查找实现：

与平衡二叉树一样，key值与当前节点对比，小则往左遍历，大则往右遍历，直至空节点

#### 插入实现：

插入节点先根据排序原则插入。并设置为红色；
插入节点后可能破坏了性质3、5；
修复手段看代码这些函数的实现

```c++
void FixInsert(NodePtr_T &root_node);// 用于插入后的修复
std::pair<bool, NodePtr_T> TryExchangeColor(); // 尝试交换颜色
void tryLR(NodePtr_T &root_node); // 尝试左旋
inline void FixInsertDyeLR(NodePtr_T &root_node); // 红底染色左旋,不改变红黑树性质
void tryRR(NodePtr_T &root_node); // 尝试右旋
inline void FixInsertDyeRR(NodePtr_T &root_node); // 红底染色右旋,不改变红黑树性质
```



#### 删除实现：

删除节点时如果左右为空，可直接删除；
如果左或右子为空，可删除一边，另一边补上；
如果左右节点都存在，在右子树中找个最小的节点代替该节点被删除。

删除操作不破坏大小排序，只破坏3、4、5特性；
修复手段看代码这些函数的实现

```c++
void Erase(NodePtr_T &root_node);// 删除节点
void FixLeft(NodePtr_T &root_node);// 左边比右边hb少一，需要修复
void FixRight(NodePtr_T &root_node);// 右边比左边hb少一，需要修复
inline void FixEraseDyeLR(NodePtr_T &root_node);// 黑底染色左旋，使兄弟节点hb加一
inline void FixEraseDyeRR(NodePtr_T &root_node);// 黑底染色右旋，使兄弟节点hb加一
```