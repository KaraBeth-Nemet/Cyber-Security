// Write a Simplified DES decrypter that can take an input string (to standard in) and output the encrypted string (to standard out)
// Again, assume these are extended ASCII bytes in.
// Assume the secret key is passed in as the first, and only, command line parameter in the form 0xYYY. 

#include <stdio.h>
#include <stdlib.h>
#include <cmath>


//global permutation variables
int debug = 0;
int main_debug = 1;

int IP[] = {2, 6, 3, 1, 4, 8, 5, 7};
int IP_NEG_1[] = {4, 1, 3, 5, 7, 2, 8, 6};
int EP[] = {4, 1, 2, 3, 2, 3, 4, 1};
int P4[] = {2, 4, 3, 1};
int P8[] = {6, 3, 7, 4, 8, 5, 10, 9};
int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};

// 00 = 0
// 01 = 1
// 10 = 2
// 11 = 3

int subsitution_box_s0[4][4] = {
    {01, 00, 11, 10},
    {11, 10, 01, 00},
    {00, 02, 01, 11},
    {11, 01, 11, 02}
};
int subsitution_box_s1[4][4] = {
    {00, 01, 10, 11},
    {10, 00, 01, 11},
    {11, 00, 01, 00},
    {10, 01, 00, 11}
};

// steps to perform the simplified DES encryption
// 1. Read in 8-bit ciphertext
// 2. Perform the initial permutation
// 3. Perform the first feistel function with K2
// 4. Perform the switch
// 5. Perform the second feistel function with K1
// 6. Perform the inverse initial permutation
// 7. Output the 8-bit plaintext

// steps for getting a 10-bit key
// 1. Read in the key from command line
// 2. Perform the P10 permutation
// 3. Perform the left shift
// 4. Perform the P8 permutation to get
// 5. Perform the left shift on step 3 result to get K2

void char_to_binary(char c, char *output) {
    for (int i = 7; i >= 0; --i) {
        output[7 - i] = (c & (1 << i)) ? '1' : '0';
    }
}

void ten_bit_key(int key, char *key_10) {
    // convert the key to a 10-bit binary number
    for (int i = 9; i >= 0; --i) {
        key_10[9 - i] = (key & (1 << i)) ? '1' : '0';
    }
    key_10[10] = '\0';

    // print the key for testing
    if(main_debug){
        printf("Key as 10-bit: %s\n", key_10);
    }
}

void k1_and_k2(char *key_10, char *k1, char *k2){
    // perform the p10 permutation
    char p10_result[10];
    for (int i = 0; i < 10; ++i) {
        p10_result[i] = key_10[P10[i] - 1];
    }

    // print the p10 result for testing
    if(debug){
        printf("P10: ");
        for (int i = 0; i < 10; ++i) {
            printf("%c", p10_result[i]);
        }
        printf("\n");
    }

    // split the p10 result into two halves
    char left[5];
    char right[5];
    for (int i = 0; i < 5; ++i) {
        left[i] = p10_result[i];
        right[i] = p10_result[i + 5];
    }

    // print the left and right halves for testing
    if(debug){
        printf("Left: ");
        for (int i = 0; i < 5; ++i) {
            printf("%c", left[i]);
        }
        printf("\nRight: ");
        for (int i = 0; i < 5; ++i) {
            printf("%c", right[i]);
        }
        printf("\n");
    }

    // perform a circular left shift on the left and right halves
    char left_shifted[5];
    char right_shifted[5];
    for (int i = 0; i < 5; ++i) {
        left_shifted[i] = left[(i + 1) % 5];
        right_shifted[i] = right[(i + 1) % 5];
    }

    // print the shifted left and right halves for testing
    if(debug){
        printf("Left Shifted: ");
        for (int i = 0; i < 5; ++i) {
            printf("%c", left_shifted[i]);
        }
        printf("\nRight Shifted: ");
        for (int i = 0; i < 5; ++i) {
            printf("%c", right_shifted[i]);
        }
        printf("\n");
    }

    // combine the shifted left and right halves
    char combined[10];
    for (int i = 0; i < 5; ++i) {
        combined[i] = left_shifted[i];
        combined[i + 5] = right_shifted[i];
    }

    // print the combined result for testing
    if(debug){
        printf("Combined: ");
        for (int i = 0; i < 10; ++i) {
            printf("%c", combined[i]);
        }
        printf("\n");
    }

    // perform the P8 permutation on the combined result to get k1
    for (int i = 0; i < 8; ++i) {
        k1[i] = combined[P8[i] - 1];
    }

    // print the K1 result for testing
    if(main_debug){
        printf("K1: ");
        for (int i = 0; i < 8; ++i) {
            printf("%c", k1[i]);
        }
        printf("\n");
    }

    // go BACK to the shifted left and right halves
    // and perform another circular left shift of 2

    // Perform a circular left shift of 2 on the left and right halves
    char left_shifted_2[5];
    char right_shifted_2[5];
    for(int i = 0; i < 5; ++i){
        left_shifted_2[i] = left_shifted[(i + 2) % 5];
        right_shifted_2[i] = right_shifted[(i + 2) % 5];
    }


    // print the shifted left and right halves for testing
    if(debug){
        printf("Left Shifted: ");
        for (int i = 0; i < 5; ++i) {
            printf("%c", left_shifted_2[i]);
        }
        printf("\nRight Shifted: ");
        for (int i = 0; i < 5; ++i) {
            printf("%c", right_shifted_2[i]);
        }
        printf("\n");
    }

    // Combine the new shifted left and right halves
    for (int i = 0; i < 5; ++i) {
        left_shifted[i] = left_shifted_2[i];
        right_shifted[i] = right_shifted_2[i];
    }

    // combine the shifted left and right halves
    for (int i = 0; i < 5; ++i) {
        combined[i] = left_shifted[i];
        combined[i + 5] = right_shifted[i];
    }

    // print the combined result for testing
    if(debug){
        printf("Combined: ");
        for (int i = 0; i < 10; ++i) {
            printf("%c", combined[i]);
        }
        printf("\n");
    }

    // perform the P8 permutation on the combined result to get k1
    for (int i = 0; i < 8; ++i) {
        k2[i] = combined[P8[i] - 1];
    }

    // print the K1 result for testing
    if(main_debug){
        printf("K2: ");
        for (int i = 0; i < 8; ++i) {
            printf("%c", k2[i]);
        }
        printf("\n");
    }
}


int main (int argc, char *argv[]) {
    
    // check if the key is passed in
    if (argc != 2) {
        printf("Please enter a key in the form 0xYYY\n");
        return 1;
    }
    // check if key is in the correct format
    int entered_key;
    sscanf(argv[1], "0x%x", &entered_key);
    if (entered_key > 0x3FF) { // 10-bit number can only be up to 0x3FF
        printf("The key must be a 10-bit number (0x000 to 0x3FF)\n");
        return 1;
    }
    
    // print out key for testing
    if(main_debug) {
        printf("Entered Key: %s\n", argv[1]);
    }

    // convert the key to a 10-bit binary number
    char key_10[11]; // 10 bits + null terminator
    
    // turn the key into a 10-bit binary number
    ten_bit_key(entered_key, key_10);

    char k1[9]; // 8 bits + null terminator
    char k2[9]; // 8 bits + null terminator
    k1_and_k2(key_10, k1, k2);

    // read in character and convert to binary
    char c;
    char binary[8];
    printf("Enter a character : ");
    scanf("%c", &c);
    char_to_binary(c, binary);
    // the binary of the character IS the 8-bit ciphertext
    // we are trying to decrypt

    // print the binary for testing
    if(main_debug) {
        printf("Character in Binary: ");
        for (int i = 0; i < 8; ++i) {
            printf("%c", binary[i]);
        }
        printf("\n");
    }

    // perform the initial permutation
    char initial_permutation[8];
    for (int i = 0; i < 8; ++i) {
        initial_permutation[i] = binary[IP[i] - 1];
    }

    // print the initial permutation for testing
    if(main_debug) {
        printf("Initial Permutation: ");
        for (int i = 0; i < 8; ++i) {
            printf("%c", initial_permutation[i]);
        }
        printf("\n");
    }

    // feistel function with K2
    // need help

    




    return 0;

}