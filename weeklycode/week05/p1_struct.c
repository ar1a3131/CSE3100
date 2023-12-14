/* Accessing elements of a struct */

typedef struct Mono {
    int coef;
    int deg;
    struct Mono* next;
} Mono;

int main() {
    Mono* ptr = (Mono*)malloc(sizeof(Mono));

    ptr[0].coef = 1;
    (*ptr).coef = 1;
    ptr->coef = 1;
}
