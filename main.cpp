#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <mutex>         
#include <condition_variable>
#include <cmath>

using namespace std;

mutex mtx;
condition_variable cv;
int phases;
int n = 0;
int **A = 0;
int working = 0;

void swap(int *xp, int *yp)  
{  
    int temp = *xp;  
    *xp = *yp;  
    *yp = temp;  
}  
  
void bubbleSortColumn(int** arr, int n, int col)  
{  
   int i, j;  
    for (i = 0; i < n-1; i++){      
    // Last i elements are already in place  
    	for (j = 0; j < n-i-1; j++) {
        	if (arr[j][col-1] > arr[j+1][col-1])  
            	swap(&arr[j][col-1], &arr[j+1][col-1]); 
    	}
    }
}

 
void bubbleSortRow(int** arr, int n, int row)  
{  
   int i, j;  
    for (i = 0; i < n-1; i++){      
    // Last i elements are already in place  
    	for (j = 0; j < n-i-1; j++) { 
        	if (arr[row-1][j] > arr[row-1][j+1])  
            	swap(&arr[row-1][j], &arr[row-1][j+1]); 
    	}
    } 
}

void bubbleSortRowRV(int** arr, int n, int row)  
{  
   int i, j;  
    for (i = 0; i < n-1; i++){      
    // Last i elements are already in place  
    	for (j = 0; j < n-i-1; j++){ 
        	if (arr[row-1][j] <= arr[row-1][j+1])  
            	swap(&arr[row-1][j], &arr[row-1][j+1]);  
  		}
  	}
}   

void printer(int **array, int n)
{
	for (int i = 0; i < n; i++){
		cout << "\n";
		for (int j = 0; j < n; j++){
  			cout << array[i][j] << " ";
		}
	}
	cout << "\n";
}

void* rowSort(void *t)
{

	int row = *((int*)(&t));
	row++; 
	if(row % 2 == 1){
		bubbleSortRow(A, n, row);}
	else if(row % 2 == 0)
		bubbleSortRowRV(A, n, row);
	
	pthread_exit((void*) t);
}

void* colSort(void *t)
{
	int col = *((int*)(&t));
	col++;
	bubbleSortColumn(A, n, col);
	
	pthread_exit((void*) t);
}

int main()
{
	int i, j;
	
	ifstream f("m.txt");
	f >> n;

	phases = round(log2(n*n) + 1);
	A = new int *[n];
	
	for(int i = 0 ; i < n ; i++ )
	A[i] = new int[n];

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
  			f >> A[i][j];	

  	cout << "Initial Matrix:";
	printer(A, n);
	cout<<"\n";

    int rc;
    void * status;
    pthread_t taskids[n], t, threads[n];

    for(int i = 1; i<phases+1; i++){
	    if(i % 2 == 1)
	    {
		    for(t=0; t < n; t++){
		    	taskids[t];
		      	//printf("In main: creating thread ROW %ld\n", t);
		      	rc = pthread_create(&threads[t], NULL, rowSort,(void*)t);
		      	if (rc){printf("ERROR; return code from pthread_create() is %d\n", rc);exit(-1);}
		    }
		}
		else if(i % 2 == 0)
	    {
		    for(t=0; t < n; t++){
		    	taskids[t];
		      	//printf("In main: creating thread COL %ld\n", t);
		      	rc = pthread_create(&threads[t], NULL, colSort, (void *)t);
		      	if (rc){printf("ERROR; return code from pthread_create() is %d\n", rc);exit(-1);}
		    }
		}
		for(int i = 0; i < n; i++){pthread_join(threads[i], &status);}
		cout<<"Phase " << i;
		printer(A,n);
		cout<<"\n";
	}
	cout<<"Final Result:";
	printer(A,n);
	
	for(int i = 0 ; i < n ; i++ ){delete [] A[i];} delete [] A;

	pthread_exit(NULL);
	return 0;
}
