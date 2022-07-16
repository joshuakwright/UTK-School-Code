#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"

void double_check_memory(int **ptrs, int *dc, int nptrs, int fl_size, int tbytes)
{
  void *low, *l;
  void *high, *h;
  int *ip, i;
  int nbytes;
  int nfl;

  nbytes = 0;

  for (i = 0; i < nptrs; i++) {
    l = (void *) ptrs[i];
    l -= 8;
    ip = (int *) l;

    if (*ip != dc[i]) {
      printf("Error: pointer number %d the wrong size (%d instead of %d)\n", i, *ip, dc[i]);
      exit(1);
    }
    h = l + *ip;
    if (nbytes == 0 || l < low) low = l;
    if (nbytes == 0 || h > high) high = h;
    nbytes += *ip;
  }
    
  nfl = 0;
  for (l = free_list_begin(); l != NULL; l = free_list_next(l)) {
    ip = (int *) l;
    h = l + *ip;
    if (nbytes == 0 || l < low) low = l;
    if (nbytes == 0 || h > high) high = h;
    nbytes += *ip;
    nfl++;
  }

  if (nbytes != tbytes) {
    printf("Error: Total bytes allocated and on the free list = %d, not %d\n", nbytes, tbytes);
    exit(0);
  }
    
  if (high - low != tbytes) {
    printf("Error: Highest address (0x%x) minus lowest (0x%x) does not equal %d\n", (int) high, (int) low, tbytes);
    exit(0);
  }

  if (nfl != fl_size && fl_size != -1) {
    printf("Error: %d nodes on the free list -- should be %d\n", nfl, fl_size);
    exit(0);
  }
}


int main()
{
  int *ptrs[27];
  int *free_ptrs[27];
  int dc[27];

  ptrs[13] = my_malloc(149); dc[13] = 160;
  ptrs[22] = my_malloc(128); dc[22] = 136;
  ptrs[25] = my_malloc(136); dc[25] = 144;
  free_ptrs[17] = my_malloc(82);
  free_ptrs[15] = my_malloc(94);
  free_ptrs[6] = my_malloc(80);
  free_ptrs[18] = my_malloc(83);
  free_ptrs[2] = my_malloc(75);
  free_ptrs[21] = my_malloc(94);
  ptrs[19] = my_malloc(131); dc[19] = 144;
  free_ptrs[8] = my_malloc(82);
  free_ptrs[14] = my_malloc(92);
  free_ptrs[5] = my_malloc(94);
  free_ptrs[0] = my_malloc(90);
  free_ptrs[19] = my_malloc(91);
  ptrs[21] = my_malloc(137); dc[21] = 152;
  ptrs[20] = my_malloc(132); dc[20] = 144;
  free_ptrs[1] = my_malloc(82);
  free_ptrs[9] = my_malloc(73);
  free_ptrs[24] = my_malloc(87);
  ptrs[3] = my_malloc(150); dc[3] = 160;
  ptrs[0] = my_malloc(124); dc[0] = 136;
  free_ptrs[10] = my_malloc(81);
  free_ptrs[20] = my_malloc(87);
  ptrs[24] = my_malloc(145); dc[24] = 160;
  free_ptrs[26] = my_malloc(76);
  ptrs[7] = my_malloc(143); dc[7] = 152;
  free_ptrs[23] = my_malloc(82);
  free_ptrs[11] = my_malloc(92);
  ptrs[1] = my_malloc(144); dc[1] = 152;
  free_ptrs[12] = my_malloc(81);
  free_ptrs[16] = my_malloc(76);
  free_ptrs[3] = my_malloc(76);
  free_ptrs[13] = my_malloc(84);
  ptrs[9] = my_malloc(137); dc[9] = 152;
  free_ptrs[7] = my_malloc(95);
  free_ptrs[25] = my_malloc(74);
  free_ptrs[22] = my_malloc(93);
  ptrs[2] = my_malloc(124); dc[2] = 136;
  free_ptrs[4] = my_malloc(77);
  my_free(free_ptrs[23]);
  my_free(free_ptrs[2]);
  my_free(free_ptrs[22]);
  my_free(free_ptrs[5]);
  ptrs[6] = my_malloc(137); dc[6] = 152;
  my_free(free_ptrs[21]);
  my_free(free_ptrs[11]);
  my_free(free_ptrs[16]);
  my_free(free_ptrs[6]);
  my_free(free_ptrs[1]);
  my_free(free_ptrs[12]);
  my_free(free_ptrs[20]);
  ptrs[12] = my_malloc(116); dc[12] = 128;
  my_free(free_ptrs[15]);
  ptrs[4] = my_malloc(134); dc[4] = 144;
  ptrs[17] = my_malloc(113); dc[17] = 128;
  my_free(free_ptrs[25]);
  ptrs[10] = my_malloc(119); dc[10] = 128;
  my_free(free_ptrs[17]);
  ptrs[16] = my_malloc(123); dc[16] = 136;
  my_free(free_ptrs[9]);
  ptrs[14] = my_malloc(118); dc[14] = 128;
  my_free(free_ptrs[7]);
  my_free(free_ptrs[13]);
  my_free(free_ptrs[26]);
  ptrs[23] = my_malloc(118); dc[23] = 128;
  my_free(free_ptrs[4]);
  ptrs[18] = my_malloc(134); dc[18] = 144;
  my_free(free_ptrs[24]);
  my_free(free_ptrs[10]);
  ptrs[8] = my_malloc(143); dc[8] = 152;
  ptrs[11] = my_malloc(122); dc[11] = 136;
  ptrs[15] = my_malloc(118); dc[15] = 128;
  my_free(free_ptrs[8]);
  ptrs[5] = my_malloc(145); dc[5] = 160;
  ptrs[26] = my_malloc(143); dc[26] = 152;
  my_free(free_ptrs[18]);
  my_free(free_ptrs[19]);
  my_free(free_ptrs[14]);
  my_free(free_ptrs[3]);
  my_free(free_ptrs[0]);

  coalesce_free_list();

  double_check_memory(ptrs, dc, 27, 10, 8192);
  printf("Correct\n");
  return 0;
}
