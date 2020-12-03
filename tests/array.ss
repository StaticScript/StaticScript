let arr1: number[];
let arr2: number[] = [];
let arr3: number[][];
let arr4: number[][] = [];

let arr5 = [1,2];
let arr6 = [[1,2],[3,4]];
let arr7 = [[],[1,2]];
let arr8 = [[[1],[2],[]],[[]]];

ss_assert(arr5[0] == 1);
ss_assert(arr5[1] == 2);
ss_assert(arr6[0][0] == 1);
ss_assert(arr6[0][1] == 2);
ss_assert(arr6[1][0] == 3);
ss_assert(arr6[1][1] == 4);
ss_assert(arr7[1][0] == 1);
ss_assert(arr7[1][1] == 2);
ss_assert(arr8[0][0][0] == 1);
ss_assert(arr8[0][1][0] == 2);

let arr: number[][][] = [[[1,2,3,4], [5,6,7,8], [9,10,11,12]], [[100,200,300,400], [201,202,203,204], [205,206,207,208]]];
for(let i = 0; i < 2; i++) {
    for(let j = 0; j < 3; j++) {
        for(let k = 0; k < 4; k++) {
            arr[i][j][k] += 1;
        }
    }
}

ss_assert(arr[0][0][0] == 2);
ss_assert(arr[0][0][1] == 3);
ss_assert(arr[0][0][2] == 4);
ss_assert(arr[0][0][3] == 5);
ss_assert(arr[0][1][0] == 6);
ss_assert(arr[0][1][1] == 7);
ss_assert(arr[0][1][2] == 8);
ss_assert(arr[0][1][3] == 9);
ss_assert(arr[0][2][0] == 10);
ss_assert(arr[0][2][1] == 11);
ss_assert(arr[0][2][2] == 12);
ss_assert(arr[0][2][3] == 13);
ss_assert(arr[1][0][0] == 101);
ss_assert(arr[1][0][1] == 201);
ss_assert(arr[1][0][2] == 301);
ss_assert(arr[1][0][3] == 401);
ss_assert(arr[1][1][0] == 202);
ss_assert(arr[1][1][1] == 203);
ss_assert(arr[1][1][2] == 204);
ss_assert(arr[1][1][3] == 205);
ss_assert(arr[1][2][0] == 206);
ss_assert(arr[1][2][1] == 207);
ss_assert(arr[1][2][2] == 208);
ss_assert(arr[1][2][3] == 209);


let floatArr: number[][] = [[1, 2.3], [1.2, 5]];

ss_assert(floatArr[0][0] == 1);
ss_assert(floatArr[0][1] == 2.3);
ss_assert(floatArr[1][0] == 1.2);
ss_assert(floatArr[1][1] == 5);