#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h" //fix name when u change it to utilize gmp

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    mpz_t tmp;
    mpz_init(tmp); //temp variable
    mpz_t tmp1;
    mpz_init(tmp1); //temp var for b since b is const
    mpz_set(tmp1, b);
    mpz_t tmp2;
    mpz_init(tmp2);
    mpz_set(tmp2, a); //tmp var for a since a is const
    while (mpz_cmp_ui(tmp1, 0) != 0) {
        mpz_set(tmp, tmp1); //setting tmp to b
        mpz_mod(tmp1, tmp2, tmp1); //b = a % b
        mpz_set(tmp2, tmp); //a = tmp
        mpz_set(g, tmp2); //g(the divisor) = tmp
    }
    mpz_clear(tmp);
    mpz_clear(tmp1);
    mpz_clear(tmp2); //clearing all mpz_t variables
}
void lcm(mpz_t c, const mpz_t a, const mpz_t b) {
    mpz_t tmp;
    mpz_init(tmp);
    gcd(tmp, a, b);
    mpz_mul(c, a, b);
    mpz_divexact(c, c, tmp); //mpz_t c = (a*b)/gcd(a,b);
    mpz_clear(tmp);
}
bool iseven(mpz_t a) {
    return mpz_even_p(
        a); 
}

bool isodd(mpz_t a) {
    return mpz_odd_p(a); //same as even
}
void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    mpz_t v;
    mpz_t p;
    mpz_t exp;
    mpz_init(v);
    mpz_set_ui(v, 1);
    mpz_init(p);
    mpz_set(p, a); //initializing p to 1
    mpz_init(exp);
    mpz_set(exp, d); //setting var for exponent that can be changed
    while (mpz_cmp_ui(exp, 0) > 0) {
        if (isodd(exp)) {
            mpz_mul(v, v, p);
            mpz_mod(v, v, n); //v = v*p%n
        }
        mpz_mul(p, p, p);
        mpz_mod(p, p, n); //p = (p*p) % modulus;
        mpz_div_ui(exp, exp, 2); //exponent = (exponent)/2;
    }
    mpz_set(o, v);
    mpz_clear(v);
    mpz_clear(p);
    mpz_clear(exp); //clearing mpz_t vars
}
void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    mpz_t r;
    mpz_init(r);
    mpz_set(r, n); //int r = n;
    mpz_t r1;
    mpz_init(r1);
    mpz_set(r1, n);
    mpz_t r_prime;
    mpz_init(r_prime);
    mpz_set(r_prime, a); //int r_prime = a;
    mpz_t t;
    mpz_init(t);
    mpz_set_ui(t, 0); //int t = 0;
    mpz_t t1;
    mpz_init(t1);
    mpz_set_ui(t1, 0);
    mpz_t t_prime;
    mpz_init(t_prime);
    mpz_set_ui(t_prime, 1); //int t_prime = 1;
    mpz_t q;
    mpz_init(q);
    mpz_t qtr;
    mpz_init(qtr);
    mpz_t qtt;
    mpz_init(qtt);
    while (mpz_cmp_ui(r_prime, 0) != 0) { //while (r_prime != 0)
        mpz_fdiv_q(q, r, r_prime);
        mpz_set(r, r_prime); //r = r_prime;
        mpz_mul(qtr, q, r_prime); //qtr = q*pr
        mpz_sub(r_prime, r1, qtr); //r_prime = r-q*r_prime;
        mpz_set(r1, r);
        mpz_set(t, t_prime); //t = t_prime;
        mpz_mul(qtt, q, t_prime);
        mpz_sub(t_prime, t1, qtt); //t_prime = t-q*t_prime;
        mpz_set(t1, t);
    }
    if (mpz_cmp_ui(r, 1) > 0) { //if (r>1)
        mpz_set_ui(o, 0);
        mpz_clear(r);
        mpz_clear(r1);
        mpz_clear(r_prime);
        mpz_clear(t);
        mpz_clear(t1);
        mpz_clear(t_prime);
        mpz_clear(q);
        mpz_clear(qtr);
        mpz_clear(qtt);
        return;
    }
    if (mpz_cmp_ui(t, 0) < 0) { //if (t<0)
        mpz_add(t, t, n); //t = t+n;
    }
    mpz_set(o, t);
    mpz_clear(r);
    mpz_clear(r1);
    mpz_clear(r_prime);
    mpz_clear(t);
    mpz_clear(t1);
    mpz_clear(t_prime);
    mpz_clear(q);
    mpz_clear(qtr);
    mpz_clear(qtt);
}
bool is_prime(const mpz_t n, uint64_t iters) {
    if ((mpz_cmp_ui(n, 2) == 0) || (mpz_cmp_ui(n, 3) == 0)) { //if (n==3) || (n ==2))
        return true;
    }
    if ((mpz_cmp_ui(n, 0) == 0) || (mpz_cmp_ui(n, 1) == 0)) {
        return false;
    }
    mpz_t r;
    mpz_init(r);
    mpz_t s;
    mpz_init(s);
    mpz_set_ui(s, 0);
    mpz_t a;
    mpz_init(a);
    mpz_t y;
    mpz_init(y);
    mpz_sub_ui(r, n, 1); //int r = n - 1;
    mpz_t n_minus;
    mpz_init(n_minus);
    mpz_sub_ui(n_minus, n, 1); //n_minus = n-1
    mpz_t s_minus;
    mpz_init(s_minus);
    mpz_sub_ui(s_minus, s, 1);
    mpz_t two;
    mpz_init(two);
    mpz_set_ui(two, 2);
    mpz_t j;
    mpz_init(j);
    while (iseven(r)) { //while ((r%2) == 0)
        mpz_add_ui(s, s, 1); //++s;
        mpz_div_ui(r, r, 2);
    }
    for (uint64_t i = 1; i < iters; ++i) {
        mpz_urandomm(a, state, n); //choose random a from 2 through n-2
        pow_mod(y, a, r, n); //y = pow_mod(a, r, n);
        if ((mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, n_minus) != 0)) { //if ((y!=1) && (y != (n-1)))
            mpz_set_ui(j, 1);
            while ((mpz_cmp(j, s_minus) <= 0)
                   && ((mpz_cmp(y, n_minus) != 0))) { //while ((j <= (s-1)) && (y != (n-1)))
                pow_mod(y, y, two, n); //y = power_mod(y, 2, n)
                if (mpz_cmp_ui(y, 1) == 0) { //if (y == 1)
                    mpz_clear(r);
                    mpz_clear(s);
                    mpz_clear(a);
                    mpz_clear(y);
                    mpz_clear(n_minus);
                    mpz_clear(s_minus);
                    mpz_clear(two);
                    mpz_clear(j);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, n_minus) != 0) { //if (y!=(n-1))
                mpz_clear(r);
                mpz_clear(s);
                mpz_clear(a);
                mpz_clear(y);
                mpz_clear(n_minus);
                mpz_clear(s_minus);
                mpz_clear(two);
                mpz_clear(j);
                return false;
            }
        }
    }
    mpz_clear(r);
    mpz_clear(s);
    mpz_clear(a);
    mpz_clear(y);
    mpz_clear(s_minus);
    mpz_clear(n_minus);
    mpz_clear(two);
    mpz_clear(j);
    return true;
}
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_urandomb(p, state, bits); //generating random num
    mpz_t more_bits;
    mpz_init(more_bits);
    mpz_ui_pow_ui(more_bits, 4, bits); //adds 2^bits in case num isn't long enough
    mpz_add(p, p, more_bits);
    while (is_prime(p, iters) == false) {
        mpz_urandomb(p, state, bits);
        mpz_add(p, p, more_bits); //generates num until it is prime
    }
    mpz_clear(more_bits);
}
