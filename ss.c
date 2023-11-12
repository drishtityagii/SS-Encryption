#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gmp.h>
#include "ss.h"
#include "numtheory.h"
#include "randstate.h"
void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    mpz_t q_minus;
    mpz_init(q_minus);
    mpz_t p_minus;
    mpz_init(p_minus);
    uint64_t lower = nbits / 5; //set lower limit for range random num of bits can be chosen from
    uint64_t upper
        = (2 * nbits) / 5; //set upper limit for range random num of bits can be chosen from
    uint64_t bits = (rand() % (upper - lower + 1)) + lower; //choose random num of bits
    make_prime(p, bits, iters); //int q = make_prime(q, iters);//make prime num q
    make_prime(q, bits, iters); //int q = make_prime(q, iters);//make prime num q
    mpz_sub_ui(q_minus, q, 1);
    mpz_sub_ui(p_minus, p, 1);
    while ((mpz_divisible_p(p, q_minus) != 0) || (mpz_divisible_p(q, p_minus) != 0)) {
        make_prime(p, bits, iters);
        make_prime(q, bits, iters);
    }
    mpz_mul(n, p, p);
    mpz_mul(n, n, q); //n = (p*p)*q; //calculate n
    mpz_clear(q_minus);
    mpz_clear(p_minus);
}
void ss_write_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n); //gmp_fprintf prints n as a hexstring
    fprintf(pbfile, "%s\n", username); //fprintf to print username as a string
}
void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n); //use gmp_fscanf to read hexstring n
    fscanf(pbfile, "%s\n", username); //use fscanf to read username
}
void ss_make_priv(mpz_t d, mpz_t pq, mpz_t p, mpz_t q) {
    mpz_t n;
    mpz_init(n);
    mpz_mul(n, p, p);
    mpz_mul(n, n, q); //int n = (p*p)*q; //calculating n
    mpz_mul(pq, p, q); //int pq = p*q; //calculating pq
    mpz_t p_minus;
    mpz_init(p_minus);
    mpz_sub_ui(p_minus, p, 1);
    mpz_t q_minus;
    mpz_init(q_minus);
    mpz_sub_ui(q_minus, q, 1);
    mpz_t lambda;
    mpz_init(lambda);
    lcm(lambda, p_minus, q_minus); //int lambda = lcm(p-1, q-1); //calculating lambda
    mod_inverse(d, n, lambda); //mod inverse using public key to find priv
    mpz_clear(n);
    mpz_clear(p_minus);
    mpz_clear(q_minus);
    mpz_clear(lambda);
}
void ss_write_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", pq);
    gmp_fprintf(pvfile, "%Zx\n", d); //writes pq and d to pvfile as hexstrings, same as pub
}
void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", pq);
    gmp_fscanf(pvfile, "%Zx\n", d); //reads pq and d from pvfile, same as pub
}
void ss_encrypt(mpz_t c, mpz_t m, mpz_t n) {
    pow_mod(c, m, n, n); //sets c to the output of power mod
}
void ss_encrypt_file(FILE *infile, FILE *outfile, mpz_t n) {
    uint64_t k;
    size_t j;
    mpz_t sqrt_exp;
    mpz_init(sqrt_exp);
    mpz_sqrt(sqrt_exp, n); //sqrt_exp = sqrt(n)
    k = (mpz_sizeinbase(sqrt_exp, 2) - 1)
        / 8; //int k = ((log2*(sqrt(n))) -1)/8; //finding block size k
    mpz_t c;
    mpz_init(c);
    mpz_t msg; //setting a var to read infile to
    mpz_init(msg);
    uint8_t *arr = (uint8_t *) calloc(k, sizeof(uint8_t)); //initializing block as array
    arr[0] = 0xFF; //prepending byte to front of the block
    while ((j = fread(&arr[1], sizeof(uint8_t), k - 1, infile))
           > 0) { //condition to check for unprocessed bytes
        mpz_import(msg, j, 1, sizeof(uint8_t), 1, 0, arr); //converting array values
        ss_encrypt(c, msg, n); //encrypting values
        gmp_fprintf(outfile, "%Zx\n", c); //printing encrypted values to outfile
    }
    mpz_clear(sqrt_exp);
    mpz_clear(msg);
    mpz_clear(c);
    free(arr);
}
void ss_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t pq) {
    pow_mod(m, c, d, pq);
}
void ss_decrypt_file(FILE *infile, FILE *outfile, mpz_t pq, mpz_t d) {
    uint64_t k = (uint64_t) (mpz_sizeinbase(
        pq, 2)); //int k = log2(pq); //finding block size (double check, might be wrong)
    size_t j;
    uint8_t *arr = (uint8_t *) calloc(k, sizeof(uint8_t)); //allocating space for block
    mpz_t encrypt_c;
    mpz_init(encrypt_c);
    mpz_t msg; //vars to read infile and decrypt to
    mpz_init(msg);
    while (gmp_fscanf(infile, "%Zx\n", encrypt_c) != EOF) { //condition to read until end of file
        ss_decrypt(msg, encrypt_c, d, pq); //decrypting values
        mpz_export(arr, &j, 1, sizeof(uint8_t), 1, 0, msg); //converting values
        fwrite(&arr[1], sizeof(uint8_t), j - 1, outfile); //writing values to outfile
    }
    mpz_clear(encrypt_c);
    mpz_clear(msg);
    free(arr); //freeing allocated memory
}
