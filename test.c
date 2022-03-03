#include <stdio.h>

void ymp(int *h) {
	*h = 12;
}

int main() {
	int f = 2;
	ymp(&f);
	printf("%d\n", f);
}
