function fibonacci(n: number): number {
    if(n <= 1) {
        return 1;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

ss_assert(fibonacci(10) == 89);
ss_assert(fibonacci(11) == 144);
ss_assert(fibonacci(12) == 233);
ss_assert(fibonacci(13) == 377);
ss_assert(fibonacci(14) == 610);
ss_assert(fibonacci(15) == 987);
ss_assert(fibonacci(16) == 1597);
ss_assert(fibonacci(17) == 2584);
ss_assert(fibonacci(18) == 4181);
ss_assert(fibonacci(19) == 6765);
ss_assert(fibonacci(20) == 10946);