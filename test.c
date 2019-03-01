#include "points_segments.h"

void print_array(int *arr, int size){
	for (int i=0; i<size; i++){
		printf("%d ",arr[i]);	
	}
	printf("\n");
}

void print_array_2D(int **arr, int rows, int cols){
	for (int i=0; i<rows; i++){
		printf("[");
		for (int j=0; j<cols; j++)
			printf("%d ",arr[i][j]);
		printf("] ");
	}
	printf("\n");
}

int* points_segments_naive(int ** segments, int * points, int num_points, int rows, int * arr){
    for (int i = 0; i < num_points; i++){
        arr[i] = 0;
        for (int j = 0; j < rows; j++){
            if (points[i]>=segments[j][0] && points[i]<=segments[j][1]){
                arr[i]++;
            }
        }
    }
    printf("Naive algorithm: ");
    print_array(arr,num_points);
    return arr;
}

void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void free_array(int **arr, int row){
    for (int i = 0; i < row; i++){
        free(arr[i]);
    }
    free(arr);
}

int partition(int* arr, int* ind, int lo, int hi){
    int pivot = arr[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; j++){
        if (arr[j] < pivot){
            i = i+1;
            swap(&arr[i],&arr[j]);
            swap(&ind[i],&ind[j]);
        } else if (arr[j] == pivot) {
            if (ind[j] > 2){
                if (ind[hi] == 2){
                    i = i+1;
                    swap(&arr[i],&arr[j]);
                    swap(&ind[i],&ind[j]);
                }
            } else if (ind[hi] > 2){
                if (ind[j] == 1){
                    i = i+1;
                    swap(&arr[i],&arr[j]);
                    swap(&ind[i],&ind[j]);
                }
            }
        }
    }
    swap(&arr[i+1],&arr[hi]);
    swap(&ind[i+1],&ind[hi]);
    return i+1;
}

void quicksort(int* arr, int* ind, int lo, int hi){
    if (lo<hi){
        int p = partition(arr,ind,lo,hi);
        quicksort(arr,ind,lo,p-1);
        quicksort(arr,ind,p+1,hi);
    }
}

int* points_segments_quicksort(int ** segments, int * points, int num_points, int rows, int * arr){
    int size_arr_sorted = num_points+rows*2;
    int indicator[size_arr_sorted];
    int* arr_sorted = malloc(size_arr_sorted*sizeof(int*));
    
    for (int i = 0; i < rows; i++){
        arr_sorted[2*i] = segments[i][0];
        indicator[2*i] = 1;
        arr_sorted[2*i+1] = segments[i][1];
        indicator[2*i+1] = 2;
    }
    for (int j = 0; j < num_points; j++){
        arr_sorted[2*rows+j] = points[j];
        indicator[2*rows+j] = j+3;
    }
    
    quicksort(arr_sorted,indicator,0,size_arr_sorted-1);

    int count = 0;
    for (int k = 0; k < size_arr_sorted; k++){
        if (indicator[k] == 1){
            count++;
        } else if (indicator[k] == 2){
            count--;
        } else {
            arr[indicator[k]-3] = count;
        }
    }
    free(arr_sorted);
    printf("quicksort algorithm: ");
    print_array(arr,num_points);
    return arr;
}

int main() {	
	const char * files[] = {"input1.txt", "input2.txt", "input3.txt"};   

	for (int f=0; f < 3; f++){
		FILE *pfile = fopen(files[f], "r");
		if(pfile == NULL)  {
			printf("Error opening file %s\n", files[f]);
			return 1;
		}
		
		int s, p;
		while(fscanf(pfile, "%d %d", &s, &p)!=EOF){		
		
			//allocate 2D array to hold s segments
			int ** segments = malloc(s*sizeof(int *));
			for (int i=0; i < s; i++){
				segments[i] = malloc(2*sizeof(int));
			}
		
			//scan all segment lines and populate the segments
			for (int i=0; i < s; i++){
				fscanf(pfile, "%d %d", &segments[i][0], &segments[i][1]);
			}		
		
			//allocate a single array to hold p points
			int * points = malloc(p*sizeof(int));
		
			//scan all point coordinates into points array
			for (int i=0; i < p; i++){
				fscanf(pfile, "%d", &points[i]);
			}
			
			printf("There are total %d segments, and %d points\n", s, p);
			
			print_array_2D(segments, s, 2);

			print_array(points, p);

			//TODO: pass s, p, segments, and points to your two algorithms
			//the output should be an array of size p containing 
			//-for each point- the number of covering segments
            
            int* result1 = malloc(p*sizeof(int));
            int* result2 = malloc(p*sizeof(int));
            points_segments_quicksort(segments, points, p, s, result2);
            points_segments_naive(segments, points, p, s, result1);

            int correct = 1;
            for (int i = 0; i < p; i++){
                if(result1[i] == result2[i]){
                } else {
                    printf("Wrong. The answer is ");
                    print_array(result1,p);
                    correct = 0;
                    break;
                }
            }
            if (correct){
                printf("Correct.\n");
            }
            
			//TODO: implement - compare these outputs from 2 algorithms
            
            free_array(segments,s);
            free(points);
            free(result1);
            free(result2);
        }
        
		fclose(pfile);
	}
	return 0;
}
