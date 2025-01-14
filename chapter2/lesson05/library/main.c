#include"head.h"

int main() {
    int a, b;
    a = 12;
    b = 2;
    printf("%d + %d = %d\n", a, b, add(a, b));
    printf("%d - %d = %d\n", a, b, sub(a, b));
    printf("%d * %d = %d\n", a, b, mul(a, b));
    printf("%d / %d = %.2lf\n", a, b, div(a, b));
    return 0;
}