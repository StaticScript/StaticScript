<h1 align="center">StaticScript</h1>

<div align="center">

StaticScript是一门类TypeScript的静态强类型编程语言

![Github Workflow Status](https://img.shields.io/github/workflow/status/StaticScript/StaticScript/Build?style=for-the-badge)
![Platform](https://img.shields.io/badge/platform-linux--64%20%7C%20macos--64-brightgreen?style=for-the-badge)
![License](https://img.shields.io/github/license/StaticScript/StaticScript?style=for-the-badge)

![GitHub Repo stars](https://img.shields.io/github/stars/StaticScript/StaticScript?style=for-the-badge&color=brightgreen)
![GitHub forks](https://img.shields.io/github/forks/StaticScript/StaticScript?style=for-the-badge&color=brightgreen)
![GitHub issues](https://img.shields.io/github/issues-raw/StaticScript/StaticScript?style=for-the-badge)
![GitHub pull requests](https://img.shields.io/github/issues-pr-raw/StaticScript/StaticScript?style=for-the-badge)

![GitHub Repository Size](https://img.shields.io/github/repo-size/StaticScript/StaticScript?style=for-the-badge&color=brightgreen)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/StaticScript/StaticScript?style=for-the-badge&color=brightgreen)
![GitHub top language](https://img.shields.io/github/languages/top/StaticScript/StaticScript?style=for-the-badge&color=brightgreen)

[English](./README.md) | 简体中文

</div>


## 语言特性概要

### 变量和常量的声明

下面是一些变量声明

```typescript
let flag: boolean = true;
let count: number = 20;
let content: string = "Hello World";
```

得益于StaticScript的类型推导特性, 我们可以把上面的变量声明写成下面这样, 它们是等效的

```typescript
let flag = true;
let count = 20;
let content = "Hello World";
```

StaticScript的编译器可以巧妙地从变量的初始值推导出变量的类型

除了使用`let`声明变量外，还可以使用`const`声明常量

```typescript
const name = "StaticScript";
const age = 1;
const developing = true;
```

`let`和`const`的区别在于`const`声明的常量不能被修改

### 变量运算

可以使用多种多样的运算符对变量执行操作，包括算术运算、按位运算、逻辑运算、关系运算、赋值和字符串连接

```typescript
let a = 1;
let b = 2;

// 加减乘除
let sum = a + b;
let diff = a - b;
let product = a * b;
let quotient = a / b;

a = a << 1; // 等效于 `a <<= 1`
b = b >> 1; // 等效于 `b >>= 1`

let year = "2020", month = "08", day = "06";
let birthday = year + "/" + month + "/" + day;
```

### 控制流

```typescript
let a = 1;
let b = 100;
if (a < b) {
    ss_println_string("b更大");
} else {
    ss_println_string("b不比a大");
}


let max = a;
if (a < b) {
    max = b;
}
```

### 循环

StaticScript支持使用`while`语句和`for`语句执行一些重复的操作

```typescript
// 计算[1, 100]间所有偶数的和
let sum1 = 0;
let i = 1;
while(i <= 100) {
    if (i % 2 == 0) {
        sum1 += i;
    }
}


// 计算[1, 100]间所有整数的和
let sum2 = 0;
for(let i = 1; i <= 100; i++) {
    sum2 += i;
}
```

### 函数

StaticScript支持在顶级范围内定义函数并在任何作用域内使用函数

```typescript
function add(a: number, b: number): number {
    return a + b;
}
```

StaticScript可以通过return语句的表达式来推断返回类型, 因此上面的函数可以省略返回类型

需要注意的是，函数的参数类型必须显式声明, 不能省略