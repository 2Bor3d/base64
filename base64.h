#ifndef BASE64_H
#define BASE64_H

size_t get_encoded_length(size_t length);

int encode_unsigned_int(char *output, unsigned int *input, size_t size);

int decode(char *output, char *input);

int decode_unsigned_int(unsigned int *output, char *input);

#endif
