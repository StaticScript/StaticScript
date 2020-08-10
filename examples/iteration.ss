let x = 1;
for(let i = 0; i < 10; i = i + 1) {
    x = x + i;
    if (x > 10) {
        break;
    }
}