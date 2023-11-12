#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <gmp.h>
#include <stdbool.h>
#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

#define OPTIONS "i:o:n:vh"

void print_usage(void) {
    fprintf(stderr,
        "SYNOPSIS\n   Encrypts data using SS encryption. Encrypted data is decrypted by the "
        "decrypt program.\n\nUSAGE\n   ./encrypt [-i: -o: -n: -v -h]\n\nOPTIONS\n   -h           "
        "Display program help and usage.\n   -v          Display verbose program output.\n   -i "
        "infile   Input file of data to encrypt (default: stdin).\n   -o outfile  Output file for "
        "encrypted data (default: stdout).\n   -n pbfile   Public key file (default: ss.pub).\n");
}

int main(int argc, char *argv[]) {
    FILE *infile = NULL;
    char *in = NULL;
    FILE *outfile = NULL;
    char *out = NULL;
    FILE *pbfile = NULL;
    char *pubkey_file = "ss.pub";
    bool verbose = false;
    int opt;

    while ((opt = getopt(argc, argv, "i:o:n:vh")) != -1) {
        switch (opt) {
        case 'i': in = optarg; break;
        case 'o': out = optarg; break;
        case 'n': pubkey_file = optarg; break;
        case 'v': verbose = true; break;
        case 'h': print_usage(); exit(0);
        default: print_usage(); exit(1);
        }
    }
    mpz_t n;
    mpz_init(n);
    char *username = getenv("USER");
    if (fopen(pubkey_file, "r") == NULL) { //if public key file doesn't open correctly, error
        perror("Couldn't open public key file to read");
        mpz_clear(n);
        exit(1); //double check
    } else if (fopen(pubkey_file, "r") != NULL) {
        pbfile = fopen(pubkey_file, "r");
        ss_read_pub(n, username, pbfile); //if it does open, set to pbfile and read the key
    }
    if ((out != NULL) && (fopen(out, "w") == NULL)) {
        perror("File specified couldn't be opened :(");
        fclose(pbfile);
        mpz_clear(n);
        exit(1); //if specified outfile doesn't open correctly, close all files and exit
    } else if ((out != NULL)) {
        outfile = fopen(out, "w"); //if file is specified, set to outfile
    } else if (out == NULL) {
        outfile = stdout; //if not, set to stdout (default)
    }
    if ((in != NULL) && (fopen(in, "r") == NULL)) {
        perror("Input file specified couldn't be opened :(");
        fclose(pbfile);
        fclose(outfile);
        mpz_clear(n); //if input file doesn't open correctly, exit and close files
        exit(1);
    } else if ((in != NULL)) {
        infile = fopen(in, "r"); //if infile is specified, open
    } else if (in == NULL) {
        infile = stdin; //default infile
    }
    if (verbose == true) { //if verbose output is enabled, print user and n
        printf("user :%s\n", username);
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
    }
    ss_encrypt_file(infile, outfile, n);
    fclose(pbfile);
    fclose(infile);
    fclose(outfile);
    mpz_clear(n); //close and clear everything
    return 0;
}
