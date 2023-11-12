#pragma once

#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>

void gcd(mpz_t g, const mpz_t a, const mpz_t b);

void lcm(mpz_t c, const mpz_t a, const mpz_t b);

bool iseven(mpz_t a);

bool isodd(mpz_t a);

void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n);

void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n);

bool is_prime(const mpz_t n, uint64_t iters);

void make_prime(mpz_t p, uint64_t bits, uint64_t iters);
