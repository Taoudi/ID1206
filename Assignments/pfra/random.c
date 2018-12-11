#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#define HIGH 20
#define FREQ 80
#define SAMPLES 20
#define PAGES 100

typedef struct pte {
	int present;
} pte;

void init(int *sequence, int refs, int pages) {

	int high = (int) (pages *((float) HIGH / 100));

	for(int i = 0; i < refs; i++) {
		if(rand() % 100 < FREQ) {
			/* the frequency case */
			sequence[i] = rand() % high;
		} else {
			/* the less frequently case */
			sequence[i] = high + rand() % (pages - high);
		}
	}
}
void clear_page_table(pte *page_table, int pages) {
	for(int i = 0; i < pages; i++) {
		page_table[i].present = 0;
	}
}
int simulate(int *seq, pte *table, int refs, int frms, int pgs) {

	int hits = 0;
	int allocated = 0;

	int i;

	for(i = 0; i < refs; i++) {
		int next = seq[i];
		pte *entry = &table[next];

		if(entry -> present == 1) {
			hits++;
		} else {
			if(allocated < frms) {
				allocated++;
				entry -> present = 1;
			} else {
				pte *evict;
				do {
					int rnd = rand() % pgs;
					evict = &table[rnd];
				} while(evict -> present != 1);

				evict -> present = 0;
				evict -> present = 1;
			}
		}
	}
	return hits;
}

int main(int argc, char *argv[]) {
	/* could be command line arguments */
	int refs = 10;
	int pages = 100;
  //pte table[PAGES];
  pte *table = (pte *)malloc(pages*sizeof(pte));
 int *sequence=(int*)malloc(refs*sizeof(int));
init(sequence, refs, pages);

/* compute a increment, the number of pages divided by the number
	of data pints, and then run the simulation for a growing number
	of frames. */

	printf("# This is a benchmark of random replacement\n");
	printf("# %d page references\n", refs);
	printf("# %d pages \n", pages);
	printf("#\n# frames\tratio\n");

	/* frames is the size of the memory in frames */
	int frames;

	int incr = pages / SAMPLES;

	for(frames = incr; frames <= pages; frames += incr) {
		/* clear page tables entries */
		clear_page_table(table, pages);

		int hits = simulate(sequence, table, refs, frames, pages);

		float ratio = (float) hits / refs;

		printf("%d\t%.2f\n", frames, ratio);
}
return 0;
}
