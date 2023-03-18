#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    int n = min(atoi(argv[1]), 1);

    FILE *test_cases = fopen("QM/in.txt", "w");
    srand(time(0));

    for (int i = 0; i < n; i++) {
        int num_terms = rand() % 10 + 1;
        for (int j = 0; j < num_terms; j++) {
            int num_vars = rand() % 10 + 1;
            for (int k = 0; k < num_vars; k++) {
                char var_name = 'a' + rand() % 10;
                fprintf(test_cases, "%c", var_name);
                if (rand() & 1) {
                    fprintf(test_cases, "\'");
                }
            }
            if (j < num_terms - 1) {
                fprintf(test_cases, " + ");
            }
        }
        fprintf(test_cases, "\n");
    }

    return 0;
}

