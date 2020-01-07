#include <zlib.h>
#include <stdio.h>
#include "kseq.h"
#include "bloom.h"

KSEQ_INIT(gzFile, gzread)

/* config options */
/* 2^FILTER_SIZE is the size of the filter in bits, i.e.,  
 * size 20 = 2^20 bits = 1 048 576 bits = 131 072 bytes = 128 KB */
#define FILTER_SIZE 20
#define NUM_HASHES 7
#define WORD_BUF_SIZE 32

#define FILTER_SIZE_BYTES (1 << (FILTER_SIZE - 3))
#define FILTER_BITMASK ((1 << FILTER_SIZE) - 1)

int main(int argc, char *argv[])
{
	gzFile fp;
	kseq_t *seq;
	int i, l;
	unsigned char filter[FILTER_SIZE_BYTES];


	if (argc == 1) {
		fprintf(stderr, "Usage: %s <in.seq>\n", argv[0]);
		return 1;
	}

	for (i = 0; i < FILTER_SIZE_BYTES; i++){
		filter[i] = 0;
	}

	fp = gzopen(argv[1], "r");
	seq = kseq_init(fp);
	while ((l = kseq_read(seq)) >= 0) {
		printf("name: %s\n", seq->name.s);
		if (seq->comment.l) printf("comment: %s\n", seq->comment.s);
		printf(" seq: %s\n", seq->seq.s);
		if (seq->qual.l) printf("qual: %s\n", seq->qual.s);
	}
	printf("return value: %d\n", l);
	kseq_destroy(seq);
	gzclose(fp);
	return 0;
}
