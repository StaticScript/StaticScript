<h1 align="center">StaticScript</h1>

<div align="center">

StaticScript is a statically typed programming language, syntactically like TypeScript.

![Github Workflow Status](https://img.shields.io/github/workflow/status/StaticScript/StaticScript/Build?style=flat-square)
![Platform](https://img.shields.io/badge/platform-linux--64%20%7C%20macos--64-brightgreen?style=flat-square)
![License](https://img.shields.io/github/license/StaticScript/StaticScript?style=flat-square)

![GitHub Repo stars](https://img.shields.io/github/stars/StaticScript/StaticScript?style=flat-square&color=brightgreen)
![GitHub forks](https://img.shields.io/github/forks/StaticScript/StaticScript?style=flat-square&color=brightgreen)
![GitHub issues](https://img.shields.io/github/issues-raw/StaticScript/StaticScript?style=flat-square)
![GitHub pull requests](https://img.shields.io/github/issues-pr-raw/StaticScript/StaticScript?style=flat-square)

![GitHub Repository Size](https://img.shields.io/github/repo-size/StaticScript/StaticScript?style=flat-square&color=brightgreen)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/StaticScript/StaticScript?style=flat-square&color=brightgreen)
![GitHub top language](https://img.shields.io/github/languages/top/StaticScript/StaticScript?style=flat-square&color=brightgreen)

English | [简体中文](./README-zh_CN.md)

</div>

## Install

### Install on Ubuntu

```shell
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/StaticScript/StaticScript/master/install-ubuntu.sh)"
```
Or
```shell
wget https://raw.githubusercontent.com/StaticScript/StaticScript/master/install-ubuntu.sh
sudo chmod +x install-ubuntu.sh
sudo /bin/bash install-ubuntu.sh
```

> For other linux distributions, you may need to modify the installation script to install properly.
> 
> The installation script may request administrator privileges.

### Install on macOS

```shell
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/StaticScript/StaticScript/master/install-macos.sh)"
```
Or
```shell
wget https://raw.githubusercontent.com/StaticScript/StaticScript/master/install-macos.sh
sudo chmod +x install-macos.sh
sudo /bin/bash install-macos.sh
```
> The installation script may request administrator privileges.

### Install on Windows

**_Temporarily not supported_**


## Usage

First you need to write a legal StaticScript code file as following.
```typescript
// test.ss

let content: string = "Hello World";

ss_println_string(content);
```

Then execute the following command from the command line.
```shell
staticscript test.ss -o test
./test
```

## Language Features Summary

### Variable and Constant Declaration

Here are some variable declarations.

```typescript
let flag: boolean = true;
let count: number = 20;
let content: string = "Hello World";
```

Thanks to the type inference feature of StaticScript, we can write the above variable declaration as follows. They are exactly equivalent.

```typescript
let flag = true;
let count = 20;
let content = "Hello World";
```

The compiler of StaticScript cleverly deduced the type of the variable from the initial value.

In addition to using `let` to declare variables, you can also use `const` to declare constants.

```typescript
const name = "StaticScript";
const age = 1;
const developing = true;
```

The difference between `let` and `const` is that constants declared with `const` cannot be modified.

### Variable Evaluation

You can use a wealth of operators to perform operations on variables, including arithmetic operations, bitwise operations, logical operations, relational operations, assignments, and string concatenation.

```typescript
let a = 1;
let b = 2;

// add, subtract, multiply and divide
let sum = a + b;
let diff = a - b;
let product = a * b;
let quotient = a / b;

a = a << 1; // equivalent to `a <<= 1`
b = b >> 1; // equivalent to `b >>= 1`

let year = "2020", month = "08", day = "06";
let birthday = year + "/" + month + "/" + day;
```

### Control Flow

```typescript
let a = 1;
let b = 100;
if (a < b) {
    ss_println_string("b is bigger");
} else {
    ss_println_string("b is not bigger");
}


let max = a;
if (a < b) {
    max = b;
}
```

### Loops
StaticScript supports using `while` statement and `for` statement to do some repetitive things.

```typescript
// Calculate the sum of all even numbers between [1, 100]
let sum1 = 0;
let i = 1;
while(i <= 100) {
    if (i % 2 == 0) {
        sum1 += i;
    }
}


// Calculate the sum of all integers between [1, 100]
let sum2 = 0;
for(let i = 1; i <= 100; i++) {
    sum2 += i;
}
```

### Function

StaticScript supports defining functions in the top level scope and using function in any scope.

```typescript
function add(a: number, b: number): number {
    return a + b;
}
```

The above function can omit the return type because StaticScript can deduce the return type through the expression of the return statement.

It is important to note that the parameter types of functions must be explicitly declared.
