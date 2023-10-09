#include "string_vector.h"
#include <stdlib.h>

typedef struct ____string_vector_t {
    char* message;    // Message pointer
    size_t size;      // Current message size
    size_t max_size;  // Maximum allocated
} string_vector_t;

void Vector_Init(string_vector_t* V) {
    char* message = (char*)malloc((VECTORSIZE + 1) * sizeof(char));
    if (message == NULL) {
        perror("malloc");
        V->message = NULL;
        V->size = 0;
        V->max_size = 0;
    } else {
        V->message = message;
        V->message[0] = '\0';
        V->size = 0;
        V->max_size = VECTORSIZE;
    }
}

int Vector_Resize(string_vector_t* V) {
    char* ptr = (char*)realloc(V->message, V->max_size * 2);
    if (ptr == NULL) {
        perror("realloc");
        return -1;
    }
    V->message = ptr;
    V->max_size *= 2;
    return 0;
}

int Vector_Append(string_vector_t* V, char* message) {
    while (V->size + strlen(message) > V->max_size) {
        int status = Vector_Resize(V);
        if (status != 0) {
            return status;
        }
    }
    V->size = V->size + strlen(message);
    strcat(V->message, message);
    return 0;
}

char* Vector_Flush(string_vector_t* V) {
    char* tmp = (char*)malloc(sizeof(char) * (V->size + 1));
    memmove(tmp, V->message, V->size + 1);
    V->message[0] = '\0';
    V->size = 0;
    return tmp;
}

void Vector_Free(string_vector_t* V) {
    free(V->message);
}