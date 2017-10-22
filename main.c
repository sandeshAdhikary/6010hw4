#include <stdio.h>
#include <stdlib.h>
#include <time.h>       // for time, clock



#define WHITE 1
#define GRAY 2
#define BLACK 3


int compsize = 0; //initialize time at 0
int largestcompsize = 0; //keep track of the largest component encountered



typedef struct node {
	int color;
	struct node* pred;
	int d;
	int f;
	int nodenumber;
} node;


//Function Prototypes
	
	//Matrix Prototypes
	int **malloc_matrix(int n1, int n2);
	void free_matrix(int n1, int n2, int **a);
	void print_matrix(int n1, int n2, int **a);

	// List Prototypes
	node** init_searchlist(int n);

	//Randomness prototypes
	double uniform(void);


//Function Definitions
	
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

	node** init_nodelist(int n) {

		//Check for invalid inputs
		if(n <= 0){printf("Could not initialize nodelist. Make sure n > 0");exit(1);} 

		//malloc space for and create an n*1 matrix i.e. 1D array
		node** nodelist = (node**)malloc(n*sizeof(node)); 

		//check that the above malloc worked
		if(nodelist == NULL){printf("\nUnable to reserve memory for nodelist\n");exit(1);}

		//initialize search matrix with all WHITEs
		for(int i = 0; i < n; i++){ //loop through labels
			node* v = (node*)malloc(sizeof(node));
			v->color = WHITE;
			v->nodenumber = i;
			nodelist[i] = v;
		} 

		return nodelist;

	}	

	void free_nodelist(int n,node** nodelist){
		
		for(int i = 0; i < n; i++){ //loop through labels
			if(nodelist[i] != NULL){
				free(nodelist[i]);
			}
		} 		
	}


void dfsVisit(int n, int** adjmat, node* inputnode,  node** nodelist){

	// printf("\n---Starting DFSVisit at node number: %d\n",inputnode->nodenumber);

	compsize += 1;

	// inputnode->d = timer;
	inputnode->color = GRAY;
	// printf("\n-----[Compsize: %d]: Grayed node number %d\n",compsize,inputnode->nodenumber);

	//loop through each neighbor of inputnode
	for (int neighborcount = 0; neighborcount < n;neighborcount++){
		if(adjmat[inputnode->nodenumber][neighborcount] == 1){
			
			//retrieve the neighbor's node
			node* neighbornode = nodelist[neighborcount];
			// printf("\n\tNeighbor color %d\n",neighbornode->color);
			
			//if the neighbornode is WHITE, schedule a dfsVisit
			if (neighbornode->color == WHITE){
				// printf("\n\tNeighbor number %d is white\n",neighbornode->nodenumber);
				neighbornode->pred = inputnode;
				// printf("\n\t[Compsize: %d]: Scheduled a dfsVisit for node number %d\n",compsize,neighbornode->nodenumber);
				dfsVisit(n,adjmat,neighbornode,nodelist);
			} else {
				// printf("\n\tNeighbor not white\n");
			}
		}
	}

	//blacken the input node; it is finished
	inputnode->color = BLACK;
	// timer = timer + 1;
	// inputnode->f = timer;
	
	//update the value of largest compsize after every dfVisit
	if(largestcompsize < compsize){largestcompsize = compsize;printf("\nLargest Comp Size updated to: %d",largestcompsize);}	
	// printf("\n---[Compsize: %d]: Blackeded node number %d\n",compsize,inputnode->nodenumber);

}

void dfs(int n, int** adjmat, node** nodelist){

	for (int nodecount = 0; nodecount < n; nodecount++){
		node* currentnode = nodelist[nodecount];
		if(currentnode->color == WHITE){

			//reset the compsize when starting from a new source node
			compsize = 0;
			
			//start a new DFS search
			printf("\nStarting DFS search at node number: %d\n",currentnode->nodenumber);
			dfsVisit(n,adjmat,currentnode,nodelist);
		}
	}
}



//rand uniform dist from engine
double uniform(void)
{
	return rand() / (RAND_MAX + 1.0);
}


///////////////////////////////////////////////////////////////////////////////////////
//////////// MAIN PROGRAM
///////////////////////////////////////////////////////////////////////////////////////

int main (int argc, const char * argv[])
{
	
	//check parameters entered at terminal
    if (argc != 3) {
    	printf("\nIncorrect number of input parameters!\n");
    	exit(1);
    }

    int n = (int) strtol(argv[1], NULL, 10); //Number of nodes
    double p = strtod(argv[2], NULL);		 // probability of two nodes having a connecting link

    if (n <= 0 || p < 0 || p > 1){
    	printf("\nInvalid inputs: Make sure n > 0 and 0 <= p < 1\n");
    	exit(1);
    }
printf

    ("\nGenerating Adjacency Matrix with the following parameters:\n");
    printf("\nNumber of Nodes: %d\n",n);
	printf("\nProbability of a link connecting two nodes: %.2f\n",p);


	srand(time(NULL));


	//Build the Adjacency Matrix
			//malloc space for and create an N*N matrix
			int** adjmat = malloc_matrix(n,n);

			if(adjmat == NULL){ //check that the above malloc worked
				printf("\nUnable to reserve memory for adjacency matrix\n");
				exit(1);
			} 	


			//fill the matrix
			for(int row = 0; row < n; row++){ //loop through labels
				for(int col = 0; col < row+1; col++){ //loop through columns
					//diagonal elements are always 0
					if(row == col){
						adjmat[row][col] = 0;
					}
					else {
						double uni = uniform();
						// printf("\nrandom number is: %.2f\n",uni);
						if (uni <= p){
							adjmat[row][col] = 1;
							adjmat[col][row] = 1;
						}
						else {
							adjmat[row][col] = 0;
							adjmat[col][row] = 0;
						}				
					}

				}
			} 



			//print out adjmat 
			printf("\nThe adjacency matrix:\n");
			print_matrix(n,n,adjmat);

	//Build the nodelist
    node** nodelist = init_nodelist(n);

 //    //print the nodelist before dfs
 //    for (int i = 0; i < n; i++){
 //    printf("\nNode color:%d\n",nodelist[i]->color);
	// }


	//Call the dfs method
	dfs(n,adjmat,nodelist);

	// //print the nodelist after dfs
 //    for (int i = 0; i < n; i++){
 //    printf("\nNode color:%d\n",nodelist[i]->color);
	// }

	//print the largest component size
	printf("\nLargest Component Size: %d\n",largestcompsize);



	//free memory for adjmat
	free_matrix(n,n,adjmat);
	adjmat = NULL;

	//free memory for nodelist
	free_nodelist(n,nodelist);
	nodelist = NULL;

}
