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

//global variables
		//init the redo & undo stacks
stackPtr redoStack = nullptr, undoStack = nullptr;

//functions
		//for DDL
void insertNode( char c, node &prev, bool init );	//done
void swapNode( node &head, bool dir );	//done
void deleteNode( node &ptr, bool dir );
void printList( node head );	//done
		//for stack
void push( stackPtr &head, char c );	//done
char pop( stackPtr &ptr );	//done


int main(){
		//init the content of text editor( DDL )
				//input string
		string input;
		printf( "Please enter an initial string consisting a/A-z/Z and space: " );
		getline( cin, input );
				//generate the list
		input += '|';
		node head = nullptr;
		insertNode( input[0], head, 1 );
		node ptr = head;
		int len = input.length();
		for( int i = 1; i < len; i++ ){
				insertNode( input[i], ptr, 1 );
				ptr = ptr->rLink;
		}

		//main feature
		char command = '?';
		while( command != 27 ){
				cout << "Enter command (a-Z for insert, 0 for delete, b for backspace, 1 for move left, 2 for move right, u for undo, r for redo, esc to quit): ";
				scanf( "%c", &command );

				if( command == '0' )
				else if( command == 'b' )
				else if( command == '1' ) swapNode( ptr, 0 );
				else if( command == '2' ) swapNode( ptr, 1 );
				else if( command == 'r' )
				else if( command == 'u' )
				else if( command == 27 ) ;
				else if( ( ( 'A' <= command ) && ( command <= 'Z' ) ) || ( ( 'a' <= command ) && ( command <= 'z' ) ) || ( command == ' ' ) ){
						insertNode( command, ptr, 0 );
		}
				else printf( "Invalid Input.\n" );


		return 0;
}

void insertNode( char c, node &prev, bool init ){
		node newNode = (node) malloc( sizeof( _node ) );
		newNode->c = c;
		newNode->rLink = nullptr, newNode->lLink = nullptr;

		if( !init ) push( undoStack, c );

		if( prev == nullptr ){
				prev = newNode;
		}
		else{
				prev->rLink = newNode;
				newNode->lLink = prev;
		}

		return;
}
void swapNode( node &head, bool dir ){
		node ptr = head;

		if( head == nullptr ) return;
		char tmp = head->c;

		if( dir ){
				if( ptr->rLink == nullptr ) return;

				push( undoStack, 2 );

				head->c = ptr->rLink->c;
				ptr->rLink->c = tmp;
		}
		else{
				if( ptr->lLink == nullptr ) return;

				push( undoStack, 1 );

				head->c = ptr->lLink->c;
				ptr->lLink->c = tmp;
		}

		return;
}
void deleteNode( node &ptr, bool dir ){
		if( ptr == nullptr ) return;

		if( dir ){

}
void printList( node head ){
		int cnt = 0;
		node ptr = head;

		while( ptr != nullptr ){
				if( cnt++ > 10 ) break;
				printf( "%c", ptr->c );
				printf( ", %p, %p\n", ptr, ptr->rLink );
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
