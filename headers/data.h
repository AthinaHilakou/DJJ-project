#include <stdio.h>
#include <stdlib.h>
#include <endian.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
// make sure to convert big/low endian to machine integer

// void *import_data_bin(char * filename, int data_size){

//     int fd = open("data.bin", O_RDONLY);
//     if (fd == -1) {
//         perror("open");
//         exit(1);
//     }

    
//     struct stat st;
//     if (fstat(fd, &st) == -1) {
//         perror("fstat");
//         exit(1);
//     }
//     int file_size = st.st_size;


//     void *data = mmap(NULL, data_size, PROT_READ, MAP_PRIVATE, fd, 0);
//     if (data == MAP_FAILED) {
//         perror("mmap");
//         exit(1);
//     }

//     // Convert the integers to host byte order
//     for(i = 0; i < file_size/data_size; i++){
//         data + i*data_size = be32toh(data->x);
//         data + i*data_size = be32toh(data->y);
//         data + i*data_size = be32toh(data->z);
//     }
//     // Don't forget to unmap the memory and close the file descriptor
//     munmap(data, sizeof(Data));
//     close(fd);

//     return 0;
// }

// void *get_data(int index){

//     return NULL;
// }
