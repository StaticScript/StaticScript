function process(n: int): int {
    if (n < 100) {
        if (n < 30) {
            return 1;
        } else if (n < 60) {
            return 2;
        } else {
            return 3;
        }
    } else if (n < 200) {
        if (n < 130) {
            return 4;
        } else if (n < 160) {
            return 5;
        } else {
            return 6;
        }
    } else if (n < 300) {
        if (n < 230) {
            return 7;
        } else if (n < 260) {
            return 8;
        } else {
            return 9;
        }
    } else {
        if (n < 330) {
            return 10;
        } else if (n < 360) {
            return 11;
        } else {
            return 12;
        }
    }
}

ss_assert(process(20) == 1);
ss_assert(process(50) == 2);
ss_assert(process(70) == 3);
ss_assert(process(120) == 4);
ss_assert(process(150) == 5);
ss_assert(process(170) == 6);
ss_assert(process(220) == 7);
ss_assert(process(250) == 8);
ss_assert(process(270) == 9);
ss_assert(process(320) == 10);
ss_assert(process(350) == 11);
ss_assert(process(370) == 12);