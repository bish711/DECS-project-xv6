#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "tempfs.h" 

int main(int argc, char *argv[]) {
    // Initialize tempfs
    tempfsinit();
    printf(1, "Tempfs initialized successfully.\n");

    // Try to create a file
    const char *filename = "testfile.txt";
    int res = tempfscreate(filename);

    if (res == 0) {i
        printf(1, "File '%s' created successfully.\n", filename);
    } else {
        printf(1, "Failed to create file '%s'.\n", filename);
    }

    exit();
}

