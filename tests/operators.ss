let n_1 = 1;
let n_2 = n_1;
let n_3 = 23 + --n_1;
ss_assert(n_1 == 0);
ss_assert(n_2 == 1);
ss_assert(n_3 == 23);

let n_4 = n_1 - ++n_2;
ss_assert(n_2 == 2);
ss_assert(n_4 == -2);

let n_5 = n_2 - -1;
ss_assert(n_5 == 3);

let b = false;
ss_assert(!b);

let n_6 = ~100;
ss_assert(n_6 == -101);

let n_7 = 15 % 4;
ss_assert(n_7 == 3);

let n_8 = 1 << 2;
ss_assert(n_8 == 4);

let n_9 = 4 >> 2;
ss_assert(n_9 == 1);

ss_assert((99 & 100) == 96);
ss_assert((99 | 100) == 103);
ss_assert((99 ^ 100) == 7);

ss_assert(true && true);
ss_assert(!(true && false));
ss_assert(!(false && true));
ss_assert(!(false && false));

let n_10 = 10;
n_10 += 100;
ss_assert(n_10 == 110);

let n_11 = 102;
n_11 -= 90;
ss_assert(n_11 == 12);

let n_12 = 4;
n_12 *= 8;
ss_assert(n_12 == 32);

let n_13 = 3;
n_13 /= 2;
ss_assert(n_13 == 1);

let n_14 = 9;
n_14 %= 2;
ss_assert(n_14 == 1);

let n_15 = 9;
n_15 <<= 2;
ss_assert(n_15 == 36);

let n_16 = 36;
n_16 >>= 2;
ss_assert(n_16 == 9);

let n_17 = 9;
n_17 &= 2;
ss_assert(n_17 == 0);

let n_18 = 9;
n_18 ^= 2;
ss_assert(n_18 == 11);

let n_19 = 9;
n_19 |= 2;
ss_assert(n_19 == 11);

let b_2 = true;
b_2 &&= false;
ss_assert(!b_2);

let b_3 = true;
b_3 ||= false;
ss_assert(b_3);