#ifndef LQR_FILE_OPERATIONS_H
#define LQR_FILE_OPERATIONS_H

typedef unsigned char byte;

namespace file_operations {
    void readFile(char *file_name, byte **buffer, int buffer_size);
    void writeFile(char *file_name, byte *buffer, int buffer_size);
};


#endif //LQR_FILE_OPERATIONS_H
