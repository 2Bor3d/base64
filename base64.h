#ifndef BASE64_H
#define BASE64_H

size_t b64_get_encoded_length(size_t length);

int b64_encode(char *output, char *input);

int b64_encode_unsigned_int(char *output, unsigned int *input, size_t size);

int b64_decode(char *output, char *input);

int b64_decode_unsigned_int(unsigned int *output, char *input);

#endif
