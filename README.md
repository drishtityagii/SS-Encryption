# SS Encryption

Contains three files that implement the SS encryption/decryption program. keygen.c generates the private/public key pair and takes options -b (specified the minimum bits needed for public modulus n), -i (specifies the iterations used in the Miller-Rabin primality test), -n (specifies the public key file), -d (specifies the private key file), -s (specifies the seed for the random state initialization), -v (enables verbose output), and -h (prints usage). The second program, encrypt.c, performs the encryption process and takes options -i (specifies the input file to encrypt), -o (specifies the output file to encrypt), -n (specifies the file containing the public key), -v (enables verbose output), -h (displays program usage). The third program, decrypt.c, performs the decryption and accepts options -i (specifies input file to decrypt), -o (specifies the output file to decrypt), -n (specifies the file containing the private key), -v (enables verbose output), and -h (displays program usage).

## Building

...

$ make all

or, for specific programs, 

$ make keygen

$ make encrypt

$ make decrypt

...

## Running

...

$ ./keygen

$ ./encrypt

$ ./decrypt

...
