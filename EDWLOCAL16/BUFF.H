
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#define PNULL 0
#define EOF   (-1)
#define BUFFER_SIZE 4096


typedef struct 
 {
   int     Count;             // Characters left in the Buffer
   char    Ptr[4096];         // Pointer To Locked handle
   int     EoF;               // flag
   HFILE   file_handle;       // file_handle
   int     max_read;          // number of Characters read
   long    FileSize;          // Size of the File
 }P_FILE;

 #ifdef __cplusplus
  extern "C" {
 #endif

 P_FILE *pfopen( char *FileName );
 int pfclose( P_FILE *pfile );
 int pfgetc( P_FILE *pfile );
 int pfgets( char *buff , int ncount , P_FILE *file );

 #ifdef __cplusplus
   }
 #endif

