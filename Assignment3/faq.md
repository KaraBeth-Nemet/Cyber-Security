# FAQ 4470 Assignment 3 RSA encryption and Cracking

- For all purposes I treated each of the unencrypted bytes individually. I turned each one into an unsigned 64 bit integer and then encrypted them with RSA. They were left padded with 0's. When decrypting it is therefore necessary to take each 64 bit chunk of bytes (8 bytes) and decrypt them into 1 byte.
- The flow of data types is something like this, where n is the length in bytes in your input.
  - Encryption: unsigned char[n] -> unsigned long[n]
  - Decryption: unsigned long[n] -> unsigned char[n]
- I have provided the secret keys and all inputs and outputs for your tests, but there will be additional tests that I'm holding back.
- I included 2 variants of each of the expected outputs to account for the fact that either prime number could be found first. You may output them in any order, it won't affect your grade. Just check your output against both versions and if it matches one of them you can consider that a passed test.
