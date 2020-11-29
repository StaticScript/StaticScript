let arr1: number[];
let arr2: number[] = [];
let arr3: number[][];
let arr4: number[][] = [];

let arr5 = [1,2];
let arr6 = [[1,2],[3,4]];
let arr7 = [[],[1,2]];
let arr8 = [[[0],[0],[]],[[]]];

let arr: number[][][] = [[[1,2], [1,2,3,4], [5,6,7]]];
++arr[0][1][3];
let ele = arr[0][1][3];
ss_println_integer(ele);

arr[0][0][0] += arr[0][2][2];
ss_println_integer(arr[0][0][0]);