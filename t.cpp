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
		//add_entry( A, 2, 2, 99, 0 );

		add_entry( B, 0, 0, 3, 0 );
		add_entry( B, 0, 1, -1, 0 );
		add_entry( B, 2, 0, 2, 0 );
		add_entry( B, 2, 2, 5, 0 );

		print(A);
		print(B);

		sparseMatrix Z = sparse_matrix_multiply( A, B );
		//sparseMatrix Z = sparse_matrix_add( A, B );
		print( Z );

		Z = ReLU(Z);
		print(Z);

		Z = Sigmoid(Z);
		print(Z);

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
sparseMatrix sparse_matrix_multiply( sparseMatrix &A, sparseMatrix &B ){	//re
		sparseMatrix result, tmp = transpose(B);
		
		for( int i = 0; i < A.at(0).row; i++ ){	//A's row iteration
				double sum = 0;
				int row_begin = A.at(1).row;

				for( int j = 1; j <= ( tmp.size() - 1 ); j++ ){	//B^T's row iteration
						if( row_begin == tmp.at(j).row ){
								int n = bs_same( A, i, tmp.at(j).col );
								if( n != 0 ) sum += ( tmp.at(j).value * A.at(n).value );
						}
						else{
								if( sum != 0 ) add_entry( result, i, row_begin, sum, 1 ), sum = 0;
								row_begin = tmp.at(j).row, j--;
						}

						if( ( j == ( tmp.size() - 1 ) ) && ( sum != 0 ) ){
								if( sum != 0 ) add_entry( result, i, row_begin, sum, 1 );
						}
				}
		}

		return result;
}
sparseMatrix ReLU( sparseMatrix &A ){	//re
		sparseMatrix result = A;

		for( int i = 1; i < result.size(); i++ ){
				if( result.at(i).value < 0 ){
						//adjust result.at(0).col
						if( result.at(i).col == ( result.at(0).col - 1 ) ){
								int flag = 0;
								for( int j = 1; j < result.size(); j++ ){
										if( j != i ){
												int n = bs_same( result, result.at(j).row, result.at(i).col );
												if( n != 0 ){
														flag = 1;
														break;
												}
										}
								}
								if( !flag ){
										int col_new = result.at(1).col;
										for( int j = 2; j < result.size(); j++ ){
												if( result.at(j).col > col_new ) col_new = result.at(j).col;
										}
										result.at(0).col = col_new + 1;
								}
						}
						//adjust result.at(0).row
						if( result.at(i).row == ( result.at(0).row - 1 ) ){
								int flag = 0;
								for( int j = 1; j < result.size(); j++ ){
										if( j != i ){
												int n = bs_same( result, result.at(i).row, result.at(j).col );
												if( n != 0 ){
														flag = 1;
														break;
												}
										}
								}
								if( !flag ){
										int row_new = result.at(1).row;
										for( int j = 2; j < result.size(); j++ ){
												if( result.at(j).row > row_new ) row_new = result.at(j).row;
										}
										result.at(0).row = row_new + 1;
								}
						}

						result.erase( result.begin() + i );
						result.at(0).value--;
						i = 1;
				}
		}

		return result;
}
sparseMatrix Sigmoid( sparseMatrix &A ){	//re
		sparseMatrix result = A;

		//tranform the existing terms
		int len = A.size();
		for( int i = 1; i < len; i++ ){
				result.at(i).value = ( 1 / ( 1 + exp( (-1) * A.at(i).value ) ) );
		}
		//turn the 0 term to 0.5
		for( int i = 0; i < A.at(0).row; i++ ){
				for( int j = 0; j < A.at(0).col; j++ ){
						add_entry( result, i, j, 0.5, 0 );
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
int bs_same(sparseMatrix &A, int row, int col) {	//re
    if (A.size() > 1) {
        sort_rowMajority(A);  // Ensure sorted by row-majority

        int left = 1, right = A.size() - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (A.at(mid).row < row) {
                left = mid + 1;
            } else if (A.at(mid).row > row) {
                right = mid - 1;
            } else {
                // Binary search within the same row for col
                if (A.at(mid).col < col) {
                    left = mid + 1;
                } else if (A.at(mid).col > col) {
                    right = mid - 1;
                } else {
                    return mid;  // Found the element
                }
            }
        }
    }
    return 0;  // Element not found
}/*
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
}*/
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
