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
	int line = 0;


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

		if (in_dict(filter, seq->seq.s)) continue;
		insert_word(filter, seq->seq.s);

		printf("%c%s", seq->qual.l == seq->seq.l? '@' : '>', seq->name.s);
		if (seq->comment.l) printf(" %s", seq->comment.s);
		putchar('\n');
		for (i = 0; i < l; ++i) {
			putchar(seq->seq.s[i]);
		}
		putchar('\n');
		if (seq->qual.l != seq->seq.l) continue;
		printf("+");
		for (i = 0; i < l; ++i) {
			if (i == 0 || (line > 0 && i % line == 0)) putchar('\n');
			putchar(seq->qual.s[i]);
		}
		putchar('\n');
	}
	kseq_destroy(seq);
	gzclose(fp);
	return 0;
}
