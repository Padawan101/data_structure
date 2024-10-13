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
void add_entry( sparseMatrix &A, int row, int col, double val, bool flag );	//add a new entry to a sparse matrix
void print( sparseMatrix &A );				//print the whole sparse matrix
int bs_same( sparseMatrix &A, int row, int col );	//find whether an entry(row^th, col^th term) existed in an sparse matrix
void sort_rowMajority( sparseMatrix &A );	//to sort a sparse matrix by row-majority
sparseMatrix transpose( sparseMatrix &A );			//make the matrix available for the multiply function
int compare( int a, int b );

int main(){
		sparseMatrix A, B;
		add_entry( A, 0, 0, 1, 0 );
		add_entry( A, 0, 2, 2, 0 );
		add_entry( A, 1, 0, -1, 0 );
		add_entry( A, 1, 1, 4, 0 );
		add_entry( A, 1, 2, 6, 0 );
		add_entry( A, 2, 2, 99, 0 );

		add_entry( B, 0, 0, 3, 0 );
		add_entry( B, 0, 1, -1, 0 );
		add_entry( B, 2, 0, 2, 0 );
		add_entry( B, 2, 2, 5, 0 );

		print(A);
		print(B);
		printf("\n\n\n");

		//sparseMatrix Z = sparse_matrix_multiply( A, B );
		sparseMatrix Z = sparse_matrix_multiply( A, B );
		print( Z );

		return 0;
}


void print_entry( sparseMatrix &A, int row, int col ){	//done
		int n = bs_same( A, row, col );

		if( n == 0 ) printf( "value: 0\n" );
		else printf( "value: %lf\n", A.at(n).value );

		return;
}
sparseMatrix sparse_matrix_add( sparseMatrix &A, sparseMatrix &B ){	//re
		sort_rowMajority(A);
		sort_rowMajority(B);
		sparseMatrix result = A;

		int len = B.size();
		for( int i = 1; i < len; i++ ){
				int n = bs_same( result, B.at(i).row, B.at(i).col );

				if( n == 0 ){
						add_entry( result, B.at(i).row, B.at(i).col, B.at(i).value, 0 );
				}
				else result.at(n).value += B.at(i).value;
		}

		return result;
}
sparseMatrix sparse_matrix_multiply( sparseMatrix &A, sparseMatrix &B ){	//
		sparseMatrix result;
		printf("cp3\n");
		sparseMatrix tmp = transpose(B);
		printf("cp4\n");
		int row = A.at(1).row, col = 0;
		int row_begin = 1;

		for( int i = 1; i < A.size(); ){
				double sum = 0;
				col = tmp.at(1).row;
				printf("cp 9\n");
				for( int j = 1; j < B.size(); ){	//problem
						printf("cp 99\n");
						if( A.at(i).row != row ){
								add_entry( result, row, col, sum, 1 );
								i = row_begin;
								for( ; tmp.at(j).row == col; j++ );
								col = tmp.at(j).row;
						}
						else if( tmp.at(j).row != col ){
								add_entry( result, row, col, sum, 1 );
								i = row_begin;
								col = tmp.at(j).row;
						}
						else{
								switch( compare( A.at(i).col, tmp.at(j).col ) ){
										case (-1):
												i++;
												break;
										case (0):
												sum += ( A.at(i++).value * tmp.at(j++).value );
												break;
										case (1):
												j++;
								}
						}
				}

				for( ; A.at(i).row == row; i++ );
				row_begin = i;
				row = A.at(i).row;
		}
		printf("cp 1\n");

		result.at(0).row = A.at(0).row, result.at(0).col = B.at(0).col, result.at(0).value = ( result.size() - 1 );

		return result;
}
sparseMatrix ReLU( sparseMatrix &A ){	//done
		sparseMatrix result = A;

		int len = A.size();
		for( int i = 1; i < len; i++ ){
				result.at(i).value = max( 0.0, result.at(i).value );
		}

		return result;
}
sparseMatrix Sigmoid( sparseMatrix &A ){	//done
		sparseMatrix result = A;

		//tranform the existing terms
		int len = A.size();
		for( int i = 1; i < len; i++ ){
				A.at(i).value = ( 1 / ( 1 + exp( (-1) * A.at(i).value ) ) );
		}
		//turn the 0 term to 0.5
		for( int i = 0; i < A.at(0).row; i++ ){
				for( int j = 0; j < A.at(0).col; j++ ){
						add_entry( A, i, j, 0.5, 0 );
				}
		}

		return result;
}

void sort_rowMajority( sparseMatrix &A ){	//re
		int len = A.size();
		if( len > 2 ){
				for( int i = 1; i < len; i++ ){
						for( int j = 1; j < ( len - 1 - i ); j++ ){
								if( A.at(j).row > A.at(j + 1).row ){
										Triple tmp = A.at(j);
										A.at(j) = A.at(j + 1);
										A.at(j + 1) = tmp;
								}
						}
				}
		}

		return;
}
int bs_same( sparseMatrix &A, int row, int col ){	//re
		if( A.size() > 1 ){
				sort_rowMajority(A);
				

				int left = 1, right = A.size() - 1;
				//binary search
				while( left <= right ){
						int mid = ( left + right ) / 2;
						if( A.at(mid).row < row ) left = mid + 1;
						else if( A.at(mid).row > row ) right = mid - 1;
						else{
								int n = A.size();
								for( int i = 1; i < n; i++ ){
										if( ( A.at(i).col == col ) && ( A.at(i).row == row ) ) return i;
								}
								break;
						}
				}
		}
		return 0;
}
void add_entry( sparseMatrix &A, int row, int col, double val, bool flag ){	//re
		int n = 0;
		if( A.size() < 1 ) A.push_back( {row + 1, col + 1, 0} );
		else n = bs_same( A, row, col );

		if( n == 0 ){
				if( row >= A.at(0).row ) A.at(0).row++;
				if( col >= A.at(0).col ) A.at(0).col++;
				A.at(0).value++;
						
				A.push_back( { row, col, val } );
		}
		else if( flag ){
				A.at(n).value = val;
		}

		sort_rowMajority(A);

		return;
}
void print( sparseMatrix &A ){	//done
		int len = A.size();

		printf( "It's a %d * %d matrix with %d non-zero terms.\n row | col | value\n", A.at(0).row, A.at(0).col, ( (int) A.at(0).value ) );
		for( int i = 1; i < len; i++ ){
				printf( "%d | %d | %lf\n", A.at(i).row, A.at(i).col, A.at(i).value );
		}

		return;
}
sparseMatrix transpose( sparseMatrix &A ){	//re
		sparseMatrix result;

		for( int i = 1; i < A.size(); i++ ){
				add_entry( result, A.at(i).col, A.at(i).row, A.at(i).value, 0 );
		}
		sort_rowMajority(A);

		return result;
}
int compare( int a, int b ){	//done
		if( a > b ) return 1;
		else if( a == b ) return 0;
		else return -1;
}
