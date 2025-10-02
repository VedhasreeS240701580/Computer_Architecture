#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define N 32

int add(int a, int b);
int subtract(int a, int b);
long long sequential_multiply(int a, int b);
long long bit_multiply(int a, int b);
long long booths_multiply(int a, int b);
long long bit_pair_multiply(int a, int b);
void division_restoring(int a, int b);
void division_non_restoring(int a, int b);

// Addition
int add(int a, int b) {
    return a + b;
}

// Subtraction
int subtract(int a, int b) {
    return a - b;
}

// Sequential Multiplication
long long sequential_multiply(int a, int b) {
    long long result = 0;
    int positive = 1;
    if (a < 0) { a = -a; positive = -positive; }
    if (b < 0) { b = -b; positive = -positive; }
    for (int i = 0; i < abs(b); i++) {
        result += a;
    }
    return result * positive;
}

// Bit Multiplication
long long bit_multiply(int a, int b) {
    long long result = 0;
    int positive = 1;
    if(a < 0) positive = -positive;
    if(b < 0) positive = -positive;
    long long abs_a = labs(a);
    unsigned int abs_b = abs(b);
    while(abs_b > 0) {
        if(abs_b & 1) result += abs_a;
        abs_a <<= 1;
        abs_b >>= 1;
    }
    return result * positive;
}

// Booth's Multiplication
long long booths_multiply(int multiplicand, int multiplier) {
    long long M = (long long)multiplicand;
    long long Q = (long long)multiplier;
    long long A = 0;
    int Q_minus_1 = 0;
    int N_bits = 32;
    for (int i = 0; i < N_bits; i++) {
        int last_two_bits = ((Q & 1) << 1) | Q_minus_1;
        if (last_two_bits == 0b01) A += M;
        else if (last_two_bits == 0b10) A -= M;
        Q_minus_1 = Q & 1;
        long long combined = (A << N_bits) | (Q & 0xFFFFFFFF);
        combined >>= 1;
        A = combined >> N_bits;
        Q = combined & 0xFFFFFFFF;
    }
    return (A << N_bits) | Q;
}

// Bit-Pair Multiplication
long long bit_pair_multiply(int multiplicand, int multiplier) {
    long long result = 0;
    int sign = 1;
    if ((multiplicand < 0) ^ (multiplier < 0)) 
        sign = -1;
    unsigned int M = abs(multiplicand);
    unsigned int Q = abs(multiplier);
    while (Q > 0) {
        int last_two_bits = Q & 3;
        switch (last_two_bits) {
            case 0:
                break;
            case 1:
                result += M;
                break;
            case 2:
                result += (M << 1);
                break;
            case 3:
                result += M + (M << 1);
                break;
        }
        Q >>= 2;
        M <<= 2;
    }
    return sign * result;
}

// Division with Restoring Method
void division_restoring(int dividend, int divisor) {
    if (divisor == 0) {
        printf("Division (Restoring): Division by zero not allowed\n");
        return;
    }
    int sign = ((dividend < 0) ^ (divisor < 0)) ? -1 : 1;
    unsigned int u_dividend = (unsigned int)abs(dividend);
    unsigned int u_divisor  = (unsigned int)abs(divisor);
    unsigned int quotient = 0;
    unsigned long long A = 0ULL;
    for (int i = N - 1; i >= 0; --i) {
        A = (A << 1) | (unsigned long long)((u_dividend >> i) & 1u);
        if (A >= u_divisor) {
            A -= u_divisor;
            quotient |= (1u << i);
        }
    }
    int q = (int)quotient * sign;
    int r = (int)A;
    if (dividend < 0) r = -r;
    printf("Division (Restoring): Quotient = %d, Remainder = %d\n", q, r);
}

// Division with Non-Restoring Method
void division_non_restoring(int dividend, int divisor) {
    if (divisor == 0) {
        printf("Division (Non-Restoring): Division by zero not allowed\n");
        return;
    }
    int overall_sign = ((dividend < 0) ^ (divisor < 0)) ? -1 : 1;
    unsigned int abs_Q = (unsigned int)abs(dividend);
    unsigned int abs_M = (unsigned int)abs(divisor);
    long long A = 0LL;
    unsigned int Q = abs_Q;
    long long M = (long long)abs_M;
    for (int i = 0; i < N; ++i) {
        A = (A << 1) | (long long)((Q >> (N - 1)) & 1u);
        Q <<= 1;
        if (A >= 0) A -= M;
        else        A += M;
        if (A >= 0) Q |= 1u;
    }
    if (A < 0) A += M;
    int q_final = (int)Q * overall_sign;
    int r_final = (int)A;
    if (dividend < 0 && r_final != 0) r_final = -r_final;
    printf("Division (Non-Restoring): Quotient = %d, Remainder = %d\n", q_final, r_final);
}

int main() {
    int a, b;
    printf("Enter first number: ");
    scanf("%d", &a);
    printf("Enter second number: ");
    scanf("%d", &b);
    printf("Addition: %d\n", add(a, b));
    printf("Subtraction: %d\n", subtract(a, b));
    printf("Sequential Multiplication: %lld\n", sequential_multiply(a, b));
    printf("Bit Multiplication: %lld\n", bit_multiply(a, b));
    printf("Booth's Multiplication: %lld\n", booths_multiply(a, b));
    printf("Bit-Pair Multiplication: %lld\n", bit_pair_multiply(a, b));
    division_restoring(a, b);
    division_non_restoring(a, b);
    return 0;
}