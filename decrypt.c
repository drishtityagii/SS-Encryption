#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <gmp.h>
#include "randstate.h"
#include "ss.h"
#include "numtheory.h"

#define OPTIONS "i:o:n:vh"

void print_usage(void) {
    fprintf(stderr, "SYNOPSIS\n  Decrypts data using SS decryption.\n  Encrypted data is encrypted "
                    "by the encrypt program.\n\nUSAGE\n   ./decrypt [-i: -o: -n: -v "
                    "-h]\n\nOPTIONS\n   -h          Display program help and usage.\n   -v         "
                    " Display verbose program output.\n   -i infile   Input file of the data to "
                    "decrypt (default: stdin).\n   -o outfile  Outut file for decrypted data "
                    "(default: stdout).\n   -n pvfile   Private key file (default: ss.priv).\n");
}

int main(int argc, char *argv[]) {
    FILE *infile = NULL; //file pointer for input
    char *in = NULL;
    FILE *outfile = NULL; //file pointer for output
    char *out = NULL;
    char *privkey_file = "ss.priv";
    FILE *pvfile = NULL;
    bool verbose = false;
    mpz_t pq;
    mpz_init(pq);
    mpz_t d;
    mpz_init(d);
    int opt;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': in = optarg; break;
        case 'o': out = optarg; break;
        case 'n': privkey_file = optarg; break;
        case 'v': verbose = true; break;
        case 'h': print_usage(); exit(0);
        default: print_usage(); exit(1); //double check
        }
    }
    if ((fopen(privkey_file, "r"))
        == NULL) { //checks to see if specified private key file can be opened
        perror("Couldn't open private key file to read :(");
        mpz_clear(pq);
        mpz_clear(d);
        exit(1);
    } else if ((fopen(privkey_file, "r")) != NULL) {
        pvfile = fopen(privkey_file, "r");
        ss_read_priv(pq, d, pvfile); //if file opens without errors, read the private key
    }
    if (verbose == true) {
        gmp_printf(
            "pq (%d bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq); //print pq and its information
        gmp_printf("d (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d); //print d and its information
    }
    if ((in != NULL) && (fopen(in, "r") == NULL)) {
        perror("Couldn't read specified input file :(");
        mpz_clear(pq);
        mpz_clear(d);
        fclose(pvfile);
        exit(1); //if input file is specified and doesn't open, handle error
    } else if ((in != NULL) && (fopen(in, "r") != NULL)) {
        infile = fopen(in, "r"); //if input file is specified and opens, open and use
    } else if (in == NULL) {
        infile = stdin; //default input
    }
    if ((out != NULL) && (fopen(out, "w") == NULL)) {
        perror("Couldn't open specified output file :(");
        mpz_clear(pq);
        mpz_clear(d);
        fclose(pvfile);
        fclose(infile);
        exit(1); //if output file is specified and doesn't open, handle error
    } else if ((out != NULL) && (fopen(out, "w") != NULL)) {
        outfile = fopen(out, "w"); //if output file is specified and opens, open and use
    } else if (out == NULL) {
        outfile = stdout; //default output
    }
    ss_decrypt_file(infile, outfile, pq, d); //decrypt using vals we got
    fclose(pvfile);
    fclose(infile);
    fclose(outfile);
    mpz_clear(d);
    mpz_clear(pq); //close and clear everything
    return 0;
}
