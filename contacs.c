//#include <assert.h>
//#include <limits.h>
//#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char** split_string(char*);

/*
 * Complete the contacts function below.
 */
#define SIZE 26

typedef struct _charity {
	char kar;
	bool kelime;
	int tree;
	struct _charity *n[SIZE];
} charity;

typedef struct _results {
	int cik;
	struct _results *next;
} results;

/*
 * Please store the size of the integer array to be returned in result_count pointer. For example,
 * int a[3] = {1, 2, 3};
 *
 * *result_count = 3;
 *
 * return a;
 *
 */
void freeall(charity *mars){
	charity *merkur;
	for(int i=0; i<SIZE; i++){
		if(mars->n[i] != NULL){
			merkur = mars->n[i];
			freeall(merkur);
		}
	}
	free(mars);
}

int* contacts(int queries_rows, int queries_columns, char*** queries, int* result_count) {
	if (queries_columns != 2)
		return NULL;

	charity earth[SIZE];
	for(int x = 97; x < 123; x++){
		earth[x-97].kar=x;
		earth[x-97].kelime = false;
		earth[x-97].tree = 0;
		for(int i=0;i<SIZE;i++)
			earth[x-97].n[i] = NULL;
	}

	char *turn;
	int addt = 0;
	results *may = (results *)malloc(sizeof(results));
	may->cik = 0;
	may->next = NULL;
	results *smay = may;
	results *nhold = may;
	charity *mars;

	for (int i=0; i<queries_rows;i++){
		if (strcmp("add", queries[i][0]) == 0){
			addt++;
			turn = queries[i][1];

			mars = &earth[(*(turn++)-97)];
			mars->tree +=1;
			if(mars->tree == 1){
				for(int i=0;i<SIZE;i++)
					mars->n[i] = NULL;
			}
			while(mars->n[*(turn)-97]){
				mars = mars->n[*(turn++)-97];
				mars->tree += 1;
			}

			while(*turn){
				mars->n[*(turn)-97] = (charity *) malloc(sizeof(charity));
				mars = mars->n[*(turn)-97];
				mars->kar = *(turn++);
				mars->tree = 1;
				mars->kelime = false;
				for(int i=0;i<SIZE;i++)
					mars->n[i] = NULL;

			}
			mars->kelime = true;
		}
		else{
			*result_count += 1;
			if(!may){
				may = (results *)malloc(sizeof(results));
				nhold->next = may;
				nhold = may;
				may->cik = 0;
				may->next = NULL;
			}

			if (addt == 0){
				may->cik = 0;
			}
			else{
				//unsigned long int bingo = 0;
				turn = queries[i][1];
				mars = &earth[(*(turn++)-97)];
				while(*(turn) && mars->n[*(turn)-97]){
					mars = mars->n[*(turn++)-97];
				}
				if(mars && (!*turn))
					may->cik = mars->tree;
				else
					may->cik = 0;
			}
			may = may->next;
		}
	}

	if (*result_count == 0){
		return NULL;
	}

	int *resarray = (int *)malloc((*result_count) * sizeof(int));
	may = smay;
	int i = 0;

	while(may){
		resarray[i] = may->cik;
		i++;
		smay = may->next;
		free(may);
		may = smay;
	}

	//FREE ALL GRAPH
	for(int i=0; i<SIZE; i++){
		if(earth[i].tree==0){
			continue;
		}
		for(int j=0; j<SIZE; j++){
			if (earth[i].n[j]){
				mars = earth[i].n[j];
				freeall(mars);
			}
		}
	}

	return resarray;
}


int main()
{
	FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

	char* queries_rows_endptr;
	char* queries_rows_str = readline();
	int queries_rows = strtol(queries_rows_str, &queries_rows_endptr, 10);

	if (queries_rows_endptr == queries_rows_str || *queries_rows_endptr != '\0') { exit(EXIT_FAILURE); }

	char*** queries = malloc(queries_rows * sizeof(char**));

	for (int queries_row_itr = 0; queries_row_itr < queries_rows; queries_row_itr++) {
		queries[queries_row_itr] = malloc(2 * (sizeof(char*)));

		char** queries_item_temp = split_string(readline());

		for (int queries_column_itr = 0; queries_column_itr < 2; queries_column_itr++) {
			char* queries_item = queries_item_temp[queries_column_itr];

			queries[queries_row_itr][queries_column_itr] = queries_item;
		}
	}

	int result_count = 0;
	int* result = contacts(queries_rows, 2, queries, &result_count);

	for (int result_itr = 0; result_itr < result_count; result_itr++) {
		fprintf(fptr, "%d", result[result_itr]);

		if (result_itr != result_count - 1) {
			fprintf(fptr, "\n");
		}
	}

	fprintf(fptr, "\n");

	fclose(fptr);

	return 0;
}

char* readline() {
	size_t alloc_length = 1024;
	size_t data_length = 0;
	char* data = malloc(alloc_length);

	while (true) {
		char* cursor = data + data_length;
		char* line = fgets(cursor, alloc_length - data_length, stdin);

		if (!line) { break; }

		data_length += strlen(cursor);

		if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') { break; }

		size_t new_length = alloc_length << 1;
		data = realloc(data, new_length);

		if (!data) { break; }

		alloc_length = new_length;
	}

	if (data[data_length - 1] == '\n') {
		data[data_length - 1] = '\0';
	}

	data = realloc(data, data_length);

	return data;
}

char** split_string(char* str) {
	char** splits = NULL;
	char* token = strtok(str, " ");

	int spaces = 0;

	while (token) {
		splits = realloc(splits, sizeof(char*) * ++spaces);
		if (!splits) {
			return splits;
		}

		splits[spaces - 1] = token;

		token = strtok(NULL, " ");
	}

	return splits;
}
