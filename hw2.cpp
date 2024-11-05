#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

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
}
typedef struct _node *stackPtr;

//for DDL
void insertNode( char c, node next );

//for stacks


void printList( node head );

int main(){
		//init the DDL
		string input;
		printf( "Please enter an initial string consisting a/A-z/Z and space: " );
		getline( cin, input );
		input += '|';
		cout << input << endl;
		int len = input.length();
		node head = nullptr;
		for( int i = 0; i < len; i++ ){
				insertNode( input[i], head );
		}

		//
		char action = '?';
		while( action != 'q' ){
				cout << "Enter command (a-Z for insert, 0 for delete, b for backspace, l for move left, r for move right, u for undo, e for redo, q to quit): ";
				scanf( "%c", &action );
		}


		return 0;
}

void insertNode( char c, node next ){
		node newNode = (node) malloc( sizeof( _node ) );
		newNode->c = c;
		newNode->rLink = nullptr, newNode->lLink = nullptr;
		if( next == nullptr ){
				next = newNode;
		}
		else{
				newNode->rLink = next;
				newNode->lLink = next->lLink;

				next->lLink = newNode;
				next->lLink->rLink = newNode;
		}

		return;
}
void printList( node head ){
		node ptr = head;

		while( ptr != NULL ){
				printf( "%c", ptr->c );
				ptr = ptr-> rLink;
		}
}
