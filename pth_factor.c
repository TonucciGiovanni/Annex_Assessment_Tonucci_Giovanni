#include <stdio.h>
#include <math.h>

long pthFactor(long n, long p) {
    long count = 0;
    long i;
    long sqrtn = (long)sqrt(n);
	
    for (i = 1; i <= sqrtn; i++) {
        if (n % i == 0) {
            count++;
            if (count == p) return i; 
        }
    }
	
    for (i = sqrtn; i >= 1; i--) {
        if (n % i == 0) {
            long paired = n / i;
			
            if (paired == i) continue;

            count++;
            if (count == p) return paired;
        }
    }

    return 0;
}