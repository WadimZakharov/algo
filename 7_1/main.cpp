/*
Солдаты. В одной военной части решили построить в одну шеренгу по росту. Т.к. часть была далеко не образцовая,
то солдаты часто приходили не вовремя, а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие, а в конце – самые низкие.
За расстановку солдат отвечал прапорщик, который заметил интересную особенность – все солдаты в части разного роста.
Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, а именно для каждого приходящего солдата
указывать, перед каким солдатом в строе он должен становится.
Вариант 7_1.  Требуемая скорость выполнения команды - O(log n) в среднем. Для решения задачи реализуйте декартово дерево.
*/
#include <iostream>
#include <queue>
using namespace std;
struct TreeNode
{
    explicit TreeNode(int _value, int _priority) : value(_value), priority(_priority) {}
    int value;
    int priority;
    int count=1;
    TreeNode *left=nullptr;
    TreeNode *right=nullptr;
};
class Treap
{
public:
    int AddNode(int value);
    void DeleteNode(int index);
    ~Treap();
    Treap(){};
    Treap(const Treap&) = delete;
    Treap(Treap&&) = delete;
    Treap& operator=(const Treap&) = delete;
    Treap& operator=(Treap&&) = delete;

private:
    TreeNode *root = nullptr;
    TreeNode *Merge(TreeNode *left, TreeNode *right);
    void Split(TreeNode *node, int key, TreeNode **left, TreeNode **right);
    int Addnode(TreeNode *node, int value, int priority);
    int AddNode(int value, int priority);
    void Deletenode(TreeNode **node, int index);
    void Fixcount(TreeNode* &node);
};

void Treap::Fixcount(TreeNode* &node)
{
    node->count = 1;
    if (node->left)
        node->count += node->left->count;
    if (node->right)
        node->count += node->right->count;
}


Treap::~Treap()
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

TreeNode* Treap::Merge(TreeNode *left, TreeNode *right) {
    if (!left || !right)
    {
        if (!left)
        {
            return right;
        }
        return left;
    }
    else if (left->priority > right->priority)
    {
        left->right = Merge(left->right, right);
        if (left)
            Fixcount(left);
        return left;
    }
    else
    {
        right->left = Merge(left, right->left);
        if (right)
            Fixcount(right);
        return right;
    }
}

void Treap::Split(TreeNode *node, int key, TreeNode **left, TreeNode **right)
{
    if (!node)
    {
        *left = nullptr;
        *right = nullptr;
    }
    else if (node->value < key)
    {
        Split(node->left, key, left, &node->left);
        *right = node;
    }
    else
    {
        Split(node->right, key, &node->right, right);
        *left = node;
    }
    if (*left)
        Fixcount(*left);
    if (*right)
        Fixcount(*right);
}


void Treap::DeleteNode(int index)
{
    if (root)
        Deletenode(&root, index);
}

void Treap::Deletenode(TreeNode **node, int index) {
    TreeNode *_node = (*node);
    int left_count = 0;
    if (_node->left)
        left_count = _node->left->count;
    if (index == left_count) {
        *node = Merge(_node->left, _node->right);
        delete _node;
    }
    else if (index < left_count)
    {
        Deletenode(&_node->left, index);
        Fixcount(_node);
    }
    else
    {
        Deletenode(&_node->right, index - left_count - 1);
        Fixcount(_node);
    }
}

int Treap::AddNode(int val)
{
    return AddNode(val, rand());
}

int Treap::AddNode(int val, int priority)
{
    if (!root)
    {
        root = new TreeNode(val, priority);
        return 0;
    }
    else
    {
        return Addnode(root, val, priority);
    }
}

int Treap::Addnode(TreeNode *node, int value, int priority)
{
    TreeNode **_node;
    int position = 0;
    if (value > node->value)
    {
        _node = &node->left;
    }
    else
    {
        _node = &node->right;
        if (node->left)
            position = 1 + node->left->count;
        else
            position = 1;
    }

    if ((*_node) && (*_node)->priority >= priority)
    {
        position += Addnode(*_node, value, priority);
        Fixcount(node);
        return position;
    }
    TreeNode *fict_node = new TreeNode(value, priority);
    if (*_node)
    {
        Split(*_node, value, &fict_node->left, &fict_node->right);
        if (fict_node->left)
        {
            position += fict_node->left->count;
        }
        Fixcount(fict_node);
    }
    *_node = fict_node;
    Fixcount(node);
    return position;
}
int main() {
    Treap treap;
    int n = 0;
    int comand = 0;
    int value = 0;
    cin >> n;
    for(int i=0; i<n; i++)
    {
        cin >> comand >> value;
        if (comand == 1)
        {
            int position = treap.AddNode(value);
            cout << position << endl;
        }
        else
        {
            treap.DeleteNode(value);
        }
    }
    return 0;
}
