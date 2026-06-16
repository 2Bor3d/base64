#include "base64.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  int test_result = 0;
  char *output = (char *)calloc(1024, sizeof(char));
  char *result = (char *)calloc(1024, sizeof(char));
  char input[] = "Man, I have to write a bit of text again, to check if this "
                 "base64 implementation can really handle input of 'unknown' "
                 "size. But I'm really confident ^^";
  char expected_encoded[] =
      "TWFuLCBJIGhhdmUgdG8gd3JpdGUgYSBiaXQgb2YgdGV4dCBhZ2FpbiwgdG8gY2hlY2sgaWYg"
      "dGhpcyBiYXNlNjQgaW1wbGVtZW50YXRpb24gY2FuIHJlYWxseSBoYW5kbGUgaW5wdXQgb2Yg"
      "J3Vua25vd24nIHNpemUuIEJ1dCBJJ20gcmVhbGx5IGNvbmZpZGVudCBeXg==";
  b64_encode(output, input);
  if (strcmp(expected_encoded, output)) {
    printf("Encoding error\n");
    test_result += 1;
  } else {
    printf("Encoding passed\n");
  }

  b64_decode(result, expected_encoded);
  if (strcmp(input, result)) {
    printf("Decoding error\n");
    test_result += 2;
  } else {
    printf("Decoding passed\n");
  }

  free(output);
  free(result);
  return test_result;
}
