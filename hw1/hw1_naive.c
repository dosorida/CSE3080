#include <stdio.h>
#include <string.h>

#define max_str 10000000
#define max_pat 3000

char str[max_str];
char pat[max_pat];

int pmatch(char *str, char *pat, FILE *fp);

int main(void) {
	FILE *fp_s, *fp_p, *fp_r;
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

	fp_r = fopen("result_naive.txt", "w");

	fgets(str, sizeof(str), fp_s);
	fgets(pat, sizeof(pat), fp_p);
	fprintf(fp_r, "          \n");

	count = pmatch(str, pat, fp_r);

	fseek(fp_r, 0, SEEK_SET);
	fprintf(fp_r, "%d", count);

	fclose(fp_s);
	fclose(fp_p);
	fclose(fp_r);

	return 0;
}

int pmatch(char *str, char *pat, FILE *fp) {
	int i, j, start = 0;
	int lasts = strlen(str) - 1;
	int lastp = strlen(pat) - 1;
	int endmatch = lastp;
	int count = 0;

	for (i = 0; endmatch <= lasts; endmatch++, start++) {
		j = 0; i = start;
		if (str[endmatch] == pat[lastp]) {
			for (j, i; j < lastp && str[i] == pat[j]; i++, j++);
		}
		if (j == lastp) {
			count++;
			fprintf(fp, "%d ", start);
		}	
	}
	return count;
}
