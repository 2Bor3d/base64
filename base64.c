#include "base64.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char encoding_table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

static char decoding_table[] = {
    ['A'] = 0,  ['B'] = 1,  ['C'] = 2,  ['D'] = 3,  ['E'] = 4,  ['F'] = 5,
    ['G'] = 6,  ['H'] = 7,  ['I'] = 8,  ['J'] = 9,  ['K'] = 10, ['L'] = 11,
    ['M'] = 12, ['N'] = 13, ['O'] = 14, ['P'] = 15, ['Q'] = 16, ['R'] = 17,
    ['S'] = 18, ['T'] = 19, ['U'] = 20, ['V'] = 21, ['W'] = 22, ['X'] = 23,
    ['Y'] = 24, ['Z'] = 25, ['a'] = 26, ['b'] = 27, ['c'] = 28, ['d'] = 29,
    ['e'] = 30, ['f'] = 31, ['g'] = 32, ['h'] = 33, ['i'] = 34, ['j'] = 35,
    ['k'] = 36, ['l'] = 37, ['m'] = 38, ['n'] = 39, ['o'] = 40, ['p'] = 41,
    ['q'] = 42, ['r'] = 43, ['s'] = 44, ['t'] = 45, ['u'] = 46, ['v'] = 47,
    ['w'] = 48, ['x'] = 49, ['y'] = 50, ['z'] = 51, ['0'] = 52, ['1'] = 53,
    ['2'] = 54, ['3'] = 55, ['4'] = 56, ['5'] = 57, ['6'] = 58, ['7'] = 59,
    ['8'] = 60, ['9'] = 61, ['+'] = 62, ['/'] = 63};

size_t get_encoded_length(size_t length) {
  return (size_t)ceil((long double)length / 3.0) * 4;
}

int encode(char *output, char *input) {
  if (strlen(input) > 4294967295) {
    printf("Block too big. Maximum block size is 4294967295 characters.");
    return 1;
  }
  unsigned int block = 0;
  unsigned int length = (unsigned int)strlen(input);

  for (unsigned int i = 0; i < length; i += 3) {
    block = 0;
    for (unsigned int b = 0; b < 3; b++) {
      if (i + b < length) {
        block |= (unsigned int)input[i + b] << ((2 - b) * 8);
      }
    }
    for (unsigned int o = 0; o < 4; o++) {
      if (i + o > length) {
        output[(i / 3) * 4 + o] = 0x3d;
      } else {
        output[(i / 3) * 4 + o] = encoding_table[(block >> (3 - o) * 6) & 0x3f];
      }
    }
  }
  output[get_encoded_length(length)] = '\0';
  return 0;
}

int encode_unsigned_int(char *output, unsigned int *input, size_t size) {
  char *converted_input = (char *)calloc(size * 8, sizeof(char));
  for (unsigned int i = 0; i < size; i++) {
    for (unsigned int b = 0; b < 8; b++) {
      converted_input[i * 8 + b] = input[i] >> (24 - 8 * b) & 0xF;
    }
  }
  int status = encode(output, converted_input);
  free(converted_input);

  return status;
}

int decode(char *output, char *input) {
  if (strlen(input) > 4294967295) {
    printf("Block too big. Maximum block size is 4294967295 characters.");
    return 1;
  }
  unsigned int length = (unsigned int)strlen(input);
  if (length % 4 != 0) {
    printf("Block length is not dividable by 4. Maybe wrong padding?");
    return 2;
  }
  unsigned int block = 0;
  printf("%u\n", length);
  for (unsigned int i = 0; i < length; i += 4) {
    block = 0;
    for (unsigned int b = 0; b < 4; b++) {
      if (input[i + b] != '=') {
        block |= (unsigned int)decoding_table[(unsigned char)input[i + b]]
                 << ((3 - b) * 6);
      }
    }
    for (unsigned int o = 0; o < 3; o++) {
      output[(i / 4) * 3 + o] = (char)(block >> (2 - o) * 8) & 0xff;
    }
  }
  output[length / 4 * 3] = '\0';
  return 0;
}

int decode_unsigned_int(unsigned int *output, char *input) {
  char *decoded_input = (char *)calloc(strlen(input), sizeof(char));
  int status_code = decode(decoded_input, input);
  if (status_code != 0) {
    free(decoded_input);
    return status_code;
  }
  for (unsigned int i = 0; i < strlen(input); i++) {
    output[i / 4] |= (unsigned int)decoded_input[i] << (24 - i % 4 * 8);
  }
  return 0;
}

int main() {
  char *output = (char *)calloc(1024, sizeof(char));
  char *result = (char *)calloc(1024, sizeof(char));
  char input[] = "Man, I have to write a bit of text again, to check if this "
                 "base64 implementation can really handle input of 'unknown' "
                 "size. But I'm really confident ^^..";
  encode(output, input);
  printf("-> %s\n", output);
  decode(result, output);
  printf("-> %s\n", result);

  free(output);
  free(result);

  return 0;
}
