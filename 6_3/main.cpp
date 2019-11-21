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
struct TreeNode
{
    explicit TreeNode(int _value) : value(_value) {}
    int value = 0;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};

class Tree
{
public:
    ~Tree();
    void Print() const;
    void AddNode(int value);
private:
    TreeNode *root = nullptr;
    void postorder(TreeNode *node) const;
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

void Tree::Print() const
{
    postorder(root);
}



void Tree::postorder(TreeNode *node) const
{
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
                cout << head->value << " ";
                last_node = head;
                stack.pop();
            }
        }
    }

}

int main()
{
    int n = 0;
    cin >> n;
    Tree* tree = new Tree;
    for (int i = 0; i < n; i++)
    {
        int value = 0;
        cin >> value;
        tree->AddNode(value);
    }

    tree->Print();
    delete tree;
    return 0;
}
