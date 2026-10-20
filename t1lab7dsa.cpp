#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
using namespace std;
struct Node {
    char data;
    Node *left, *right;
    Node(char c) {
        data = c;
        left = right = nullptr;
    }
};
struct BSTNode {
    int data;
    BSTNode *left, *right;
    BSTNode(int v) {
        data = v;
        left = right = nullptr;
    }
};
Node* parse(string s, int& pos) {
    if (pos >= s.size() || s[pos] == ',' || s[pos] == ')')
        return nullptr;
    Node* node = new Node(s[pos]);
    pos++;
    if (pos < s.size() && s[pos] == '(') {
        pos++;
        node->left = parse(s, pos);
        pos++;
        node->right = parse(s, pos);
        pos++;
    }
    return node;
}
Node* buildTree(string s) {
    int pos = 0;
    return parse(s, pos);
}
void preorderString(Node* root) {
    if (root == nullptr) return;
    cout << root->data;
    if (root->left || root->right) {
        cout << "(";
        preorderString(root->left);
        cout << ",";
        preorderString(root->right);
        cout << ")";
    }
}
void preorder(Node* root) {
    if (root == nullptr) return;
    cout << root->data << " ";
    preorder(root->left);
    preorder(root->right);
}
void inorderIter(Node* root) {
    stack<Node*> st;
    Node* cur = root;
    while (cur != nullptr || !st.empty()) {
        while (cur != nullptr) {
            st.push(cur);
            cur = cur->left;
        }
        cur = st.top();
        st.pop();
        cout << cur->data << " ";
        cur = cur->right;
    }
}
// uses two stacks, second one reverses the order
void postorderIter(Node* root) {
    if (root == nullptr) return;
    stack<Node*> s1, s2;
    s1.push(root);
    while (!s1.empty()) {
        Node* n = s1.top();
        s1.pop();
        s2.push(n);
        if (n->left) s1.push(n->left);
        if (n->right) s1.push(n->right);
    }
    while (!s2.empty()) {
        cout << s2.top()->data << " ";
        s2.pop();
    }
}
bool checkNode(string s, int& pos) {
    if (pos >= s.size()) return true;
    if (s[pos] == ',' || s[pos] == ')') return true;
    if (!isalnum(s[pos])) return false;
    pos++;
    if (pos < s.size() && s[pos] == '(') {
        pos++;
        if (!checkNode(s, pos)) return false;
        if (pos >= s.size() || s[pos] != ',') return false;
        pos++;
        if (!checkNode(s, pos)) return false;
        if (pos >= s.size() || s[pos] != ')') return false;
        pos++;
    }
    return true;
}
bool isValid(string s) {
    if (s.empty() || !isalnum(s[0])) return false;
    // check brackets are balanced
    stack<char> st;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') {
            st.push('(');
        }
        else if (s[i] == ')') {
            if (st.empty()) return false;
            st.pop();
        }
    }
    if (!st.empty()) return false;
    int pos = 0;
    return checkNode(s, pos) && pos == s.size();
}
// height in edges, so a single node is 0
int height(Node* root) {
    if (root == nullptr) return -1;
    return 1 + max(height(root->left), height(root->right));
}
void printDepths(Node* root, int depth = 0) {
    if (root == nullptr) return;
    cout << "  Node " << root->data << " -> depth " << depth << endl;
    printDepths(root->left, depth + 1);
    printDepths(root->right, depth + 1);
}
BSTNode* bstInsert(BSTNode* root, int v) {
    if (root == nullptr)
        return new BSTNode(v);
    if (v < root->data)
        root->left = bstInsert(root->left, v);
    else if (v > root->data)
        root->right = bstInsert(root->right, v);

    return root;
}
BSTNode* buildBST(vector<int> vals) {
    BSTNode* root = nullptr;
    for (int i = 0; i < vals.size(); i++)
        root = bstInsert(root, vals[i]);
    return root;
}
void bstInorder(BSTNode* root) {
    if (root == nullptr) return;
    bstInorder(root->left);
    cout << root->data << " ";
    bstInorder(root->right);
}
void freeTree(Node* root) {
    if (root == nullptr) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}
void freeBST(BSTNode* root) {
    if (root == nullptr) return;
    freeBST(root->left);
    freeBST(root->right);
    delete root;
}
int main() {
    string s = "a(b(d,e),c(,f(g,)))";
    cout << "Given string: " << s << "\n\n";
    cout << "[Validation]\n";
    vector<string> tests = {s, "a(b(d,e),c(,f(g,))", "a(b,c))", "a(b)", "(b,c)", "a(b,c)"};
    for (int i = 0; i < tests.size(); i++) {
        cout << "  " << tests[i] << "  ->  ";
        if (isValid(tests[i])) cout << "VALID\n";
        else cout << "INVALID\n";
    }
    if (!isValid(s)) {
        cout << "Main string invalid, stopping.\n";
        return 1;
    }
    Node* root = buildTree(s);
    cout << "\n[Preorder - string form]\n  ";
    preorderString(root);
    cout << "\n[Preorder - sequence]\n  ";
    preorder(root);
    cout << "\n\n[Inorder - iterative]\n  ";
    inorderIter(root);
    cout << "\n\n[Postorder - iterative]\n  ";
    postorderIter(root);
    cout << "\n\n[Height]\n  Height of tree (in edges) = " << height(root) << endl;
    cout << "\n[Depth of each node]\n";
    printDepths(root);
    vector<int> nums = {50, 30, 70, 20, 40, 60, 80};
    BSTNode* bst = buildBST(nums);
    cout << "\n[BST from {50,30,70,20,40,60,80}]\n  Inorder: ";
    bstInorder(bst);
    cout << endl;
    freeTree(root);
    freeBST(bst);
    return 0;
}