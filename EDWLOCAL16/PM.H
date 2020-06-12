
#ifndef _MEM_DOT_H
#define _MEM_DOT_H

#include <windows.h>
#include <stdio.h>

typedef struct
 {
   unsigned short msg_id;
   unsigned char far *ptr;
 }AppArray;
 

 typedef struct
  {
    HGLOBAL *GlobalHandle; 
    unsigned char far *g_ptr;
    long num_chars;
    long allocated;
    int  index_array;
  }StrArray;


  class pAppData
 {
   public:
   
   HGLOBAL    ArrayHandle;   //Handle To Global memory
   AppArray   *a_ptr;        //Pointer To Array 
   long num_elems;           //number of elements
   long curr_max;            //maximum which current buffer can hold
   int  obj_created;         //whether all is fine 
   
  
   public:
      
   pAppData();
   ~pAppData();
   int AddItem( unsigned short id , unsigned char far *offset );
   char far * GetStringPtr(unsigned short data );
   
   void DumpData();  // For Diagnostics purpose
   
   
 };



        
        


#endif


