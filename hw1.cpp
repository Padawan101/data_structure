#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

//the datatype of sparse matrix
typedef struct triple{
		int row;
		int col;
		double value;
} Triple;
typedef vector<Triple> sparseMatrix;

//homework requirements
void print_entry( sparseMatrix &A, int row, int col );
sparseMatrix sparse_matrix_add( sparseMatrix &A, sparseMatrix &B );
sparseMatrix sparse_matrix_multiply( sparseMatrix &A, sparseMatrix &B );
sparseMatrix ReLU( sparseMatrix &A );
sparseMatrix Sigmoid( sparseMatrix &A );

//funamental tools for developers
void add_entry( sparseMatrix &A, int row, int col, double val );	//add a new entry to a sparse matrix
void print( sparseMatrix &A );				//print the whole sparse matrix
int bs_same( sparseMatrix &A, int row, int col );				//find whether an entry(row^th, col^th term) existed in an sparse matrix
void sort_rowMajority( sparseMatrix &A );	//to sort a sparse matrix by row-majority

int main(){
		//create & initialize the matrices
		sparseMatrix W, X, B, Z;
		add_entry( W, 0, 0, 0 );
		add_entry( X, 0, 0, 0 );
		add_entry( B, 0, 0, 0 );
		add_entry( Z, 0, 0, 0 );

		//user-dominated
		

		return 0;
}

void sort_rowMajority( sparseMatrix &A ){	//done
		if( A.size() > 2 ){
				int len = A.size();
				for( int i = 1; i < len; i++ ){
						for( int j = 1; j < ( len - 1 - i ); j++ ){
								Triple tmp = A.at(j);
								A.at(j) = A.at(j + 1);
								A.at(j + 1) = tmp;
						}
				}
		}

		return;
}
int bs_same( sparseMatrix &A, int row, int col ){	//done
		if( A.size() > 1 ){
				sort_rowMajority(A);

				int left = 1, right = A.at(0).row;
				while( left <= right ){
						int mid = ( left + right ) / 2;
						if( A.at(mid).row < row ) left = mid + 1;
						else if( A.at(mid).row > row ) right = mid - 1;
						else{
								int n = A.size();
								for( int i = 1; i < n; i++ ){
										if( A.at(i).col == col ) return i;
								}
								break;
						}
				}
		}
		return 0;
}
void add_entry( sparseMatrix &A, int row, int col, double val ){	//done
		int n = bs_same( A, row, col );

		if( n == 0 ) A.push_back( { row, col, val } );
		else{
				A.at(n).value = val;
				sort_rowMajority(A);
		}

		return;
}
void print( sparseMatrix &A ){	//done
		int len = A.size();

		printf( "It's a %d * %d matrix with %lf non-zero terms.\nrow | col | value\n", A.at(0).row, A.at(0).col, A.at(0).value );
		for( int i = 1; i < len; i++ ){
				printf( "%d | %d | %lf\n", A.at(i).row, A.at(i).col, A.at(i).value );
		}

		return;
}
