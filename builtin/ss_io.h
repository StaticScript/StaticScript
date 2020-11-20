#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "ss_base.h"
#include "ss_string.h"

ss_string *ss_integer2string(long number);

long ss_string2integer(ss_string *str);

void ss_print_integer(long number);

void ss_println_integer(long number);

void ss_print_string(ss_string *str);

void ss_println_string(ss_string *str);