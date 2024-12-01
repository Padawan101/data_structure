#include <iostream>
#include <unordered_map>
#include <cctype>
#include <string>

using namespace std;

// Custom Stack implementation
template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };
    Node* topNode;

public:
    Stack() : topNode(nullptr) {}
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
    void push(T val) {
        Node* newNode = new Node(val);
        newNode->next = topNode;
        topNode = newNode;
    }
    void pop() {
        if (topNode) {
            Node* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
    }
    T top() const {
        if (topNode) {
            return topNode->data;
        }
        throw runtime_error("Stack is empty");
    }
    bool isEmpty() const {
        return topNode == nullptr;
    }
};

// Custom Queue implementation
template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };
    Node *frontNode, *rearNode;
    int count; // Track the number of elements in the queue

public:
    Queue() : frontNode(nullptr), rearNode(nullptr), count(0) {}
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
    void enqueue(T val) {
        Node* newNode = new Node(val);
        if (rearNode) {
            rearNode->next = newNode;
        } else {
            frontNode = newNode;
        }
        rearNode = newNode;
        count++; // Increment the count when a new element is added
    }
    void dequeue() {
        if (frontNode) {
            Node* temp = frontNode;
            frontNode = frontNode->next;
            if (!frontNode) {
                rearNode = nullptr;
            }
            delete temp;
            count--; // Decrement the count when an element is removed
        }
    }
    T front() const {
        if (frontNode) {
            return frontNode->data;
        }
        throw runtime_error("Queue is empty");
    }
    bool isEmpty() const {
        return frontNode == nullptr;
    }
    int size() const {
        return count; // Return the number of elements in the queue
    }
};

// Node structure for the binary expression tree
struct Node {
    char value;
    Node *left, *right;
    Node(char val) : value(val), left(nullptr), right(nullptr) {}
};

// Function to check if a character is an operator
bool isOperator(char c);
// Function to get precedence of operators
int precedence(char op);
// Convert infix expression to postfix expression
string infixToPostfix(const string &infix);
// Build expression tree from postfix expression
Node* buildExpressionTree(const string &postfix);
// Level-order traversal of the binary expression tree
void levelOrder(Node* root);
// Preorder traversal (Prefix expression)
void preorder(Node* root);
// Postorder traversal (Postfix expression)
void postorder(Node* root);
// Evaluate the expression tree
double evaluate(Node* root, unordered_map<char, double> &values);

int main() {
    string infix;
    cout << "Please enter an infix expression and press enter:" << endl;
    while (cin >> infix) {
    	if( infix[0] == 27 ) break;
    	
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
        unordered_map<char, double> values;
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
    Stack<char> s; // Custom Stack to hold operators
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
            while (!s.isEmpty() && s.top() != '(') {
                postfix += s.top();
                s.pop();
            }
            s.pop(); // Pop the '('
        } 
        // If the character is an operator
        else if (isOperator(c)) {
            // Pop from stack to postfix if the precedence of stack top is >= current operator
            while (!s.isEmpty() && precedence(s.top()) >= precedence(c)) {
                postfix += s.top();
                s.pop();
            }
            s.push(c); // Push current operator to stack
        }
    }
    // Pop all remaining operators from the stack
    while (!s.isEmpty()) {
        postfix += s.top();
        s.pop();
    }
    
    return postfix;
}

// Build expression tree from postfix expression
Node* buildExpressionTree(const string &postfix) {
    Stack<Node*> s; // Custom Stack to hold nodes
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
    Queue<Node*> q; // Custom Queue to hold nodes for level-order traversal
    q.enqueue(root);
    while (!q.isEmpty()) {
        int size = q.size(); // Number of nodes at current level
        while (size--) {
            Node* node = q.front(); q.dequeue();
            cout << node->value << " ";
            if (node->left) q.enqueue(node->left);
            if (node->right) q.enqueue(node->right);
        }
        cout << endl; // Print new line for each level
    }
    
    return;
}

// Preorder traversal (Prefix expression)
void preorder(Node* root) {
    if (!root) return;
    cout << root->value; // Print root value
    preorder(root->left); // Recur on left subtree
    preorder(root->right); // Recur on right subtree
    
    return;
}

// Postorder traversal (Postfix expression)
void postorder(Node* root) {
    if (!root) return;
    postorder(root->left); // Recur on left subtree
    postorder(root->right); // Recur on right subtree
    cout << root->value; // Print root value
    
    return;
}

// Evaluate the expression tree
double evaluate(Node* root, unordered_map<char, double> &values) {
    if (!root) return 0;
    // If the node is a leaf node (operand), return its value
    if (!root->left && !root->right) {
        return values[root->value];
    }
    // Evaluate left and right subtrees
    double left_val = evaluate(root->left, values);
    double right_val = evaluate(root->right, values);
    // Apply the operator at root to the values of left and right subtrees
    switch (root->value) {
        case '+': return left_val + right_val;
        case '-': return left_val - right_val;
        case '*': return left_val * right_val;
        case '/': return left_val / right_val;
    }
    
    return 0; // Default return value (should not reach here)
}
