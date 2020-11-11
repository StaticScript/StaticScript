#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "ss_base.h"
#include "ss_string.h"

ss_string *KEEPALIVE ss_integer2string(long number);

long KEEPALIVE ss_string2integer(ss_string *str);

void KEEPALIVE ss_print_integer(long number);

void KEEPALIVE ss_println_integer(long number);

void KEEPALIVE ss_print_string(ss_string *str);

void KEEPALIVE ss_println_string(ss_string *str);