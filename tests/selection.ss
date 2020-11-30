let x: number = 1;
let y: number = 0;
if (x < y) {
    x = x + 1;
} else {
    y = y + 1;
}
ss_assert(x == 1);
ss_assert(y == 1);