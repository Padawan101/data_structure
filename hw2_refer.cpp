#include <iostream>
#include <stack>

struct Node {
    char data;
    Node* next;
    Node* prev;
    Node(char d) : data(d), next(nullptr), prev(nullptr) {}
};

class TextEditor {
    Node* head;
    Node* tail;
    Node* cursor;

    struct Action {
        enum Type { INSERT, DELETE, BACKSPACE, MOVE_LEFT, MOVE_RIGHT } type;
        char data;
        Action(Type t, char d = '\0') : type(t), data(d) {}
    };

    std::stack<Action> undoStack, redoStack;

public:
    TextEditor(const std::string& initialText) {
        head = tail = cursor = nullptr;
        for (char c : initialText) {
            insertCharAtEnd(c);
        }
        cursor = tail; // Set cursor at the end
    }

    ~TextEditor() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void insertChar(char c) {
        saveState(Action::INSERT, c);
        Node* newNode = new Node(c);
        if (!head) {
            head = tail = newNode;
        } else if (cursor == tail) {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        } else {
            Node* nextNode = cursor->next;
            cursor->next = newNode;
            newNode->prev = cursor;
            newNode->next = nextNode;
            if (nextNode) nextNode->prev = newNode;
        }
        cursor = newNode;
    }

    void deleteChar() {
        if (!cursor || !cursor->next) return;
        saveState(Action::DELETE, cursor->next->data);
        Node* temp = cursor->next;
        if (temp == tail) {
            tail = cursor;
            cursor->next = nullptr;
        } else {
            cursor->next = temp->next;
            temp->next->prev = cursor;
        }
        delete temp;
    }

    void backspaceChar() {
        if (!cursor) return;
        saveState(Action::BACKSPACE, cursor->data);
        Node* temp = cursor;
        if (cursor == head) {
            head = head->next;
            if (head) head->prev = nullptr;
        } else {
            cursor->prev->next = cursor->next;
            if (cursor->next) cursor->next->prev = cursor->prev;
        }
        cursor = cursor->prev ? cursor->prev : head;
        delete temp;
    }

    void moveLeft() {
        if (cursor && cursor->prev) {
            saveState(Action::MOVE_LEFT);
            cursor = cursor->prev;
        }
    }

    void moveRight() {
        if (cursor && cursor->next) {
            saveState(Action::MOVE_RIGHT);
            cursor = cursor->next;
        }
    }

    void undo() {
        if (undoStack.empty()) return;
        Action action = undoStack.top();
        undoStack.pop();

        switch (action.type) {
            case Action::INSERT:
                deleteChar();
                break;
            case Action::DELETE:
                insertChar(action.data);
                cursor = cursor->prev;
                break;
            case Action::BACKSPACE:
                insertChar(action.data);
                cursor = cursor->prev;
                break;
            case Action::MOVE_LEFT:
                moveRight();
                break;
            case Action::MOVE_RIGHT:
                moveLeft();
                break;
        }
        redoStack.push(action);
    }

    void redo() {
        if (redoStack.empty()) return;
        Action action = redoStack.top();
        redoStack.pop();

        switch (action.type) {
            case Action::INSERT:
                insertChar(action.data);
                break;
            case Action::DELETE:
                deleteChar();
                break;
            case Action::BACKSPACE:
                backspaceChar();
                break;
            case Action::MOVE_LEFT:
                moveLeft();
                break;
            case Action::MOVE_RIGHT:
                moveRight();
                break;
        }
        undoStack.push(action);
    }

    void display() {
        Node* temp = head;
        while (temp) {
            if (temp == cursor) std::cout << "|";
            std::cout << temp->data;
            temp = temp->next;
        }
        if (cursor == nullptr) std::cout << "|";
        std::cout << std::endl;
    }

private:
    void insertCharAtEnd(char c) {
        Node* newNode = new Node(c);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void saveState(Action::Type type, char data = '\0') {
        undoStack.push(Action(type, data));
        while (!redoStack.empty()) redoStack.pop(); // Clear redo stack after new action
    }
};

int main() {
    std::string input;
    std::cout << "Please enter an initial string consisting a/A-z/Z and space: ";
    std::getline(std::cin, input);

    TextEditor editor(input);
    editor.display();

    char command;
    while (true) {
        std::cout << "Enter command (a-Z for insert, 0 for delete, b for backspace, l for move left, r for move right, u for undo, e for redo, q to quit): ";
        std::cin >> command;

        if (command == 'q') break;
        else if (command >= 'a' && command <= 'z' || command >= 'A' && command <= 'Z' || command == ' ') {
            editor.insertChar(command);
        } else if (command == '0') {
            editor.deleteChar();
        } else if (command == 'b') {
            editor.backspaceChar();
        } else if (command == 'l') {
            editor.moveLeft();
        } else if (command == 'r') {
            editor.moveRight();
        } else if (command == 'u') {
            editor.undo();
        } else if (command == 'e') {
            editor.redo();
        }

        editor.display();
    }

    return 0;
}
