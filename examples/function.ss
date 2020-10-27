function nop() {
}

function nopRet() {
    return;
}

function add(a: number, b: number): number {
    return a + b;
}

function max(a: number, b: number): number {
    if (a < b) {
        return b;
    }
    return a;
}

function lessThan(a: number, b: number): boolean {
    return a < b;
}

nop();
nopRet();
add(1, 2);
max(1, 2);
lessThan(1, 2);
