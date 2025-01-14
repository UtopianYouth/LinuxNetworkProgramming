#include"head.h"

int main() {
    int a, b;
    printf("%d + %d = %d", a, b, add(a, b));
    printf("%d - %d = %d", a, b, sub(a, b));
    printf("%d * %d = %d", a, b, mul(a, b));
    printf("%d / %d = %.lf", a, b, div(a, b));
    return 0;
}