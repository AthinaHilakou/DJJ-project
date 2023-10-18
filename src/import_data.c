#include <stdio.h>

int main() {
    FILE *fp;
    char filename[] = "data.txt";
    char line[100];
    float a, b, c, d;
    int line_num = 1;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        return 1;
    }

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%f %f %f %f", &a, &b, &c, &d);
        printf("Line %d: %.2f %.2f %.2f %.2f\n", line_num, a, b, c, d);
        line_num++;
    }

    fclose(fp);
    return 0;
}
