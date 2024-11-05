#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

//data structures
		//store the member of string of tex editor
struct _node{
		_node *lLink;
		char c;
		_node *rLink;
};
typedef struct _node *node;
		//stack for redo and undo
struct _stackNode{
		char command;
		_stackNode *next;
};
typedef struct _stackNode *stackPtr;

//functions
		//for DDL
void insertNode( char c, node &prev );
void printList( node head );
		//for stack
void push( stackPtr &head, char c );
char pop( stackPtr &ptr );


int main(){
		//init the content of text editor( DDL )
				//input string
		string input;
		printf( "Please enter an initial string consisting a/A-z/Z and space: " );
		getline( cin, input );
				//generate the list
		input += '|';
		node head = nullptr;
		insertNode( input[0], head );
		node tmp = head;
		int len = input.length();
		for( int i = 1; i < len; i++ ){
				insertNode( input[i], tmp );
				tmp = tmp->rLink;
		}

		//init the redo & undo stacks
		stackPtr redoStack = nullptr, undoStack = nullptr;


		return 0;

		//main feature
		char action = '?';
		while( action != 27 ){
				cout << "Enter command (a-Z for insert, 0 for delete, b for backspace, l for move left, r for move right, u for undo, e for redo, esc to quit): ";
				scanf( "%c", &action );
		}


		//return 0;
}

void insertNode( char c, node &prev ){
		node newNode = (node) malloc( sizeof( _node ) );
		newNode->c = c;
		newNode->rLink = nullptr, newNode->lLink = nullptr;

		if( prev == nullptr ){
				prev = newNode;
		}
		else{
				prev->rLink = newNode;
				newNode->lLink = prev;
		}

		return;
}
void printList( node head ){
		node ptr = head;

		while( ptr != NULL ){
				printf( "%c", ptr->c );
				ptr = ptr-> rLink;
		}

		printf("\n");
}

void push( stackPtr &head, char c ){
		stackPtr newE = (stackPtr) malloc( sizeof(_stackNode) );
		newE->command = c;
		newE->next = nullptr;

		if( head == nullptr ){
				head = newE;
		}
		else{
				head->next = newE;
		}

		return;
}
char pop( stackPtr &ptr ){
		char element = '-';

		if( ptr != nullptr ){
				element = ptr->command;
				free(ptr);
		}

		return element;
}
