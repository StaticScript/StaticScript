let a1 = 1;
let a2 = 2.0;
let a3 = a1 + a2;

ss_assert(a3 == 3);
ss_assert(a3 == 3.0);

// =-----------------------------------------------=

let b1 = 1;
let b2: number = b1;

ss_assert(b2 == 1.0);

let b3 = 1.2;
let b4: int = b3;

ss_assert(b4 == 1);

// =-----------------------------------------------=

let c1 = 1;

function addMore(n: number) {
    return n + 1.2;
}

ss_assert(addMore(c1) == 2.2);

let c3 = 1.234;

function floor(n: int): int {
    return n;
}

ss_assert(floor(c3) == 1);

// =-----------------------------------------------=

let arr1: number[] = [1, 2, 3, 4, 5];
for(let i = 0; i < 5; i++) {
    arr1[i] += 1.2;
}
ss_assert(arr1[0] == 2.2);
ss_assert(arr1[1] == 3.2);
ss_assert(arr1[2] == 4.2);
ss_assert(arr1[3] == 5.2);
ss_assert(arr1[4] == 6.2);

let arr2: number[] = [2.2, 4.4, 8.8, 16.16, 32.32];

for(let j = 0; j < 5; j++) {
    arr2[j] += 8;
}

ss_assert(arr2[0] == 10.2);
ss_assert(arr2[1] == 12.4);
ss_assert(arr2[2] == 16.8);
ss_assert(arr2[3] == 24.16);
ss_assert(arr2[4] == 40.32);
