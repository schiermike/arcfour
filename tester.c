#include "rc4.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char* read_key(char* keyarg, unsigned int* keylen) {
	*keylen = strlen(keyarg);
	if ((*keylen+1)%3 != 0) {
		fprintf(stderr, "Invalid hex key length!\n");
		exit(-1);
	}
	*keylen = (*keylen+1)/3;
	unsigned char* key = (unsigned char*)(malloc(*keylen));

	int i;
	unsigned int t;
	for (i=0; i<*keylen; i++) {
		if (i+1!=*keylen && keyarg[3*i+2]!='-' || sscanf(keyarg + 3*i, "%x-", &t) != 1 || t>255) {
			fprintf(stderr, "Invalid hex key structure!\n");
			exit(-1);
		}
		key[i] = (unsigned char)t;
	}

	return key;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: rc4crypt hexbyte0-hexbyte1-...-hexbyten\n");
		fprintf(stderr, "Example: rc4crypt 44-fa-3f-33-25\n");
		return -1;
	}
	unsigned int keylen;
	unsigned char* key = read_key(argv[1], &keylen);

	rc4_state state;
	rc4_init(&state, key, keylen);

	unsigned char buffer[1024];
	int len;
	
	int has_input = 0;
	while ((len = fread(buffer, 1, 1024, stdin)) > 0) {
		rc4_crypt(&state, buffer, len);
		fwrite(buffer, 1, len, stdout);
		has_input = 1;
	}

	if (!has_input) {
		printf("Hex Bitstream: ");
		int i;
		for (i=0; i<30; i++) {
			unsigned char c;
			rc4_crypt(&state, &c, 1);
			printf("%02x ", c);
		}
		printf("\n");
	}

	return 0;
}

