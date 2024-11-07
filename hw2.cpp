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
		char element;
		_stackNode *next;
};
typedef struct _stackNode *stackPtr;


//global variables
node cursor = nullptr;
		//init the redo & undo stacks
stackPtr redoStack = nullptr, undoStack = nullptr;


//functions
		//for DDL
void insertNode( char c, bool init );
void swapNode( bool dir );	//[dir] 1: right, 0: left
void deleteNode( bool dir );	//[dir] 1: right, 0: left
void printList( node head );
void undoNode();
void redoNode();
		//for stack
void push( stackPtr &head, char command, char element );
_stackNode pop( stackPtr &ptr );


int main(){
		//init the content of text editor( DDL )
				//input string
		string input;
		printf( "Please enter an initial string consisting a/A-z/Z and space: " );
		getline( cin, input );
				//generate the list
		_node beg;
		beg.lLink = nullptr, beg.rLink = nullptr, beg.c = '|';
		cursor = &beg;
		node head;
		int len = input.length();
		for( int i = 0; i < len; i++ ){
				insertNode( input[i], 1 );
				if( i == 0 ) head = cursor->lLink;
		}

		printList(head);

		//main feature
		char command = '?';
		while( command != 27 ){
				cout << "Enter command (a-Z for insert, 0 for delete, b for backspace, 1 for move left, 2 for move right, u for undo, r for redo, esc to quit): ";
				scanf( "%c", &command );

				if( command == '0' ) deleteNode( 1 );
				else if( command == 'b' ) deleteNode( 0 );
				else if( command == '1' ) swapNode( 0 );
				else if( command == '2' ) swapNode( 1 );
				else if( command == 'r' );
				else if( command == 'u' );
				else if( command == 27 ) ;
				else if( ( ( 'A' <= command ) && ( command <= 'Z' ) ) || ( ( 'a' <= command ) && ( command <= 'z' ) ) || ( command == ' ' ) ){
						insertNode( command, cursor->lLink, 0 );
		}
				else printf( "Invalid Input.\n" );
		}

		return 0;
}

void insertNode( char c, bool init ){
		node newNode = (node) malloc( sizeof( _node ) );
		newNode->c = c;
		newNode->rLink = nullptr, newNode->lLink = nullptr;

		if( !init ) push( undoStack, 'i', c );

		if( cursor->lLink == nullptr ){
				newNode->rLink = cursor;
				cursor->lLink = newNode;
		}
		else{
				cursor->lLink->rLink = newNode;
				newNode->lLink = cursor->lLink;

				newNode->rLink = cursor;
				cursor->lLink = newNode;
		}

		return;
}
void swapNode( bool dir ){
		node ptr = cursor;

		if( cursor == nullptr ) return;
		char tmp = cursor->c;

		if( dir ){
				if( ptr->rLink == nullptr ) return;

				push( undoStack, '2', '-' );

				cursor->c = ptr->rLink->c;
				ptr->rLink->c = tmp;

				cursor = cursor->rLink;
		}
		else{
				if( ptr->lLink == nullptr ) return;

				push( undoStack, '1', '-' );

				cursor->c = ptr->lLink->c;
				ptr->lLink->c = tmp;

				cursor = cursor->lLink;
		}

		return;
}
void deleteNode( bool dir ){
		if( cursor == nullptr ) return;

		if( dir ){
				if( cursor->rLink == nullptr ) return;

				push( undoStack, '0', cursor->rLink->c );

				node deleted = cursor->rLink;
				if( deleted->rLink == nullptr ){
						cursor->rLink = nullptr;
				}
				else{
						deleted->rLink->lLink = deleted->lLink;
						deleted->lLink->rLink = deleted->rLink;
				}
				free(deleted);
		}
		else{
				if( cursor->lLink == nullptr ) return;

				push( undoStack, '1', cursor->lLink->c );

				node deleted = cursor->lLink;
				if( deleted->lLink == nullptr ){
						cursor->lLink = nullptr;
				}
				else{
						deleted->lLink->rLink = deleted->rLink;
						deleted->rLink->lLink = deleted->lLink;
				}
				free(deleted);
		}

		return;
}
void printList( node head ){
		int cnt = 0;
		node ptr = head;

		while( ptr != nullptr ){
				if( cnt++ > 10 ) break;	//delete this
				printf( "%c", ptr->c );
				ptr = ptr-> rLink;
		}

		printf("\n");
}
void undoNode(){
		_stackNode tmp = pop();

		if( tmp.command == '-' ) return;
		else{
				if( top.command == 'i' ) deleteNode(0);
				else if( top.command == '0' ) insertNode(  );

				push( redo, tmp.command, tmp.element );
		}
}


void push( stackPtr &head, char command, char element ){
		stackPtr newE = (stackPtr) malloc( sizeof(_stackNode) );
		newE->command = command;
		newE->element = element;
		newE->next = nullptr;

		if( head == nullptr ){
				head = newE;
		}
		else{
				head->next = newE;
		}

		return;
}
_stackNode pop( stackPtr &ptr ){
		_stackNode tmp;
		tmp.command = '-';

		if( ptr != nullptr ){
				tmp = *ptr;
				free(ptr);
		}

		return tmp;
}
