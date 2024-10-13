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
		int n_w = 1, n_b = 1;
		//delete this
		sparseMatrix W[n_w], X,B[n_b], Z;
		add_entry( W[0], 0, 0, 1, 0 );
		add_entry( W[0], 0, 2, 2, 0 );
		add_entry( W[0], 1, 0, -1, 0 );
		add_entry( W[0], 1, 1, 4, 0 );
		add_entry( W[0], 1, 2, 6, 0 );

		add_entry( X, 0, 0, 3, 0 );
		add_entry( X, 0, 1, -1, 0 );
		add_entry( X, 2, 0, 2, 0 );
		add_entry( X, 2, 2, 5, 0 );

		add_entry( B[0], 0, 0, 1, 0 );
		add_entry( B[0], 0, 1, 1, 0 );
		add_entry( B[0], 0, 2, 1, 0 );
		add_entry( B[0], 1, 0, 1, 0 );
		add_entry( B[0], 1, 1, 1, 0 );
		add_entry( B[0], 1, 2, 1, 0 );

		print(W[0]);
		print(X);
		print(B[0]);
		/*
		printf( "Please input the number of \"W\" matrix (enter an integer greater than 0): " );
		scanf( "%d", &n_w );
		printf( "Please input the number of \"B\" matrix (enter an integer greater than 0): " );
		scanf( "%d", &n_b );
		printf( "------------------------------------------------------------------------------------\n" );

		//create the matrices
		sparseMatrix W[n_w], X, B[n_b], Z;

		printf( "Now, you'll have to enter the entries of matrices one by one in a designated format.\n" );
		printf( "Let's have a quick example to show how you should give instructions to this program: imagine that you told the program you want 3 W matrices, then you have to enter \"+ 0 1 -6\" to add 0th row, 1st column a \"-6\" entry in the first W matrix. You can add as more entries as you want to the matrix.\n\nAnd as you're done with it and want to start entering the entries of the second W, simply enter a \"n\". So that you'll be able to go on to the the matrix.\n" );
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
				printf( "\nX matrix #0:\n" );
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
						printf( "\nB matrix #%d:\n", i );
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
				print(B[0]); //delete this*/
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

		print(Z);
				//B
		for( int i = 0; i < n_b; i++ ){
				if( ( B[i].at(i).row != Z.at(i).row ) || ( B[i].at(i).col != Z.at(i).col ) ){
						printf( "Some B matrices were found to be incompatible for performing matrix addition, failed to achieve the computation of Fully Connected Layer.\n" );
						return 0;
				}

				Z = sparse_matrix_add( Z, B[i] );
		}

		printf( "\nSuccessfully complete the computation of Fully Connected Layer!\nHere are the non-zero terms of Z:\n" );
		print(Z);

		sparseMatrix Z_a1 = ReLU(Z), Z_a2 = Sigmoid(Z);
		printf("\nHere are the result of Z after manipulated by the ReLU function:\n");
		print(Z_a1);
		printf("\nHere are the result of Z after manipulated by the Sigmoid function:\n");
		print(Z_a2);

		printf( "------------------------------------------------------------------------------------\n" );
		printf("If you would like to search an entry of any specific matrix, feel free to do it in the designated format:\n");
		printf("(matrix name, matrix num(if exists), row, column)  e.g.: \"B 0 1 2\" or \"X 1 1\"\nMatrix Names: W, X, B, Z, R(Z after ReLU), S(Z after Sigmoid)\nIf you've done with searching and is looking forward to end the whole program, simply enter \"z\"\n");
		while(1){
				printf("> ");
				char m = '?';
				int n = 0, row = 0, col = 0;
				scanf( "%c", &m );

				if( ( m == 'B' ) || ( m == 'b' ) ){
						scanf( " %d %d %d", &n, &row, &col );
						print_entry( B[n], row, col );
				}
				else if( ( m == 'w' ) || ( m == 'W' ) ){
						scanf( " %d %d %d", &n, &row, &col );
						print_entry( B[n], row, col );
				}
				else if( ( m == 'X' ) || ( m == 'x' ) ){
						scanf( " %d %d", &row, &col );
						print_entry( X, row, col );
				}
		}


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
		//delete 0 terms
		for( int i = 1; i < result.size(); i++ ){
				if( result.at(i).value == 0 ){
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
