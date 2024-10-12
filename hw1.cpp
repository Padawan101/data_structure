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
		int n_w = 0, n_b = 0;
		printf( "Please input the number of \"W\" matrix (enter an integer greater than 0): " );
		scanf( "%d", &n_w );
		printf( "Please input the number of \"B\" matrix (enter an integer greater than 0): " );
		scanf( "%d", &n_b );
		printf( "------------------------------------------------------------------------------------\n" );

		//create the matrices
		sparseMatrix W[n_w], X, B[n_b], Z;

		printf( "Now, you'll have to enter the entries of matrices one by one in a designated format.\n" );
		printf( "Let's have a quick example to show how you should give instructions to this program: previously you told the program you want 3 W matrices, then you have to enter \"+ 0 1 -6\" to add 0th row, 1st column a \"-6\" entry in the first W matrix. You can add as more entries as you want to the matrix.\n\nAnd as you're done with it and want to start entering the entries of the second W, simply enter a \"n\". So that you'll be able to go on to the the matrix.\n" );
		printf( "------------------------------------------------------------------------------------\n" );
				char instruction = '?';
				//creating "n_w" W matrices
				for( int i = 0; i < n_w; i++ ){
						printf( "W matrix #%d:\n", i );
						while(1){
								//adding entries to the "i^th" W matrix
								int row = 0, col = 0, val = 0;
								printf("> ");
								scanf( " %c", &instruction );

								if( instruction == '+' ){
										scanf( " %d %d %d", &row, &col, &val );
										if( ( row < 0 ) || ( col < 0 ) ) printf( "Invalid position of matrix, failed to add an entry.\n" );
										else add_entry( W[i], row, col, val, 1 );
								}
								else if( instruction == 'n' ){
										cin.ignore( numeric_limits<std::streamsize>::max(), '\n' );
										break;
								}
								else{
										printf( "Invalid instruction, check the format again.\n" );
										cin.ignore( numeric_limits<std::streamsize>::max(), '\n' );
								}

						}
				}
				print(W[0]); //delete this
				//creating the X matrix
				printf( "X matrix #0:\n" );
						while(1){
								//adding entries to the "i^th" W matrix
								int row = 0, col = 0, val = 0;
								printf("> ");
								scanf( " %c", &instruction );

								if( instruction == '+' ){
										scanf( " %d %d %d", &row, &col, &val );
										if( ( row < 0 ) || ( col < 0 ) ) printf( "Invalid position of matrix, failed to add an entry.\n" );
										else add_entry( X, row, col, val, 1 );
								}
								else if( instruction == 'n' ){
										cin.ignore( numeric_limits<std::streamsize>::max(), '\n' );
										break;
								}
								else{
										printf( "Invalid instruction, check the format again.\n" );
										cin.ignore( numeric_limits<std::streamsize>::max(), '\n' );
								}

						}
						print(X); //delete this
				//creating "n_b" B matrices
				for( int i = 0; i < n_b; i++ ){
						printf( "B matrix #%d:\n", i );
						while(1){
								//adding entries to the "i^th" W matrix
								int row = 0, col = 0, val = 0;
								printf("> ");
								scanf( " %c", &instruction );

								if( instruction == '+' ){
										scanf( " %d %d %d", &row, &col, &val );
										if( ( row < 0 ) || ( col < 0 ) ) printf( "Invalid position of matrix, failed to add an entry.\n" );
										else add_entry( B[i], row, col, val, 1 );
								}
								else if( instruction == 'n' ){
										cin.ignore( numeric_limits<std::streamsize>::max(), '\n' );
										break;
								}
								else{
										printf( "Invalid instruction, check the format again.\n" );
										cin.ignore( numeric_limits<std::streamsize>::max(), '\n' );
								}

						}
				}
				print(B[0]); //delete this
		printf( "------------------------------------------------------------------------------------\n" );

		//computing the fully connected layer
				//Ws
		Z = W[0];
		for( int i = 1; i < n_w; i++ ){
				if( Z.at(0).col != B[i].at(0).row ){
						printf( "Some of the W matrices were found to be incompatible for performing matrix multiplication, failed to achieve the computation of Fully Connected Layer.\n" );
						return 0;
				}
				
				Z = sparse_matrix_multiply( Z, W[i] );
		}

				//X
		if( Z.at(0).col != X.at(0).row ){
				printf( "The last W matrix and X matrix were found to be incompatible for performing matrix multiplication, failed to achieve the computation of Fully Connected Layer.\n" );
				return 0;
		}
		Z = sparse_matrix_multiply( Z, X );

				//B
		for( int i = 0; i < n_b; i++ ){
				if( ( Z.at(0).row != X.at(0).row ) || ( Z.at(0).col != X.at(0).col ) ){
						printf( "Some B matrices were found to be incompatible for performing matrix addition, failed to achieve the computation of Fully Connected Layer.\n" );
						return 0;
				}

				Z = sparse_matrix_add( Z, B[i] );
		}

		printf( "Successfully complete the computation of Fully Connected Layer!\nHere are the non-zero terms of Z:\n" );
		print(Z);
		

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
int compare( int a, int b ){	//done
		if( a > b ) return 1;
		else if( a == b ) return 0;
		else return -1;
}
