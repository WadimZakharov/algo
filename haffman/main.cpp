#include "Huffman.h"
#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
using namespace std;
class BitsWriter {
public:
	void WriteBit(bool bit);
	void WriteByte(unsigned char byte);
	std::vector<unsigned char> GetResult();

private:
	std::vector<unsigned char> buffer_;
	unsigned char accumulator_ = 0;
	int bits_count_ = 0;
};

void BitsWriter::WriteBit(bool bit) {
	// Ставим бит в аккумулятор на нужное место
	accumulator_ |= static_cast<unsigned char>(bit) << bits_count_;
	++bits_count_;
	if (bits_count_ == 8) {
		bits_count_ = 0;
		buffer_.push_back(accumulator_);
		accumulator_ = 0;
	}
}

void BitsWriter::WriteByte(unsigned char byte) {
	if (bits_count_ == 0) {
		buffer_.push_back(byte);
	}
	else {
		accumulator_ |= byte << bits_count_;
		buffer_.push_back(accumulator_);
		accumulator_ = byte >> (8 - bits_count_);
	}
}

std::vector<unsigned char> BitsWriter::GetResult() {
	if (bits_count_ != 0) {
		// Добавляем в буфер аккумулятор, если в нем что-то есть.
		buffer_.push_back(accumulator_);
	}
	buffer_.push_back(static_cast<unsigned char>(bits_count_));
	return move(buffer_);
}


struct TreeNode
{
	unsigned char symbol;
	int priority;
	vector<bool> code;
	TreeNode* left = nullptr;
	TreeNode* right = nullptr;
	explicit TreeNode(unsigned char _symbol, int _priority) : symbol(_symbol), priority(_priority) {}
	explicit TreeNode(int _priority) : priority(_priority) {}
};

struct my_less
{
	bool operator()(const TreeNode* left, const TreeNode* right) const
	{
		return left->priority > right->priority;
	}
};

class Haffman
{
public:
	Haffman() {};
	~Haffman();
	Haffman(const Haffman&) = delete;
	Haffman(Haffman&&) = delete;
	Haffman& operator=(const Haffman&) = delete;
	Haffman& operator=(Haffman&&) = delete;
	void HaffmanEncoder(string &original, IOutputStream& output);
	TreeNode * DecodeNode(vector<bool>& tree, size_t& index);
private:
	unordered_map<unsigned char, vector<bool>> encode_map;
	void CreateTree(string str);
	priority_queue<TreeNode*, vector<TreeNode*>, my_less> queue_node;
	void CreateCode(TreeNode *node, vector<bool> &code);
	BitsWriter bits_writer;
	void EncodeNode(TreeNode* node, vector<bool>& tree);
	TreeNode *root = nullptr;
};


Haffman::~Haffman()
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
void Haffman::CreateCode(TreeNode *node, vector<bool> &code)
{

	if (node->left != nullptr)
	{
		code.push_back(false);
		CreateCode(node->left, code);
	}

	if (node->right != nullptr) {
		code.push_back(true);
		CreateCode(node->right, code);
	}

	if (node != nullptr && node->left == nullptr && node->right == nullptr) {
		node->code = code;
		encode_map[node->symbol] = code;
	}
	if (code.size()) {
		code.pop_back();
	}
}


void Haffman::CreateTree(string str)
{
	unordered_map<unsigned, int> map;
	for (auto c : str)
	{
		map[c]++;
	}

	for (auto& it : map)
	{
		TreeNode *node = new TreeNode(it.first, it.second);
		queue_node.push(node);
	}

	int i = 0;
	while (queue_node.size() != 1)
	{
		TreeNode *node1 = queue_node.top();
		queue_node.pop();
		TreeNode *node2 = queue_node.top();
		queue_node.pop();
		TreeNode *res_node = new TreeNode(node1->priority + node2->priority);
		res_node->left = node1;
		res_node->right = node2;
		queue_node.push(res_node);
		i++;
	}

	root = queue_node.top();
	queue_node.pop();
	if (i > 0)
	{
		vector<bool> code = {};
		CreateCode(root, code);
	}
	else
	{
		vector<bool> code = { false };
		root->code = code;
		encode_map[root->symbol] = code;
	}

}

void Haffman::HaffmanEncoder(string &original, IOutputStream& output)
{
	CreateTree(original);

	vector<bool> encoded_tree;
	//static_cast<unsigned char>(size_tree);
	EncodeNode(root, encoded_tree);
	int size_tree = encoded_tree.size();
	for (auto c : encoded_tree)
		bits_writer.WriteBit(c);

	for (auto c : original)
	{
		auto code = encode_map[c];
		for (auto c : code)
			bits_writer.WriteBit(c);
	}
	vector<unsigned char> code = move(bits_writer.GetResult());
	for (auto value : code)
		output.Write(value);
}


void Haffman::EncodeNode(TreeNode* node, vector<bool>& tree) 
{
	if (!node->left) {
		tree.push_back(1);
		unsigned char symbol = node->symbol;
		for (size_t i = 0; i < 8; i++) {
			tree.push_back(symbol & 1);
			symbol >>= 1;
		}
	}
	else
	{
		tree.push_back(0);
		EncodeNode(node->left, tree);
		EncodeNode(node->right, tree);
	}
}

TreeNode* Haffman::DecodeNode(vector<bool>& tree, size_t& index) {
	if (tree[index++] == 1) {
		unsigned char symbol = 0;
		for (size_t i = 0; i < 8; i++) {
			symbol |= tree[index] << i;
			index++;
		}
		return new TreeNode(symbol, 0);
	}
	else {
		TreeNode* left = DecodeNode(tree, index);
		TreeNode* right = DecodeNode(tree, index);
		TreeNode * node = new TreeNode(left->priority + right->priority);
		node->left = left;
		node->right = right;
		return node;
	}
}


void Encode(IInputStream& original, IOutputStream& compressed)
{
	Haffman h;
	bytee value;
	string data;
	while (original.Read(value))
	{
		data += value;
	}

	h.HaffmanEncoder(data, compressed);
}




void Decode(IInputStream& compressed, IOutputStream& original)
{
	Haffman h;
	vector<bytee> encoded;
	vector<bool> encoded_bit;
	unsigned char value;
	unsigned char copy_value;
	while (compressed.Read(value)) {
		copy_value = value;
		for (size_t i = 0; i < 8; i++) {
			encoded_bit.push_back(value & 1);
			value >>= 1;
		}
	}

	size_t index = 0;
	TreeNode* root = h.DecodeNode(encoded_bit, index);
	TreeNode *node = root;
	auto count_valid_bit = copy_value;
	
	while (index < encoded_bit.size()- 16 + count_valid_bit)
	{
			bool step = encoded_bit[index++];
			if (step)
				node = node->right;
			else
				node = node->left;

			if (node->left == nullptr && node->right == nullptr)
			{
				original.Write(node->symbol);
				node = root;
			}
	}

}
