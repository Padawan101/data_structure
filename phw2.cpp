#include <iostream>

using namespace std;

//data structures
struct _node{
		_node *lLink;
		char command;
		char element;
		_node *rLink;
};
typedef struct _node *nodePtr;

//global variables
		//DDL
_node beg, cursor;
		//DDL's ptrs
nodePtr head = &beg, cursorPtr = &cursor;
		//stacks
_node redoBeg, undoBeg;
		//stacks' ptrs
stackPtr redoTop = &redoBeg, undoTop = &undoBeg;

//functions
		//for DDL
void insertNode( bool dir, char element, bool init );	//dir: 1 for right; init: 1 for if first input of string
void printList();
		//for stacks
void push( bool type, char command, char element );	//type: 1 for undo, 0 for redo
_stackNode pop();

int main(){
		//init
				//the init input of text editor
		string input;
		printf( "Please enter an initial string consisting a/A-z/Z and space: " );
		getline( cin, input );
		beg.lLink = nullptr, beg.rLink = &cursor, beg.element = '!';
		cursor.lLink = &beg, cursor.rLink = nullptr, cursor.element = '|';
				//init stack
		undoBeg.lLink = nullptr, undoBeg.rLink = nullptr, undoBeg.element = '!';
		redoBeg.lLink = nullptr, redoBeg.rLink = nullptr, redoBeg.element = '!';
		
		int len = input.length();
		//printList();	//delete this
		for( int i = 0; i < len; i++ ){
				insertNode( 0, input[i], 1 );
		}
		printList();

		//main feature
		char command = '-';
		while( command != 27 ){
				scanf(" %c", &command);
				if( ( ( 'A' <= command ) && ( command <= 'Z' ) ) || ( ( 'a' <= command ) && ( command <= 'z' ) ) || ( command == ' ' ) ){
						insertNode( 0, command, 0 );
				}
				printList();
		}

		return 0;
}

void insertNode( bool dir, char element, bool init ){
		nodePtr newNode = (nodePtr) malloc( sizeof( _node ) );
		newNode->element = element;
		
		if(dir){	//add from the right of cursor
				if( cursorPtr->rLink != nullptr ){
						newNode->rLink = cursorPtr->rLink, newNode->lLink = cursorPtr;
						cursorPtr->rLink->lLink = newNode, cursorPtr->rLink = newNode;
				}
				else{
						newNode->rLink = nullptr, newNode->lLink = cursorPtr;
						cursorPtr->rLink = newNode;
				}
		}
		else{	//add from the left of cursor
				newNode->lLink = cursorPtr->lLink, newNode->rLink = cursorPtr;
				cursorPtr->lLink->rLink = newNode, cursorPtr->lLink = newNode;
		}

		return;
}
void printList(){
		nodePtr ptr = head->rLink;
		while( ptr != nullptr ){
				printf( "%c", ptr->element );
				ptr = ptr->rLink;
		}
		printf("\n");

		return;
}

void push( bool type, char command, char element ){
		stackPtr 
