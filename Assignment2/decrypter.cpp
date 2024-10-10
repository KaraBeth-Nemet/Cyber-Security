// Write a Simplified DES decrypter that can take an input string (to standard in) and output the encrypted string (to standard out)
// Again, assume these are extended ASCII bytes in.
// Assume the secret key is passed in as the first, and only, command line parameter in the form 0xYYY. 

#include <stdio.h>
#include <stdlib.h>
#include <cmath>

// global variables
int all_debug = 0;
int main_debug = 0;
int ten_bit_key_debug = 0;
int k1_k2_debug = 0;
int feistel_function_debug = 0;

// permutations
int IP[] = {2, 6, 3, 1, 4, 8, 5, 7};
int IP_NEG_1[] = {4, 1, 3, 5, 7, 2, 8, 6};
int expansion[] = {4, 1, 2, 3, 2, 3, 4, 1};
int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
int P8[] = {6, 3, 7, 4, 8, 5, 10, 9};
int P4[] = {2, 4, 3, 1};

int subsitution_box_s0[4][4] = {
    {01, 00, 11, 10},
    {11, 10, 01, 00},
    {00, 10, 01, 11},
    {11, 01, 11, 10}
};

int subsitution_box_s1[4][4] = {
    {00, 01, 10, 11},
    {10, 00, 01, 11},
    {11, 00, 01, 00},
    {10, 01, 00, 11}
};


void char_to_binary(char c, char *output) {
    for (int i = 7; i >= 0; --i) {
        output[7 - i] = (c & (1 << i)) ? '1' : '0';
    }
}

void get_ten_bit_key(int key, char *key_10){
    // convert the key to a 10-bit binary number
    for (int i = 9; i >= 0; --i) {
        key_10[9 - i] = (key & (1 << i)) ? '1' : '0';
    }
    key_10[10] = '\0';

    // print the key for testing
    if(ten_bit_key_debug || all_debug){
        printf("Key as 10-bit: %s\n", key_10);
    }
}

void get_k1_and_k2(char *key, char *K1, char *K2){

    // perform the P10 permutation
    char p10_result[10];
    for (int i = 0; i < 10; ++i) {
        p10_result[i] = key[P10[i] - 1];
    }

    //print the P10 result
    if(k1_k2_debug || all_debug){
        printf("P10: ");
        for (int i = 0; i < 10; ++i) {
            printf("%c", p10_result[i]);
        }
        printf("\n");
    }


    // divide the P10 result into two halves
    char left[5];
    char right[5];
    for (int i = 0; i < 5; ++i) {
        left[i] = p10_result[i];
        right[i] = p10_result[i + 5];
    }

    // print the left and right halves
    if(k1_k2_debug || all_debug){
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

    // print the shifted left and right halves
    if(k1_k2_debug || all_debug){
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

    // print the combined result
    if(k1_k2_debug || all_debug){
        printf("Combined: ");
        for (int i = 0; i < 10; ++i) {
            printf("%c", combined[i]);
        }
        printf("\n");
    }

    // perform the P8 permutation
    for (int i = 0; i < 8; ++i) {
        K1[i] = combined[P8[i] - 1];
    }

    // print the K1 result
    if(k1_k2_debug || all_debug){
        printf("K1: ");
        for (int i = 0; i < 8; ++i) {
            printf("%c", K1[i]);
        }
        printf("\n");
    }

    // print the old shifted left and right halves to make sure they are correct
   if(k1_k2_debug || all_debug){
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

    // go back to the shifted left and right halves 
    // and perform another circular left shift of 2
    // Perform a circular left shift of 2 on the left and right halves
    char left_shifted_2[5];
    char right_shifted_2[5];
    for (int i = 0; i < 5; ++i) {
        left_shifted_2[i] = left_shifted[(i + 2) % 5];
        right_shifted_2[i] = right_shifted[(i + 2) % 5];
    }

    // Copy the shifted values back to the original arrays
    for (int i = 0; i < 5; ++i) {
        left_shifted[i] = left_shifted_2[i];
        right_shifted[i] = right_shifted_2[i];
    }

    // print the shifted left and right halves
    if(k1_k2_debug || all_debug){
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
    for (int i = 0; i < 5; ++i) {
        combined[i] = left_shifted[i];
        combined[i + 5] = right_shifted[i];
    }

    // print the combined result
    if(k1_k2_debug || all_debug){
        printf("Combined: ");
        for (int i = 0; i < 10; ++i) {
            printf("%c", combined[i]);
        }
        printf("\n");
    }

    // perform the P8 permutation
    for (int i = 0; i < 8; ++i) {
        K2[i] = combined[P8[i] - 1];
    }

    // print the K2 result
   if(k1_k2_debug || all_debug){
        printf("K2: ");
        for (int i = 0; i < 8; ++i) {
            printf("%c", K2[i]);
        }
        printf("\n");
    }
}

void feistal_function(char *input, char* k_val, char *output){

    // split input into two halves
    char left[4];
    char right[4];
    for (int i = 0; i < 4; ++i) {
        left[i] = input[i];
        right[i] = input[i + 4];
    }

    // print the left and right halves
    if(feistel_function_debug || all_debug){
        printf("Left: ");
        for (int i = 0; i < 4; ++i) {
            printf("%c", left[i]);
        }
        printf("\nRight: ");
        for (int i = 0; i < 4; ++i) {
            printf("%c", right[i]);
        }
        printf("\n");
    }

    // perform the expansion permutation on the right half
    char expanded[8];
    for (int i = 0; i < 8; ++i) {
        expanded[i] = right[expansion[i] - 1];
    }

    // print the expanded result
    if(feistel_function_debug || all_debug){
        printf("Expanded: ");
        for (int i = 0; i < 8; ++i) {
            printf("%c", expanded[i]);
        }
        printf("\n");
    }

    char xor_result[8];

    // perform the XOR operation
    for (int i = 0; i < 8; ++i) {
       xor_result[i] = ((expanded[i] - '0') ^ (k_val[i] - '0')) + '0';
    }

    // print the XOR result
    if(feistel_function_debug || all_debug){
        printf("XOR: ");
        for (int i = 0; i < 8; ++i) {
            printf("%c", xor_result[i]);
        }
        printf("\n");
    }

    
    // split the xor result into two halves
    char s0_s1[4];
    char s1_s1[4];
    for (int i = 0; i < 4; ++i) {
        s0_s1[i] = xor_result[i];
        s1_s1[i] = xor_result[i + 4];
    }

    // print the s0_s1 and s1_s1 for testing
    if(feistel_function_debug || all_debug){
        printf("S0_S1: ");
        for (int i = 0; i < 4; ++i) {
            printf("%c", s0_s1[i]);
        }
        printf("\n");
        printf("S1_S1: ");
        for (int i = 0; i < 4; ++i) {
            printf("%c", s1_s1[i]);
        }
        printf("\n");
    }

    // get the row and column values for the s0_s1 and s1_s1 values
    int row = 2 * (s0_s1[0] - '0') + (s0_s1[3] - '0');
    int col = 2 * (s0_s1[1] - '0') + (s0_s1[2] - '0');

    if(feistel_function_debug || all_debug){
        printf("Row: %d\n", row);
        printf("Col: %d\n", col);
    }

    // get the value from the s0 box
    int s0_val = subsitution_box_s0[row][col];


    // get the row and column values for the s1_s1 values
    int row2 = 2 * (s1_s1[0] - '0') + (s1_s1[3] - '0');
    int col2 = 2 * (s1_s1[1] - '0') + (s1_s1[2] - '0');

    if(feistel_function_debug || all_debug){
        printf("Row2: %d\n", row2);
        printf("Col2: %d\n", col2);
    }


    // get the value from the s1 box
    int s1_val = subsitution_box_s1[row2][col2];

    // print out the s0 and s1 values as they are
    if(feistel_function_debug || all_debug){
        printf("S0 Value: %d\n", s0_val);
        printf("S1 Value: %d\n", s1_val);
    }

    
    // turn the int values into char arrays
    char s0_val_binary[2];
    char s1_val_binary[2];

    for (int i = 1; i >= 0; --i) {
        s0_val_binary[1 - i] = (s0_val & (1 << i)) ? '1' : '0';
        s1_val_binary[1 - i] = (s1_val & (1 << i)) ? '1' : '0';
    }

    // print the s0 and s1 values as binary
    if(feistel_function_debug || all_debug){
        printf("S0 Value Binary: ");
        for (int i = 0; i < 2; ++i) {
            printf("%c", s0_val_binary[i]);
        }
        printf("\n");
        printf("S1 Value Binary: ");
        for (int i = 0; i < 2; ++i) {
            printf("%c", s1_val_binary[i]);
        }
        printf("\n");
    }

    // combine the s0 and s1 values where 1 and 2 are s0 and 3 and 4 are s1
    char combined[4];
    for (int i = 0; i < 2; ++i) {
        combined[i] = s0_val_binary[i];
        combined[i + 2] = s1_val_binary[i];
    }

   
    //testing if p4 does the same thing
    char p4_result[4];
    for (int i = 0; i < 4; ++i) {
        p4_result[i] = combined[P4[i] - 1];
    }

    // print left
    if(feistel_function_debug || all_debug){
        printf("Left: ");
        for (int i = 0; i < 4; ++i) {
            printf("%c", left[i]);
        }
        printf("\n");
    }
    

    // print to test
    if(feistel_function_debug || all_debug){
        printf("P4: ");
        for (int i = 0; i < 4; ++i) {
            printf("%c", p4_result[i]);
        }
        printf("\n");
    }

    // do XOR the output of the P4 table with the left half
    char xor_result2[4];
    for (int i = 0; i < 4; ++i) {
        xor_result2[i] = ((p4_result[i] - '0') ^ (left[i] - '0')) + '0';
    }

    // print the XOR result
    if(feistel_function_debug || all_debug){
        printf("XOR2: ");
        for (int i = 0; i < 4; ++i) {
            printf("%c", xor_result2[i]);
        }
        printf("\n");
    }

    // print out the right half
    if(feistel_function_debug || all_debug){
        printf("Right: ");
        for (int i = 0; i < 4; ++i) {
            printf("%c", right[i]);
        }
        printf("\n");
    }


    // combine the xor result with the right half
    for (int i = 0; i < 4; ++i) {
        output[i] = xor_result2[i];
        output[i + 4] = right[i];;
    }

    // print the output
    if(feistel_function_debug || all_debug){
        printf("Output: ");
        for (int i = 0; i < 8; ++i) {
            printf("%c", output[i]);
        }
        printf("\n");
    }

}

void decrypt(char *cipher_text, char *K1, char *K2, char *output){

    // perform the initial permutation
    char initial_permutation[8];
    for (int i = 0; i < 8; ++i) {
        initial_permutation[i] = cipher_text[IP[i] - 1];
    }

    // print the initial permutation for testing
    if(main_debug || all_debug){
        printf("Initial Permutation: ");
        for (int i = 0; i < 8; ++i) {
            printf("%c", initial_permutation[i]);
        }
        printf("\n");
    }

    if(main_debug || all_debug){
        printf("-----------------\n");
    }

    // call the feistel function with K2

    feistal_function(initial_permutation, K2, initial_permutation);

    if(main_debug || all_debug){
        printf("-----------------\n");
    }
    // perform the switch
    // divide the output into two halves of 4 bit each. Combine them again, 
    // but now the left part should become right and the right part should become left
    char switched[8];
    for (int i = 0; i < 4; ++i) {
        switched[i] = initial_permutation[i + 4];
        switched[i + 4] = initial_permutation[i];
    }

    // print the switched result
    if(main_debug || all_debug){
        printf("Switched: ");
        for (int i = 0; i < 8; ++i) {
            printf("%c", switched[i]);
        }
        printf("\n");
    }

    if(main_debug || all_debug){
        printf("-----------------\n");
    }


    // call the feistel function with K2
    feistal_function(switched, K1, switched);

    if(main_debug || all_debug){
        printf("-----------------\n");
    }

    // perform the inverse initial permutation
    char inverse_permutation[8];
    for (int i = 0; i < 8; ++i) {
        inverse_permutation[i] = switched[IP_NEG_1[i] - 1];
    }

    // print the inverse permutation for testing
    if(main_debug || all_debug){
        printf("Inverse Permutation: ");
        for (int i = 0; i < 8; ++i) {
            printf("%c", inverse_permutation[i]);
        }
        printf("\n");
    }

    if(main_debug || all_debug){
        printf("-----------------\n");
    }


    //print output in binary
    if(main_debug || all_debug){
        printf("Output in Binary: ");
        for(int i = 0; i < 8; ++i){
            printf("%c", inverse_permutation[i]);
        }
        printf("\n");
    }

    // convert the output to a character
    char c = 0;
    for (int i = 0; i < 8; ++i) {
        c += (inverse_permutation[i] - '0') * pow(2, 7 - i);
    }

    output[0] = c;


    // print the character
    if(main_debug || all_debug){
        printf("Character: %c\n", c);
    }

}

int main(int argc, char *argv[]){

    //get the key from the command line
    int key = strtol(argv[1], NULL, 16);

    if(main_debug || all_debug){
        printf("Key: %d\n", key);
    }

    // get the 10-bit key
    char key_10[10];
    get_ten_bit_key(key, key_10);

    // get the k1 and k2 values
    char k1[8];
    char k2[8];
    get_k1_and_k2(key_10, k1, k2);

   // keep reading characters until EOF is DECRYPTED
    char c;
    char output[1] = { };
    char cipher_text[8];
    while (output[0] != EOF) {
        c = getchar();
        char_to_binary(c, cipher_text);
        decrypt(cipher_text, k1, k2, output);
        printf("%c", output[0]);
    }
    return 0;

}