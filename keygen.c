#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "ss.h"
#include "numtheory.h"
#include <time.h>
#include <gmp.h>
#include <unistd.h>
#include "randstate.h"
#include <sys/stat.h>

#define OPTIONS       "b:i:n:d:s:vh"
#define PRIVKEY_PERMS 0600 //double check
void print_usage(void) {
    fprintf(stderr, "SYNOPSIS\n   Generates an SS private/public key pair.\n\nUSAGE\n   ./keygen "
                    "[-b: -i: -n: -d: -s: -v -h]\n\nOPTIONS\n   -h              Display program "
                    "help and usage.\n   -v              Display verbose program output.\n   -b "
                    "bits         Minimum bits needed for public key n (default:256).\n   -i "
                    "iterations   Miller-Rabin iterations for testing primes (default: 50).\n   -n "
                    "pbfile       Public key file (default: ss.pub).\n   -d pvfile       Private "
                    "key file (default: ss.priv)\n   -s seed        Random seed for testing.\n");
}

int main(int argc, char *argv[]) {
    uint64_t bits = 256;
    uint64_t iters = 50;
    char *pubfile = "ss.pub";
    char *privfile = "ss.priv";
    FILE *pbfile = NULL;
    FILE *pvfile = NULL;
    unsigned int randseed = time(NULL); //setting default seed
    bool verbose = false;
    randseed = time(NULL);
    char *username = getenv("USER");
    mpz_t p;
    mpz_init(p);
    mpz_t q;
    mpz_init(q);
    mpz_t n;
    mpz_init(n);
    mpz_t d;
    mpz_init(d);
    mpz_t pq;
    mpz_init(pq);
    int opt;
    while ((opt = getopt(argc, argv, "b:i:n:d:s:vh")) != -1) {
        switch (opt) {
        case 'b': bits = strtoull(optarg, NULL, 10); break;
        case 'i': iters = strtoull(optarg, NULL, 10); break;
        case 'n': pubfile = optarg; break;
        case 'd': privfile = optarg; break;
        case 's': randseed = atoi(optarg); break;
        case 'v': verbose = true; break;
        case 'h': print_usage(); exit(0);
        default: print_usage(); exit(1);
        }
    }
    if ((fopen(pubfile, "w") == NULL)) { //checks to see if specified public key file can be opened
        perror("Couldn't open public key file :(");
        mpz_clear(p);
        mpz_clear(q);
        mpz_clear(n);
        mpz_clear(d);
        mpz_clear(pq);
        exit(1);
    } else if (fopen(pubfile, "w") != NULL) { //if public key file can be opened, set pbfile to it
        pbfile = fopen(pubfile, "w");
    }
    if (fopen(privfile, "w") == NULL) { //if private key file can't be opened, error and exit
        perror("Couldn't open private key file :(");
        fclose(pbfile);
        mpz_clear(n);
        mpz_clear(q);
        mpz_clear(p);
        mpz_clear(d);
        mpz_clear(pq);
        exit(1);
    } else if (fopen(privfile, "w") != NULL) { //if private key file can be opened, set pvfile to it
        pvfile = fopen(privfile, "w");
    }
    if ((fchmod(fileno(pvfile), PRIVKEY_PERMS))
        == -1) { //set permissions: if it doesn't work, error
        perror("Couldn't set private key file permission :(");
        fclose(pbfile);
        fclose(pvfile);
        mpz_clear(n);
        mpz_clear(p);
        mpz_clear(q);
        mpz_clear(d);
        mpz_clear(pq);
        exit(1);
    }
    randstate_init(randseed); //initialize state using the seed
    ss_make_pub(p, q, n, bits, iters); //make the public key
    ss_make_priv(d, pq, p, q); //make the private key
    ss_write_pub(n, username, pbfile); //write public key to specified file
    ss_write_priv(pq, d, pvfile); //write private key to specified file
    if (verbose == true) { //if verbose output is selected, print p, qp, n, d and pq
        printf("user = %s\n", username);
        gmp_printf("p (%d bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q (%d bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("d (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
        gmp_printf("pq (%d bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
    }
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(d);
    mpz_clear(pq); //clear all mpz_t
    fclose(pbfile);
    fclose(pvfile); //close opened files
    randstate_clear(); //clear random state
    return 0;
}
