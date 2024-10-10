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
		//create & initialize the matrices
		sparseMatrix W, X, B, Z;
		add_entry( W, 0, 0, 0, 1 );
		add_entry( X, 0, 0, 0, 1 );
		add_entry( B, 0, 0, 0, 1 );
		add_entry( Z, 0, 0, 0, 1 );

		//user-dominated
		

		return 0;
}


void print_entry( sparseMatrix &A, int row, int col ){	//done
		int n = bs_same( A, row, col );

		if( n == 0 ) printf( "value: 0\n" );
		else printf( "value: %lf\n", A.at(n).value );

		return;
}
sparseMatrix sparse_matrix_add( sparseMatrix &A, sparseMatrix &B ){	//done
		sparseMatrix result = A;

		int len = B.size();
		for( int i = 1; i < len; i++ ){
				int n = bs_same( A, B.at(i).row, B.at(i).col );

				if( n == 0 ){
						add_entry( A, B.at(i).row, B.at(i).col, B.at(i).value, 0 );
				}
				else result.at(n).value += B.at(i).value;
		}

		return result;
}
sparseMatrix sparse_matrix_multiply( sparseMatrix &A, sparseMatrix &B ){	//done
		sparseMatrix result, tmp = transpose(B);
		int row = A.at(1).row, col = 0;
		int row_begin = 1, sum = 0;

		for( int i = 1; i <= A.at(0).value; ){
				double sum = 0;
				col = tmp.at(1).row;
				for( int j = 1; j <= ( B.at(0).value + 1 ); ){
						if( A.at(1).row != row ){
								add_entry( result, row, col, sum, 1 );
								i = row_begin;
								for( ; tmp.at(j).row == col; j++ );
								row = tmp.at(j).row;
						}
						else if( tmp.at(j).row != col ){
								add_entry( result, row, col, sum, 1 );
								i = row_begin;
								row = tmp.at(j).row;
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
				//binary search
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
void add_entry( sparseMatrix &A, int row, int col, double val, bool flag ){	//done
		if( A.size() < 1 ) A.push_back( {0, 0, 0} );
		int n = bs_same( A, row, col );

		if( n == 0 ){
				if( row > A.at(0).row ) A.at(0).row++;
				if( col > A.at(0).col ) A.at(0).col++;
				A.at(0).value++;
						
				A.push_back( { row, col, val } );
		}
		else if( flag ){
				A.at(n).value = val;
				sort_rowMajority(A);
		}

		return;
}
void print( sparseMatrix &A ){	//done
		int len = A.size();

		printf( "It's a %d * %d matrix with %lf non-zero terms.\n row | col | value\n", A.at(0).row, A.at(0).col, A.at(0).value );
		for( int i = 1; i < len; i++ ){
				printf( "%d | %d | %lf\n", A.at(i).row, A.at(i).col, A.at(i).value );
		}

		return;
}
sparseMatrix transpose( sparseMatrix &A ){	//done
		sparseMatrix result;

		//initialize the table
		int table[2][( (int) A.at(0).value )];
		for( int i = 0; i < A.at(0).col; i++ ){
				table[0][i] = 0, table[1][i] = 0;
		}

		//calculate "(row)' terms" & "starting row position"
		sort_rowMajority(A);
		for( int i = 0; i < A.at(0).value; i++ ){	//row terms
				table[0][A.at(i).col]++;
		}

		table[1][0] = 1;	//starting row position
		for( int i = 1; i < A.at(0).col; i++ ){
				table[1][i] = table[0][i - 1] + table[1][i - 1];
		}

		//create a transposed matrix
		for( int i = 0; i < A.at(0).value; i++ ){
				if( table[0][i] > 0 ){
						add_entry( result, table[1][A.at(i).col], A.at(i).row, A.at(i).value, 0 );
						table[1][A.at(i).col]++;
				}
		}

		return result;
}
int compare( int a, int b ){
		if( a > b ) return 1;
		else if( a == b ) return 0;
		else return -1;
}
