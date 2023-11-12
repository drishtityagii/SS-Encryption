#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>

void gcd(int g, int a, int b);

void mod_inverse(int o, int a, int n);

void pow_mod(int o, int a, int d, int n);

bool is_prime(int n, uint64_t iters);

void make_prime(int p, uint64_t bits, uint64_t iters);

bool iseven(int a);

bool isodd(int a);
