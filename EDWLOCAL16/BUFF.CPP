
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "buff.h"

// Analogous to fopen from standard c library

extern "C"  P_FILE *pfopen( char *FileName )
  {
    P_FILE *memory = ( P_FILE * ) malloc( sizeof( P_FILE ) );

       if ( memory == NULL )
           return NULL;


     memory->file_handle = _lopen( FileName , OF_READWRITE ); 
     
     

     if ( memory->file_handle == HFILE_ERROR )
        {
               free( memory );
               return NULL;
        }
        
        

        memory->FileSize = _llseek( memory->file_handle , 0L , 2 );
        _llseek( memory->file_handle , 0 , 0 );
        memory->Count = 0;
        memory->EoF   = 0;
        memory->max_read = 0; 
        return memory;


   }

   extern "C" int pfclose( P_FILE *pfile )
    {
       if ( pfile == NULL )
           return 0;

       _lclose( pfile->file_handle );

       free( pfile );
       pfile = NULL;
       return 1;
    }



    extern "C" int pfgetc( P_FILE *pfile )
     {
        if ( pfile->EoF == 1 )
            return -1;


        if ( pfile->Count == 0 ||
             pfile->Count == pfile->max_read )
         {
            pfile->max_read = _lread( pfile->file_handle ,
                                      pfile->Ptr , 4096 );

            if ( pfile->max_read == 0 )
               {
                  pfile->EoF = 1;
                  return -1;
               }

              pfile->Count = 0;
         }


        return   (int) pfile->Ptr[ pfile->Count++ ];

       }


       extern "C" int pfgets( char *buff , int ncount , P_FILE *file )
        {
             static int  old_left = 0;
             int index = 0;
             int c;


             if (file->EoF == 1 )
                return -1;

             buff[ 0 ] = 0;

             if ( old_left != 0 )
              {
                buff[ index++ ] = (char)old_left;
                old_left =0;
              }


             while (1) {
               c = pfgetc( file );

               if ( c == -1 )
                {
                  buff[ index ] = 0;
                  return 1;
                }

              else if ( c == 13 )
                {
                  c = pfgetc( file );

                  if ( c != 10 )
                   {
                      old_left = c;

                   }
                   
                   buff[ index++ ] = 0;
                   return 1;
                }
                  
              else if ( c == 10 ) {
                 
                  buff[ index++ ] =0;
                  return 1;
                }
              else
                  buff[ index++ ] = (char)c;

             }

             return 0;

           }

