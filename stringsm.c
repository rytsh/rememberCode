//#include <assert.h>
//#include <limits.h>
//#include <math.h>
#include <stdbool.h>
//#include <stddef.h>
//#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();

// Complete the stringSimilarity function below.
unsigned long int stringSimilarity(char* s) {
	int n=strlen(s);
	int * z = (int *) malloc((n+1) * sizeof(int));
	int L = 0, R = 0;
	for (int i = 1; i < n; i++) {
		if (i > R) {
			L = R = i;
			while (R < n && s[R-L] == s[R]) R++;
			z[i] = R-L; R--;
		}
		else {
			int k = i-L;
			if (z[k] < R-i+1) z[i] = z[k];
			else {
				L = i;
				while (R < n && s[R-L] == s[R]) R++;
				z[i] = R-L; R--;
			}
		}
	}
	unsigned long int ans=0;
	for(int i=1;i<n;i++) ans+=z[i];

	free(z);
	return ans + n;
}

int main()
{
	FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

	char* t_endptr;
	char* t_str = readline();
	int t = strtol(t_str, &t_endptr, 10);

	if (t_endptr == t_str || *t_endptr != '\0') { exit(EXIT_FAILURE); }

	for (int t_itr = 0; t_itr < t; t_itr++) {
		char* s = readline();

		unsigned long int result = stringSimilarity(s);

		fprintf(fptr, "%lu\n", result);
		//printf("%lu\n", result);
		free(s);
	}

	fclose(fptr);

	return 0;
}

char* readline() {
	size_t alloc_length = 100001;
	size_t data_length;
	char* data = malloc(alloc_length);
	scanf("%[^\n]s",data);
	//fgets(data,alloc_length,stdin);
	getchar();
	data_length = strlen(data) + 1;

	data = realloc(data, data_length+1);
	data[data_length] = '\0';

	return data;
}
