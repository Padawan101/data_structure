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
nodePtr redoTop = &redoBeg, undoTop = &undoBeg;

//functions
		//for DDL
void insertNode( bool dir, char element, bool _push );	//dir: 1 for right; init: 1 for if first input of string
void deleteNode( bool dir, bool _push );
void swapNode( bool dir, bool _push );
void undo();
void redo();
void printList();
		//for stacks
void push( bool type, char command, char element );	//type: 1 for undo, 0 for redo
_node pop( bool type );

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
				insertNode( 0, input[i], 0 );
		}
		printList();

		//main feature
		char command = '-';
		while( command != 27 ){
				cout << "Enter command (a-Z, A-Z, and space for insertion, 0 for delete, 3 for backspace delete, 1 for move left, 2 for move right, 4 for undo, 5 for redo, esc to quit): ";
				command = cin.get();
				cin.get();
				if( ( ( 'A' <= command ) && ( command <= 'Z' ) ) || ( ( 'a' <= command ) && ( command <= 'z' ) ) || ( command == ' ' ) ){
						insertNode( 0, command, 1 );
				}
				else if( command == '0' ) deleteNode( 1, 1 );
				else if( command == '3' ) deleteNode( 0, 1 );
				else if( command == '1' ) swapNode( 0, 1 );
				else if( command == '2' ) swapNode( 1, 1 );
				else if( command == '4' ) undo();
				else if( command == '5' ) redo();
				printList();
		}

		return 0;
}

void insertNode( bool dir, char element, bool _push ){
		nodePtr newNode = (nodePtr) malloc( sizeof( _node ) );
		newNode->element = element;

		if( _push ) push( 1, 'i', element );
		
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
void deleteNode( bool dir, bool _push ){
		if( dir ){
				if( cursorPtr->rLink != nullptr ){
						nodePtr deleteN = cursorPtr->rLink;
						if( _push ) push( 1, '0', deleteN->element );
						if( deleteN->rLink == nullptr ) cursorPtr->rLink = nullptr;
						else cursorPtr->rLink = deleteN->rLink, deleteN->rLink->lLink = cursorPtr, free(deleteN);
				}
		}
		else{
				if( cursorPtr->lLink != head ){
						nodePtr deleteN = cursorPtr->lLink;
						if( _push ) push( 1, '3', deleteN->element );
						if( deleteN->lLink == head ) cursorPtr->lLink = head, head->rLink = cursorPtr;
						else cursorPtr->lLink = deleteN->lLink, deleteN->lLink->rLink = cursorPtr, free(deleteN);
				}
		}

		return;
}
void swapNode( bool dir, bool _push ){
		if( dir ){
				if( cursorPtr->rLink != nullptr ){
						if( _push ) push( 1, '2', ' ' );
						
						cursorPtr->element = cursorPtr->rLink->element;
						cursorPtr->rLink->element = '|';
						cursorPtr = cursorPtr->rLink;
				}
		}
		else{
				if( cursorPtr->lLink != head ){
						if( _push ) push( 1, '1', ' ' );
						
						cursorPtr->element = cursorPtr->lLink->element;
						cursorPtr->lLink->element = '|';
						cursorPtr = cursorPtr->lLink;
				}
		}

		return;
}
void undo(){
		if( undoTop->command != '!' ){
				_node p = pop(1);

				if( p.command == 'i' ) deleteNode( 0, 0 );
				else if( p.command == '0' ) insertNode( 1, p.element, 0 );
				else if( p.command == '3' ) insertNode( 0, p.element, 0 );
				else if( p.command == '1' ) swapNode( 1, 0 );
				else if( p.command == '2' ) swapNode( 0, 0 );

				push( 0, p.command, p.element );
		}

		return;
}
void redo(){
		if( redoTop->command != '!' ){
				_node p = pop(0);

				if( p.command == 'i' ) insertNode( 0, p.element, 0 );
				else if( p.command == '0' ) deleteNode( 1, 0 );
				else if( p.command == '3' ) deleteNode( 0, 0 );
				else if( p.command == '1' ) swapNode( 0, 0 );
				else if( p.command == '2' ) swapNode( 1, 0 );
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
		nodePtr newStack = (nodePtr) malloc( sizeof(_node) );
		newStack->command = command, newStack->element = element, newStack->rLink = nullptr;

		if( type ){
				undoTop->rLink = newStack, newStack->lLink = undoTop;
				undoTop = newStack;
		}
		else{
				redoTop->rLink = newStack, newStack->lLink = redoTop;
				redoTop = newStack;
		}

		return;
}
_node pop( bool type ){
		_node p;
		p.command = '!';

		if( type ){
				if( undoTop->element != '!' ){
						p.command = undoTop->command, p.element = undoTop->element;
						nodePtr tmp = undoTop;
						undoTop = undoTop->lLink, undoTop->rLink = nullptr;
						free(tmp);
				}
		}
		else{
				if( redoTop->element != '!' ){
						p.command = redoTop->command, p.element = redoTop->element;
						nodePtr tmp = redoTop;
						redoTop = redoTop->lLink, redoTop->rLink = nullptr;
						free(tmp);
				}
		}

		return p;
}
