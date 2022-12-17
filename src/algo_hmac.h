#ifndef _ALGO_HMAC_H_
#define _ALGO_HMAC_H_

int hmac_encode(const char * algo,
                const char * key, unsigned int key_length,
                const char * input, unsigned int input_length,
                unsigned char * &output, unsigned int &output_length);

#endif
