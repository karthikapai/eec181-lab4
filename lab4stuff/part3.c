#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>

#include "rt_nonfinite.h"
#include "test1.h"
#include "rand.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

//include vector header file
#include "vector.h"

#define HW_BASE 0xc0000000
#define HW_SPAN 0x40000000
#define HW_MASK (HW_SPAN - 1)
#define I_W 512*512
//#define O_W 510*510


/*static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}*/


void test1(const unsigned char lenna_gray[I_W], unsigned char D[I_W])
{
  //static double unusedExpr[I_W];
  int row;
  int col;
  double sumx;
  double sumy;
  int i;
  int j;
  static const signed char Gx[9] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

  static const signed char Gy[9] = { 1, 0, -1, 2, 0, -2, 1, 0, -1 };

  unsigned char u0;

  for (row = 0; row < 512; row++) {
    for (col = 0; col < 512; col++) {
      sumx = 0.0;
      sumy = 0.0;
      for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
          sumx += (double)(lenna_gray[(row + i) + 512 * (col + j)] * Gx[i + 3 *
                           j]);
          sumy += (double)(lenna_gray[(row + i) + 512 * (col + j)] * Gy[i + 3 *
                           j]);
        }
      }

     // sumx = rt_roundd_snf(fabs(sumx) + fabs(sumy));
      u0 = fabs(sumx) + fabs(sumy);

     /*if (sumx < 256.0) {
        u0 = (unsigned char)sumx;
      } else {
        u0 = MAX_uint8_T;
      } */

      D[row + 512 * col] = u0;
    }
  }
}

 
void print_array(unsigned char* array, int MAX_ELEMENT)
{
	FILE* fp;
	unsigned char output[] = "hpsarray.txt";
  	int i;
	fp = fopen(output,"w+");
	for (i=0; i < MAX_ELEMENT; i++)
	{
	  fprintf(fp, "%u,", array[i]);
	}
	fclose(fp);
}


/*int delay(unsigned int mseconds)
{
  clock_t goal = mseconds + clock();
  while (goal > clock());
}*/

int main(){
  void *VA, *led_ptr, *sdram; 
  int fd;
  int result;
  unsigned int data = 0;
  int i;
  int j;
  int m;
  int n;
  int a;
  clock_t t;
  t = clock();
  
  unsigned char D_vector[I_W];
  unsigned char R_vector[I_W];
  int w = 0;
  int MAX_ELEMENT = I_W;

  if ((fd = open("/dev/mem", (O_RDWR|O_SYNC))) == -1){
    printf("ERROR: could not open \"/dev/mem\"...\n");
    return  (1);
  }

  VA = mmap(NULL, HW_SPAN, (PROT_READ|PROT_WRITE), MAP_SHARED, fd, HW_BASE);

  if (VA == MAP_FAILED){
    printf("ERROR: mmap() failed... \n");
    close(fd);
    return (1);
  }

  //led_ptr = VA + ((unsigned long)(0xff200000 + 0x80) & (unsigned long)(HW_MASK));
  sdram = VA + ((unsigned long)(HW_BASE + 0x00) & (unsigned long)(HW_MASK));
     
  //****************Read from vector and write into sdram***********

  for (m = 0; m < I_W; m++){
    ((unsigned char*)sdram)[m] = vector[m];
  }
  //********************************************

 // printf("sdram is %u\n ", ((unsigned char*)sdram)[1] );
 
 // t = clock() - t;
 // double time_taken = ((double)t)/CLOCKS_PER_SEC;
  //***************Read from sdram write into a vector*****************
  for (n = 0; n < I_W; n++){
    R_vector[n] = ((unsigned char*)sdram)[n];
  }

  //printf("R_vector is %u\n", R_vector[1]);

  test1(R_vector, D_vector);

  for (a = 0; a < I_W; a++){
    ((unsigned char*)sdram)[a] = D_vector[a]; 
  }

  //for (i = 0; i < 10; i++){
  //  printf("RD_vector is %u\n", D_vector[i]);
 // }
  //*((unsigned char*)sdram) = D_vector[MAX_ELEMENT];
  
  t = clock() - t;

  double time_taken = ((double)t)/CLOCKS_PER_SEC;

  printf("progess time is %f seconds \n", time_taken);
	
  print_array(D_vector, MAX_ELEMENT);

  if(munmap(VA, HW_SPAN) != 0){
    printf("ERROR: munmap() failed... \n");
    close(fd);
    return 1;
  }

  close(fd);
  return 0;
}
