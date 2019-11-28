/*
 Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
 Требуется построить бинарное дерево, заданное наивным порядком вставки.
 Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
 то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
 Рекурсия запрещена.
6_3. Выведите элементы в порядке post-order (снизу вверх).
 */
#include <iostream>
#include <stack>
#include <queue>
using namespace std;

class Tree
{
public:
    ~Tree();
    Tree(){};
    Tree(const Tree&) = delete;
    Tree(Tree&&) = delete;
    Tree& operator=(const Tree&) = delete;
    Tree& operator=(Tree&&) = delete;
    queue<int> get_post_order() const;
    void AddNode(int value);
private:
    struct TreeNode
    {
        explicit TreeNode(int _value) : value(_value) {}
        int value = 0;
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
    };

    TreeNode *root = nullptr;
    queue<int> postorder(TreeNode *node) const;
};


Tree::~Tree()
{
    if (root)
    {
        queue<TreeNode*> que;
        que.push(root);
        while (!que.empty())
        {
            TreeNode* node = que.front();
            que.pop();
            if (node->left) {
                que.push(node->left);
            }
            if (node->right) {
                que.push(node->right);
            }
            delete node;
        }
    }
}


void Tree::AddNode(int value)
{
    if (!root)
        root = new TreeNode(value);
    else
    {
        TreeNode* node = root;
        while(true)
        {
            if (value >= node->value)
            {
                if (!node->right)
                {
                    node->right = new TreeNode(value);
                    return;
                }
                node = node->right;
            }
            else
            {
                if (!node->left)
                {
                    node->left = new TreeNode(value);
                    return;
                }
                node = node->left;
            }
        }
    }
}

queue<int> Tree::get_post_order() const
{
    return postorder(root);
}



queue<int> Tree::postorder(TreeNode *node) const
{
    queue<int> que;
    if (node)
    {
        stack<TreeNode*> stack;
        TreeNode *head = nullptr;
        TreeNode *last_node = nullptr;
        while (node || !stack.empty())
        {
            if (node) {
                stack.push(node);
                node = node->left;
                continue;
            }
            head = stack.top();
            if (head->right && last_node != head->right)
            {
                node = head->right;
            }
            else
            {
                que.push(head->value);
                last_node = head;
                stack.pop();
            }
        }
    }
    return que;
}

int main()
{
    int n = 0;
    cin >> n;
    Tree tree;
    for (int i = 0; i < n; i++)
    {
        int value = 0;
        cin >> value;
        tree.AddNode(value);
    }

    auto qpost_order_queue = tree.get_post_order();
    for(int i=0; i<n; i++)
    {
        std::cout << qpost_order_queue.front() << " ";
        qpost_order_queue.pop();
    }
    return 0;
}
