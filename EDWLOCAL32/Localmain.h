

#ifndef LOCAL_MAIN_DOT_H
#define LOCAL_MAIN_DOT_H


typedef struct 
{
  ULONG  index;
  ULONG   Offset;

}TABLE , *LPTABLE ;





const int HEAP_ALLOC_FAILURE   = -100;
const int TABLE_ALLOC_FAILURE  = -101;
const int STR_ALLOC_FAILURE    = -102;



///////////////////////////////////////////////////
//
//  
//
//
//
//
//


class  vStringList
{
  private:  // Attributes  
  
    FILE    *infile;     // Pointer to Above see buff.cpp
    HANDLE  HeapHandle;  // Handle To Heap
    LPSTR   StrBuffer;   // pointer to String
    LPTABLE indexer;     // Pointer To Table 
	int     table_allocated; // memory allocated for table
	int     table_consumed;  // memory consumed  for table 

	int     str_allocated;   // memory allocated for strings
	int     str_consumed;    // memort consumed  for strings


  private:  // operations 

    char * GetNextLine(char *);   // Get the Next Line from file

  public:    // Interface methods of class
  
 
   int AddString( ULONG number  , char *KeyString );
   
   // Build The String Table
   
   int BuildTree();
   
   // Get The pointer to message for parameter id
   
   LPSTR  GetMessage( ULONG message_id );


   // Add An Item in to indexer

    int AddItem( ULONG  code , ULONG offset );

   
    // Sort The items using C run time qsort 

    int SortTable();
	

#ifdef _TEST_DRIVER

	void DumpStat( LPSTR pFileName );

#endif



   //////////////////////////////////////////////
   // ctor 
   //
   //
   //

   vStringList(void) 
    { 
    
      infile = NULL;     // Set File Pointer To NULL


      //====== Create a Private Heap 
      HeapHandle = HeapCreate( 0 , 1048576*2 , 0 );

      if ( HeapHandle == NULL ) 
    	   throw HEAP_ALLOC_FAILURE;


	  //===== Allocate memory for Table 
      indexer = (LPTABLE) HeapAlloc( HeapHandle,
			HEAP_ZERO_MEMORY , ( 1024L*128L )  );
		
         
	  if ( indexer == NULL ) 
	       throw TABLE_ALLOC_FAILURE;

        
	  table_allocated = 1024L*128L;
	  table_consumed  = 0;

      
	  //============= Allocate memory for string 
	  StrBuffer = (LPSTR) HeapAlloc( HeapHandle,
	 		HEAP_ZERO_MEMORY , ( 1024L*1024L )  );
		         

       
	  if ( StrBuffer == NULL )
	     throw STR_ALLOC_FAILURE;


      str_allocated = 1024L*1024L;
	  str_consumed  = 0;

		      
    };
    
    
   // Worker method to Build the String Tree 
   // accepts one parameter , Name of The File 
       
    
    BOOL  Read(char *pFileName)
     {
  
       
        
      

   
       if (( infile = fopen( pFileName ,"r") ) == NULL )
           
            {
                   
               
                   
                 return  0;
                       
             }          
                       
           
         
         if ( BuildTree() == 0 ) 
           {
               return 0; 
             
           }      
                     
             
         
          return 1;
             
             
             

    };    
    
    
    ~vStringList()    //dtor
     {   
                        
        HeapFree(HeapHandle , 0 , indexer );
		HeapFree(HeapHandle , 0 , StrBuffer );
        HeapDestroy( HeapHandle );


		if ( infile != NULL )
		{
			fclose(infile);
        }


                
     };        
                


 };
 
 

#endif
