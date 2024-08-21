#include <stdio.h>
#include <string.h>

#define max_str 10000000
#define max_pat 3000

char str[max_str];
char pat[max_pat];
int failure[max_pat];

int pmatch(char* str, char* pat, int *failure, FILE* fp);
void fail(char *pat, int *failure);

int main(void) {
	FILE* fp_s, * fp_p, * fp_r;
	int count = 0;

	fp_s = fopen("string.txt", "r");
	fp_p = fopen("pattern.txt", "r");

	if (fp_s == NULL) {
		printf("The string file does not exist.\n");
		return 1;
	}

	if (fp_p == NULL) {
		printf("The pattern file does not exist.\n");
		return 1;
	}

	fp_r = fopen("result_kmp.txt", "w");

	fgets(str, sizeof(str), fp_s);
	fgets(pat, sizeof(pat), fp_p);
	fprintf(fp_r, "          \n");

	count = pmatch(str, pat, failure, fp_r);

	fseek(fp_r, 0, SEEK_SET);
	fprintf(fp_r, "%d", count);

	fclose(fp_s);
	fclose(fp_p);
	fclose(fp_r);

	return 0;
}

int pmatch(char* str, char* pat, int *failure, FILE* fp) {
	int i = 0, j = 0, count = 0;
	int lens = strlen(str);
	int lenp = strlen(pat);
	
	fail(pat, failure);
	
	while (i < lens && j < lenp){
		if (str[i] == pat[j]) { 
			i++;
			if (j == lenp - 1) {
				count++;
				fprintf(fp, "%d ", i - lenp);
				j = failure[j] + 1;
			}
			else j++;
		}
		else if (j == 0) i++;
		else j = failure[j - 1] + 1;
	}
	return count;
}

void fail(char *pat, int *failure) {
	int i, n = strlen(pat);
	failure[0] = -1;
	for (int j = 1; j < n; j++) {
		i = failure[j - 1];
		while ((pat[j] != pat[i + 1]) && (i >= 0))
			i = failure[i];
		if (pat[j] == pat[i + 1])
			failure[j] = i + 1;
		else failure[j] = -1;
	}
}
