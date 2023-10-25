/**************************************************************************/
/*                                                                        */
/*  Reads in a binary file, and converts each four chars into             */
/*    a float, after each N floats the floats and a newline are printed.  */
/*                                                                        */
/*  usage:  convert inputFilename floats-per-line(record)                 */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/**************************************************************************/
#include <stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <string.h>

#define BUFSIZE 100 

int rp1, wp ;
int NUM_FLOAT_PER_RECORD ;





int b2a_fun(int argc, char *argv[])
{

  /* char *theRecord ; */
  char theRecord[10] ;
  char *inputFile ;
  char from, to ;
  int returnVal ;
  int numRecs ;
  int i ;
  int offset ;
  float tempFloat ;


  if (argc < 3)
  { 
    printf("usage: %s filename numFloats_per_record \n", argv[0]) ; 
    exit(1) ; 
  }

  inputFile = argv[1] ;
  NUM_FLOAT_PER_RECORD = atoi(argv[2]) ;

  /* from = (char) *argv[2] ; */
  /* to = (char) *argv[3] ; */
  /* numRecs = atoi(argv[4]) ; */

  /* inputFile = "in1" ; */
  /* from = 'b' ; */
  /* to = 'a' ; */
  /* numRecs = 100 ; */

  /* printf("Inputs: %s %c %c %d\n",inputFile, from, to, numRecs); */


  if ((rp1=open(inputFile,O_RDONLY,0))==-1)
  {
	printf("cannot open %s\n", inputFile);
	exit(1);
  }

  /* if ((from != 'b') || (to != 'a')) */
  /* { */
    /* printf("ERROR, program only conversts from binary to ascii\n") ; */
    /* exit(1) ; */
  /* } */

  /* theRecord = (char *) malloc(BUFSIZE) ; */

  returnVal = 4 ;
  offset = 0 ;
  i = 0 ;
  while (returnVal == 4)
  {
    returnVal = lseek(rp1, offset, SEEK_SET) ;
    returnVal = read(rp1, &theRecord, sizeof(float) ) ;
    if (returnVal < sizeof(float))
      exit(1) ;
    /* printf("%12.5f ", *(float *) theRecord) ; */
    printf("%12.8f ", *(float *) theRecord) ;
    offset += 4 ;
    if ( ((i++ + 1) % NUM_FLOAT_PER_RECORD) == 0)
    {
      printf("\n") ;
      i = 0 ;
    }
  }

  printf("\n") ;

  close(rp1) ;

}






