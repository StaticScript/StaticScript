function func(a: number, b: number): number {
    let x = a + 1;
    let y = b - 1;
    if (x < y) {
        let temp = x;
        x = y;
        y = temp;
    }
    for(let i = 0; i < 100; i = i + 1) {
        y = y + i - x;
        if (i > 50) {
            break;
        }
    }
    return x + y;
}

add(1, 2);