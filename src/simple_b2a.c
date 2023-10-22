#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// Define a structure to store the data for each line
struct DataLine {
    float float1;
    float float2;
    float float3;
    float float4;
};

int main() {
    const char *file_path = "datasets/bin/5k.rect.normal.bin";
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // Get the file size
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return 1;
    }

    // Map the file into memory
    void *file_data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_data == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    // Calculate the number of lines based on the file size
    size_t num_lines = st.st_size / (sizeof(struct DataLine));

    // Process the data
    struct DataLine *data = (struct DataLine *)file_data;
    for (int i = 0; i < 10; i++) {
        printf("Line %d: Float1 = %.6f, Float2 = %.6f, Float3 = %.6f, Float4 = %.6f\n",
               i + 1, data[i].float1, data[i].float2, data[i].float3, data[i].float4);
    }

    // Unmap the file and close it
    munmap(file_data, st.st_size);
    close(fd);

    return 0;
}
