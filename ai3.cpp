#include <iostream>
#include <stack>
#include <queue>
#include <unordered_map>
#include <cctype>
#include <string>

using namespace std;

// Node structure for the binary expression tree
struct Node {
    char value;
    Node *left, *right;
    Node(char val) : value(val), left(nullptr), right(nullptr) {}
};

// Function to check if a character is an operator
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Function to get precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Convert infix expression to postfix expression
string infixToPostfix(const string &infix) {
    stack<char> s; // Stack to hold operators
    string postfix; // Resultant postfix expression
    for (char c : infix) {
        // If the character is an operand, add it to postfix
        if (isalpha(c)) {
            postfix += c;
        } 
        // If the character is '(', push it to the stack
        else if (c == '(') {
            s.push(c);
        } 
        // If the character is ')', pop and add to postfix until '(' is found
        else if (c == ')') {
            while (!s.empty() && s.top() != '(') {
                postfix += s.top();
                s.pop();
            }
            s.pop(); // Pop the '('
        } 
        // If the character is an operator
        else if (isOperator(c)) {
            // Pop from stack to postfix if the precedence of stack top is >= current operator
            while (!s.empty() && precedence(s.top()) >= precedence(c)) {
                postfix += s.top();
                s.pop();
            }
            s.push(c); // Push current operator to stack
        }
    }
    // Pop all remaining operators from the stack
    while (!s.empty()) {
        postfix += s.top();
        s.pop();
    }
    return postfix;
}

// Build expression tree from postfix expression
Node* buildExpressionTree(const string &postfix) {
    stack<Node*> s; // Stack to hold nodes
    for (char c : postfix) {
        // If the character is an operand, create a new node and push it to stack
        if (isalpha(c)) {
            s.push(new Node(c));
        } 
        // If the character is an operator
        else if (isOperator(c)) {
            Node *node = new Node(c);
            // Pop two nodes for the operator
            node->right = s.top(); s.pop();
            node->left = s.top(); s.pop();
            // Push the new subtree to stack
            s.push(node);
        }
    }
    return s.top(); // Root of the expression tree
}

// Level-order traversal of the binary expression tree
void levelOrder(Node* root) {
    if (!root) return;
    queue<Node*> q; // Queue to hold nodes for level-order traversal
    q.push(root);
    while (!q.empty()) {
        int size = q.size(); // Number of nodes at current level
        while (size--) {
            Node* node = q.front(); q.pop();
            cout << node->value << " ";
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        cout << endl; // Print new line for each level
    }
}

// Preorder traversal (Prefix expression)
void preorder(Node* root) {
    if (!root) return;
    cout << root->value; // Print root value
    preorder(root->left); // Recur on left subtree
    preorder(root->right); // Recur on right subtree
}

// Postorder traversal (Postfix expression)
void postorder(Node* root) {
    if (!root) return;
    postorder(root->left); // Recur on left subtree
    postorder(root->right); // Recur on right subtree
    cout << root->value; // Print root value
}

// Evaluate the expression tree
int evaluate(Node* root, unordered_map<char, int> &values) {
    if (!root) return 0;
    // If the node is a leaf node (operand), return its value
    if (!root->left && !root->right) {
        return values[root->value];
    }
    // Evaluate left and right subtrees
    int left_val = evaluate(root->left, values);
    int right_val = evaluate(root->right, values);
    // Apply the operator at root to the values of left and right subtrees
    switch (root->value) {
        case '+': return left_val + right_val;
        case '-': return left_val - right_val;
        case '*': return left_val * right_val;
        case '/': return left_val / right_val;
    }
    return 0; // Default return value (should not reach here)
}

int main() {
    string infix;
    cout << "Please enter an infix expression and press enter:" << endl;
    while (cin >> infix) {
        // Convert infix to postfix
        string postfix = infixToPostfix(infix);
        // Build the expression tree from postfix
        Node* root = buildExpressionTree(postfix);

        // Output the level-order traversal of the expression tree
        cout << "The level-order of the expression tree:" << endl;
        levelOrder(root);

        // Output the postfix expression
        cout << "The postfix expression: ";
        postorder(root);
        cout << endl;

        // Output the prefix expression
        cout << "The prefix expression: ";
        preorder(root);
        cout << endl;

        // Get values for each variable from the user
        unordered_map<char, int> values;
        for (char c : infix) {
            if (isalpha(c) && values.find(c) == values.end()) {
                cout << "Please enter the value of " << c << ": ";
                cin >> values[c];
            }
        }

        // Evaluate and output the result of the expression
        cout << "= " << evaluate(root, values) << endl;
        cout << "\nPlease enter an infix expression and press enter:" << endl;
    }
    return 0;
}
