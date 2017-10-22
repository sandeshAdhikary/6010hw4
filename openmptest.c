#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 2
#define NUM_ELEM 10

double A[NUM_ELEM] = {1,2,3,4,5,6,7,8,9,10};
double B[NUM_ELEM] = {1,2,3,4,5,6,7,8,9,10};
double C[NUM_THREADS];

double Result;



	//rand uniform dist from engine
	double uniform(void)
	{
		return rand() / (RAND_MAX + 1.0);
	}	

	//Matrix Functions
	int **malloc_matrix(int n1, int n2) {

	    int **mat = NULL;       // pointer to the matrix

		if(n1>0 && n2 >0){ //checking for invalid inputs
		    mat = (int**)malloc(n1*sizeof(int *)); //Allocate memory for n1 rows

		    if(mat == NULL){
		    	printf("Error: Could not reserve memory for matrix.");
		    	exit(1);
		    }

			for(int i = 0; i < n1; i++){
				mat[i] = (int*)malloc(n2*sizeof(int)); //Allocate memory for n2 columns for each of the n1 rows

				if (mat[i] == NULL) { //if memory allocation failure
					for (int j = 0; j < i; j++) {  // free memory reserved for all previous cells
						free(mat[j]);
					}

					printf("Could not reserve memory for matrix.");
				    exit(1);
				}


			}
		} else {
			printf("Invalid input for malloc_matrix: Make sure n1 > 0 && n2 > 0");
			exit(1);
		}

	    return mat;
	}


	void free_matrix(int n1, int n2, int **a) {

		if(n1 > 0 && n2 > 0 && a != NULL){ //checking for invalid inputs

			for(int i =0; i<n1; i++){ //Freeing up columns unless they are null
				if(a[i]!=NULL){
					free(a[i]);
				}
			}

			free(a); //Freeing up rows

		} else {
			printf("Invalid input for free_matrix: Make sure n1 > 0 && n2 > 0 && input matrix != NULL");
			exit(1);
		}

	}

	void print_matrix(int n1, int n2, int **a) {

		if(n1 > 0 && n2 > 0 && a != NULL){ //Checking for invalid inputs

			printf("\n");
			for(int i=0;i<n1;i++){
				for(int j=0;j<n2;j++){
					if(j==n2-1){
						printf("%d",a[i][j]); //Doesn't add a comma for the last column
					} else {
						printf("%d,",a[i][j]); //Adds a comma after every element to separate them
					}

				}
				printf("\n");//Print the next row on a different line
			}
		} else {
			printf("Invalid input for print_matrix: Make sure n1 > 0 && n2 > 0");
			exit(1);
		}
	}




int fill_adjmat(int n,double p,int** adjmat){

	int i, nthreads;  //shared variables

	omp_set_num_threads(NUM_THREADS);

	#pragma omp parallel
	{
		
		int row, id, nthrds; //private variables

		double threadC;

		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();

		double prob = p; //define locally so no need to keep requesting global variable

		if(id == 0) nthreads = nthrds; //one thread writes nthreads

		for(row = id; row < n; row+=nthrds) {//round robin loop through threads
				for(int col = 0; col < row; col++){ //loop through columns
						// printf("On the %d th row and %dth columnn with thread no %d\n",row,col,id);	
						double uni = uniform();
						// printf("\nrandom number is: %.2f\n",uni);
						if (uni <= prob){
							adjmat[row][col] = 1;
						}
						else {
							adjmat[row][col] = 0;
						}				
				}

		}
		
		// #pragma omp critical
		// {
		// C[id] = threadC;	
		// }

		// printf("Hello(%d,%d)\n",ID,num_threads);
		// printf("World(%d,%d)\n",ID,num_threads);

	}

	//fill up the rest of the adj matrix outside the parallel computations

	for(int row = 0; row < n; row++){ //loop through labels
		for(int col = row; col < n; col++){ //loop through columns
			
			if(row == col){
				adjmat[row][col] = 0; //diagonal elements are always 0
			}
			else {
				adjmat[row][col] = adjmat[col][row]; //put the transpose elements in off diagonals
			
			}

		}
	} 	



	// printf("Done!\n");


}



int main(void){

	int n = 20;
	double p = 0.1;


	//malloc space for and create an N*N matrix
	int** adjmat = malloc_matrix(n,n);


	//fill up adjmat
	fill_adjmat(n,p,adjmat);

	//print it out
	printf("\nThe adjacency matrix:\n");
	print_matrix(n,n,adjmat);	


}

