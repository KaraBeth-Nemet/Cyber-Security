// Write a Simplified DES decrypter that can take an input string (to standard in) and output the encrypted string (to standard out)
// Again, assume these are extended ASCII bytes in.
// Assume the secret key is passed in as the first, and only, command line parameter in the form 0xYYY. 

//Plaintext = IP-1 (fK1 (SW (fk2 (IP (ciphertext)))))

#include <stdio.h>
#include <stdlib.h>
#include <cmath>


//global permutation variables
int IP[] = {2, 6, 3, 1, 4, 8, 5, 7};
int IP_NEG_1[] = {4, 1, 3, 5, 7, 2, 8, 6};
int EP[] = {4, 1, 2, 3, 2, 3, 4, 1};
int P4[] = {2, 4, 3, 1};
int P8[] = {6, 3, 7, 4, 8, 5, 10, 9};
int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};


int subsitution_box_s0[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};
int subsitution_box_s1[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};


// turn a char into a binary
void char_to_binary(char c, char *output) {
   for (int i = 7; i >= 0; --i) {
        output[7 - i] = (c & (1 << i)) ? '1' : '0';
    }
}

char binary_array_to_char(char *binary_array, char *output) {
    int temp;
    for (int i = 0; i < 8; ++i)
    {
        temp += output[i] * pow(2, 7 - i);
        //print the temp for testing
        printf("Temp: %d\n", temp);

    }
    char finChar = temp;
    return finChar;
}

// feistel function
void feistel_function(char *input, char *k_val, char *output) {

    // see what k_val is
    printf("K_val: ");
    for (int i = 0; i < 8; ++i) {
        printf("%c", k_val[i]);
    }
    printf("\n");


    // divide the input into two halfs, left and right
    char left[4];
    char right[4];
    for (int i = 0; i < 4; ++i) {
        left[i] = input[i];
        right[i] = input[i + 4];
    }

    // print the left and right halves for testing
    printf("Left: ");
    for (int i = 0; i < 4; ++i) {
        printf("%c", left[i]);
    }
    printf("\nRight: ");
    for (int i = 0; i < 4; ++i) {
        printf("%c", right[i]);
    }
    printf("\n");

    // expland the right half
    char right_expanded[8];
    for (int i = 0; i < 8; ++i) {
        right_expanded[i] = right[EP[i] - 1];
    }

    // print the expanded right half for testing
    printf("Right Expanded: ");
    for (int i = 0; i < 8; ++i) {
        printf("%c", right_expanded[i]);
    }
    printf("\n");

    // XOR the expanded right half with the key
    char right_expanded_xor_key[8];
    for (int i = 0; i < 8; ++i) {
        right_expanded_xor_key[i] = (right_expanded[i] == k_val[i]) ? '0' : '1';
    }

    // print the XOR result for testing
    printf("Right Expanded XOR Key: ");
    for (int i = 0; i < 8; ++i) {
        printf("%c", right_expanded_xor_key[i]);
    }
    printf("\n");


    // divide the XOR result into two halves
    char xor_left[4];
    char xor_right[4];
    for (int i = 0; i < 4; ++i) {
        xor_left[i] = right_expanded_xor_key[i];
        xor_right[i] = right_expanded_xor_key[i + 4];
    }

    // print the left and right halves of the XOR result for testing
    printf("XOR Left: ");
    for (int i = 0; i < 4; ++i) {
        printf("%c", xor_left[i]);
    }
    printf("\nXOR Right: ");
    for (int i = 0; i < 4; ++i) {
        printf("%c", xor_right[i]);
    }
    printf("\n");

    // Use the substitution rules in the PowerPoint: compare the 1st and 4th bit for the row, 2nd and third for the column
    // left side is S0, right side is S1. These align to a binary number, convert to decimal use these 2 decimal numbers as coordinates to find the value in the S-box

    //get the row and column for S0
    int row0 = (xor_left[0] - '0') * 2 + (xor_left[3] - '0');
    int col0 = (xor_left[1] - '0') * 2 + (xor_left[2] - '0');
    int s0_value = subsitution_box_s0[row0][col0];

    // print s0 value for testing
    printf("S0 Value: %d\n", s0_value);

    // get the row and column for S1
    int row1 = (xor_right[0] - '0') * 2 + (xor_right[3] - '0');
    int col1 = (xor_right[1] - '0') * 2 + (xor_right[2] - '0');
    int s1_value = subsitution_box_s1[row1][col1];

    //print s1 value for testing
    printf("S1 Value: %d\n", s1_value);

    // convert the S-box values to binary
    char s0_binary[2];
    char s1_binary[2];
    s0_binary[0] = (s0_value & 2) ? '1' : '0';
    s0_binary[1] = (s0_value & 1) ? '1' : '0';
    s1_binary[0] = (s1_value & 2) ? '1' : '0';
    s1_binary[1] = (s1_value & 1) ? '1' : '0';

    // print the S-box binary values for testing
    printf("S0 Binary: ");
    for (int i = 0; i < 2; ++i) {
        printf("%c", s0_binary[i]);
    }
    printf("\nS1 Binary: ");
    for (int i = 0; i < 2; ++i) {
        printf("%c", s1_binary[i]);
    }
    printf("\n");

    // combine the S-box binary values
    char s_combined[4];
    s_combined[0] = s0_binary[0];
    s_combined[1] = s0_binary[1];
    s_combined[2] = s1_binary[0];
    s_combined[3] = s1_binary[1];

    // print the combined S-box binary values for testing
    printf("S Combined: ");
    for (int i = 0; i < 4; ++i) {
        printf("%c", s_combined[i]);
    }
    printf("\n");

    // perform the P4 permutation
    for (int i = 0; i < 4; ++i) {
        output[i] = s_combined[P4[i] - 1];
    }

    // print the P4 result for testing
    printf("P4: ");
    for (int i = 0; i < 4; ++i) {
        printf("%c", output[i]);
    }
    printf("\n");

    // XOR the left half with the P4 result
    for (int i = 0; i < 4; ++i) {
        output[i] = (left[i] == output[i]) ? '0' : '1';
    }

    // print the XOR result for testing
    printf("XOR Left with P4: ");
    for (int i = 0; i < 4; ++i) {
        printf("%c", output[i]);
    }
    printf("\n");

    // combine the left and right halves
    for (int i = 0; i < 4; ++i) {
        output[i + 4] = right[i];
    }

    // print the combined result for testing
    printf("Combined: ");
    for (int i = 0; i < 8; ++i) {
        printf("%c", output[i]);
    }
    printf("\n");
}


void find_k1_and_k2(char *key, char *K1, char *K2) {
    // perform the P10 permutation
    char p10_result[10];
    for (int i = 0; i < 10; ++i) {
        p10_result[i] = key[P10[i] - 1];
    }

    //print the P10 result
    printf("P10: ");
    for (int i = 0; i < 10; ++i) {
        printf("%c", p10_result[i]);
    }

    // divide the P10 result into two halves
    char left[5];
    char right[5];
    for (int i = 0; i < 5; ++i) {
        left[i] = p10_result[i];
        right[i] = p10_result[i + 5];
    }

    // print the left and right halves
    printf("\nLeft: ");
    for (int i = 0; i < 5; ++i) {
        printf("%c", left[i]);
    }
    printf("\nRight: ");
    for (int i = 0; i < 5; ++i) {
        printf("%c", right[i]);
    }
    printf("\n");

    // perform a circular left shift on the left and right halves
    char left_shifted[5];
    char right_shifted[5];
    for (int i = 0; i < 5; ++i) {
        left_shifted[i] = left[(i + 1) % 5];
        right_shifted[i] = right[(i + 1) % 5];
    }

    // print the shifted left and right halves
    printf("Left Shifted: ");
    for (int i = 0; i < 5; ++i) {
        printf("%c", left_shifted[i]);
    }
    printf("\nRight Shifted: ");
    for (int i = 0; i < 5; ++i) {
        printf("%c", right_shifted[i]);
    }
    printf("\n");

    // combine the shifted left and right halves
    char combined[10];
    for (int i = 0; i < 5; ++i) {
        combined[i] = left_shifted[i];
        combined[i + 5] = right_shifted[i];
    }

    // print the combined result
    printf("Combined: ");
    for (int i = 0; i < 10; ++i) {
        printf("%c", combined[i]);
    }
    printf("\n");

    // perform the P8 permutation
    for (int i = 0; i < 8; ++i) {
        K1[i] = combined[P8[i] - 1];
    }

    // print the K1 result
    printf("K1: ");
    for (int i = 0; i < 8; ++i) {
        printf("%c", K1[i]);
    }
    printf("\n");

    // print the old shifted left and right halves to make sure they are correct
    printf("Old Left Shifted: ");
    for (int i = 0; i < 5; ++i) {
        printf("%c", left_shifted[i]);
    }
    printf("\nOld Right Shifted: ");
    for (int i = 0; i < 5; ++i) {
        printf("%c", right_shifted[i]);
    }
    printf("\n");

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
    printf("Left Shifted: ");
    for (int i = 0; i < 5; ++i) {
        printf("%c", left_shifted[i]);
    }
    printf("\nRight Shifted: ");
    for (int i = 0; i < 5; ++i) {
        printf("%c", right_shifted[i]);
    }
    printf("\n");

    // combine the shifted left and right halves
    for (int i = 0; i < 5; ++i) {
        combined[i] = left_shifted[i];
        combined[i + 5] = right_shifted[i];
    }

    // print the combined result
    printf("Combined: ");
    for (int i = 0; i < 10; ++i) {
        printf("%c", combined[i]);
    }
    printf("\n");

    // perform the P8 permutation
    for (int i = 0; i < 8; ++i) {
        K2[i] = combined[P8[i] - 1];
    }

    // print the K2 result
    printf("K2: ");
    for (int i = 0; i < 8; ++i) {
        printf("%c", K2[i]);
    }
    printf("\n");

}


void decrypt(char *input, char *output, char *K1, char *K2) {
    // perform the initial permutation
    char ip_result[8];
    for (int i = 0; i < 8; ++i) {
        ip_result[i] = input[IP[i] - 1];
    }

    // print the IP result
    printf("IP: ");
    for (int i = 0; i < 8; ++i) {
        printf("%c", ip_result[i]);
    }
    printf("\n");

    // perform the feistel function with K2
    char feistel_result[8];
    feistel_function(ip_result, K2, feistel_result);

    // print the feistel result
    printf("Feistel: ");
    for (int i = 0; i < 8; ++i) {
        printf("%c", feistel_result[i]);
    }
    printf("\n");

    // perform the switch
    char switch_result[8];
    for (int i = 0; i < 8; ++i) {
        switch_result[i] = feistel_result[i];
    }

    // print the switch result
    printf("Switch: ");
    for (int i = 0; i < 8; ++i) {
        printf("%c", switch_result[i]);
    }
    printf("\n");

    // perform the feistel function with K1
    feistel_function(switch_result, K1, feistel_result);

    // print the feistel result
    printf("Feistel: ");
    for (int i = 0; i < 8; ++i) {
        printf("%c", feistel_result[i]);
    }
    printf("\n");

    // perform the inverse initial permutation
    for (int i = 0; i < 8; ++i) {
        output[i] = feistel_result[IP_NEG_1[i] - 1];
    }

    // print the output
    printf("Output: ");
    for (int i = 0; i < 8; ++i) {
        printf("%c", output[i]);
    }
    printf("\n");

    // convert the output to a character
    int output_int[8];
    for (int i = 0; i < 8; ++i) {
        output_int[i] = output[i] - '0';
    }

    // print output_int for testing
    printf("Output Int: ");
    for (int i = 0; i < 8; ++i) {
        printf("%d", output_int[i]);
    }
    printf("\n");

    //convert the output to a character

    char output_char = binary_array_to_char(output, output);
    printf("Output Char: %c\n", output_char);




}


int main(int argc, char *argv[]) {

    u_int8_t binaryValue = 0b01000001; // Binary for 'A' (ASCII 65)
    
    // Convert binary value to a character
    char character = (char)binaryValue;
    
    // Print the character
    printf("The character is: %c\n", character);

    // Convert the hexadecimal key to a 10-bit binary number
    unsigned int key;
    sscanf(argv[1], "0x%x", &key);

    if (key > 0x3FF) { // 10-bit number can only be up to 0x3FF
        printf("The key must be a 10-bit number (0x000 to 0x3FF)\n");
        return 1;
    }

    char key_10[11]; // 10 bits + null terminator
    for (int i = 9; i >= 0; --i) {
        key_10[9 - i] = (key & (1 << i)) ? '1' : '0';
    }
    key_10[10] = '\0';

    printf("Key: %s\n", key_10);

    //print a line to seperate the key from the rest of the output
    printf("--------------------------------------------------\n");

    //test the find_k1_and_k2 function
    char K1[8];
    char K2[8];
    find_k1_and_k2(key_10, K1, K2);

    // print K1 and K2 for testing
    printf("K1: ");
    for (int i = 0; i < 8; ++i) {
        printf("%c", K1[i]);
    }
    printf("\n");
    printf("K2: ");
    for (int i = 0; i < 8; ++i) {
        printf("%c", K2[i]);
    }
    printf("\n");

    //print a line to seperate the key from the rest of the output
    printf("--------------------------------------------------\n");

    //test the feistel function
    char input[8] = {'1', '0', '1', '1', '1', '1', '0', '1'};
    feistel_function(input, K1, input);
    printf("--------------------------------------------------\n");
    feistel_function(input, K2, input);

    //have user input a character for tesitng
    char c;
    printf("Enter a character: ");
    scanf("%c", &c);
    //print out character to check 
    printf("Character: %c\n", c);
    //convert character to binary
    char binary[8];
    char_to_binary(c, binary);
    //print out binary to check
    printf("Binary: ");
    for (int i = 0; i < 8; ++i) {
        printf("%c", binary[i]);
    }
    printf("\n");
    printf("--------------------------------------------------\n");

    //test the decrypt function
    char output[8];
    decrypt(binary, output, K1, K2);







    return 0;
}