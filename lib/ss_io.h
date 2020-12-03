#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "ss_string.h"

ss_string *ss_integer2string(long number);

ss_string *ss_float2string(double number);

long ss_string2integer(ss_string *str);

double ss_string2float(ss_string *str);

void ss_print_integer(long number);

void ss_println_integer(long number);

void ss_print_float(double number);

void ss_println_float(double number);

void ss_print_string(ss_string *str);

void ss_println_string(ss_string *str);