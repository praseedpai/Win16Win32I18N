//////////////////////////////////////////////////////////////
//  Routines To Handle Edwin Internationalisation
//   
//
//  Written By     :=           PRASEED PAI KT 
//                              Visionics INDIA PVT LTD 
//  Date           :=           08/12/1997 
//
////////////////////////////////////////////////////////////// 



#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "localmain.h"


#ifdef _TEST_DRIVER 

#define DEXEC(x)  (x)

#else

#define DEXEC(x)  

#endif


#ifdef _TEST_DRIVER

  FILE *npl;

#endif





///////////////////////////////////////////////////////////////////////
// Routine to Peel off String from both sides
// analogous to RTRIM$( LTRIM$( <param> ) ) in VB
// or Xbase Function by Same Name
//  Param 1 
//       Address of  Source String
//  Param 2
//       Address Where result has to be copied


char *alltrim( char *SrcString , char *ret_val )
 {      
   
   int empty_str=0;                    
   LPSTR  iterator;
   LPSTR  FirstPtr =  SrcString;
   
   
   /*************** Skip White Spaces *****************/

   while ( *FirstPtr == ' ' || *FirstPtr == '\t' )
              FirstPtr=CharNext( FirstPtr);
              

   
               if ( *FirstPtr == 0 )
               {
                  ret_val[0]=0;
                  return ret_val;
               }   
                  
                   
   /************** Find out the End of String ****************/
   
          iterator = FirstPtr;
          
          while ( *iterator != 0 ) 
            iterator = CharNext( iterator);
              
               
     char *TailPtr = CharPrev(FirstPtr , iterator);

   /******** Find first non white space character by Scanning backwards ***/

      while ( (*TailPtr == ' ' || *TailPtr == '\t'))
               TailPtr = CharPrev(FirstPtr , TailPtr);
         

    /******************* forcibly terminate with null **************/
                        
      *( CharNext(TailPtr)) = 0;     
      
       
      
      lstrcpy( ret_val , FirstPtr );  // copy the string to return buffer
      
      return ret_val;

  }                                                     
  
///////////////////////////////////////////////////////////////
//
//  DBCS aware Version of strchr ; since character to be scanned
//  can be a trail byte , C run  time library functions wont work
//  correctly 
//
//          

char *p_strchr( char *src_string , int c ) 
 {
   char *iterator = src_string;
   
   
   while ( *iterator != 0 )
    {
        
        if ( *iterator == (char)c )
         {
             return iterator;
         }      
         
        iterator = CharNext(iterator );
        
    }
        
    return NULL;
    
 }

/////////////////////////////////////////////
//
//
//  Function to be passed to binary Search 
//
//
//

int __cdecl CmpFn( const void *arg1,
				   const void *arg2 )
{
  
	return ( ((LPTABLE)arg1)->index) -
		   ( ((LPTABLE)arg2)->index);

  
}

///////////////////////////////////////////
//  
// Implementation of vStringList methods 
//
//
//
//
//
//
//
//
//
//
//



int vStringList::BuildTree()
 {

   char Buffer[1000];                
   char re_buffer[1000];
   char CurrToken[50];   
   char ret_buffer[100];
   char far *Temp;


   start_read:

    
     /*********** IF EOF close the input file and return ***********/
     
     if ( GetNextLine(Buffer) ==NULL )
      {
         return 1;
      }


      Temp = (char far * ) Buffer; 

      while ( *Temp == ' ' || *Temp =='\t'  ) 
                 Temp = CharNext(Temp);


      if (!( *Temp >= '0' && *Temp <= '9' ) )
             goto start_read;     
             
             

      char *TempPtr = p_strchr( Temp , '~' );

      if ( TempPtr == NULL )
           goto start_read;
           
       
 
       
         
       

      *TempPtr++ = 0;


      lstrcpy( CurrToken , Temp );

      

      alltrim( CurrToken , ret_buffer );

 

      char *m_ptr = p_strchr( TempPtr , '~' );

   
      
      if ( m_ptr == NULL )
         goto start_read; 
         
         
       

      *m_ptr-- =0;

      
      
      
      alltrim( TempPtr , re_buffer );  
      

      
      if ( *re_buffer == 0 )
          goto start_read;


#ifdef _TEST_DRIVER
	  fprintf(npl , "Value %s #%d\n",ret_buffer, atol(ret_buffer) );

#endif



          
            
      if (AddString( (ULONG) atol(ret_buffer) , re_buffer ) == 0 )
               return 0; 
                

      goto start_read;

      return 1;



    }


 
   
char *vStringList::GetNextLine( char *NextLineBuffer )
   {
     
     char *n = 
     fgets( NextLineBuffer , 500 , infile );       
     
     return n;
     
     
   }

          



        




      
    
 int vStringList::AddString( ULONG id_string ,
                             char *message )
  {
      int s= lstrlen( message ) +1;
         
         
       if ( str_consumed + s >= (str_allocated-10) )
       {

           StrBuffer = ( LPSTR ) HeapReAlloc(
           HeapHandle , 0 , 
           StrBuffer , 
	   	   str_allocated + 100L*1024L );

		   if ( StrBuffer == NULL ) {
 			       #ifdef _TEST_DRIVER
            		 char buff[100];
		             sprintf( buff,"%s"," S Realloc" );
		             MessageBox(0,buff,"Error", 1);
                   #endif

			   return 0;
		   }

	   
	       str_allocated += ( 100L*1024L );
	   
	   }         
               
                
          
       LPSTR old_ptr = ( StrBuffer + str_consumed );
		 
              
       lstrcpy(old_ptr , 
			 ( *message == ' ' ) ? message+1 : message );

         
       		 
                 
         

                   
 
         if ( AddItem( id_string , str_consumed ) == 0 )
                      return 0; 
              
         str_consumed += s;     

         return 1;
             
         

   }


    int   vStringList::AddItem( ULONG code , ULONG offset )               

	{
        
		int ar_index = ( table_consumed / sizeof( TABLE ) );

        indexer[ ar_index ].index = code;
		indexer[ ar_index ].Offset = offset;
		
		table_consumed += sizeof(TABLE);


		if ( table_consumed == table_allocated  )
		{  

           #ifdef _TEST_DRIVER
     		 char buff[100];
	    	 sprintf( buff,"%d %d", table_consumed , table_allocated);
		     MessageBox(0,buff,"Error", 1);
           #endif

		   indexer = ( LPTABLE ) HeapReAlloc(
           HeapHandle ,0 , 
           (LPVOID)indexer , 
	   	   table_allocated + 60000L );


		   if ( indexer == NULL )
		   {

	       #ifdef _TEST_DRIVER
     		 char nbuff[100];
	    	 sprintf( nbuff,"%s", " T Realloc");
		     MessageBox(0,nbuff,"Error", 1);
           #endif

               return 0;
		   }	   
	   
	       table_allocated +=  60000L;
	   
        }
		
		return 1;

	}





	int vStringList::SortTable()
	{

      
	   
       // Remove unwanted memory 

       StrBuffer = (LPSTR) HeapReAlloc( HeapHandle , 
		   0 , StrBuffer , str_consumed  );


	    if ( StrBuffer == NULL ) {
	    #ifdef _TEST_DRIVER
  		 char buff[100];
		 sprintf( buff,"%s", "S Last " );
		 MessageBox(0,buff,"Error", 1);
        #endif

         return 0;
		}


       // remove unwanted memory

	   indexer = (LPTABLE) HeapReAlloc( HeapHandle , 
		   0 , (LPVOID)indexer , table_consumed  );



	    if ( indexer == NULL )
		{
    	    #ifdef _TEST_DRIVER
	    	 char buff[100];
		     sprintf( buff,"%s %d  %d", " T Last", table_allocated , table_consumed );
			 MessageBox(0,buff,"Error", 1);
            #endif
 
           return 0;
		}
       // Sort The Array
	   
	    qsort( (LPVOID)indexer ,
		    (table_consumed)/sizeof(TABLE),
			sizeof(TABLE), 
			CmpFn );

 

			
			
       DEXEC(DumpStat( "E:\\TEST.TXT" ));        




       return 1;
    }


/////////////////////////////////////////////////
//
//
//
	
#ifdef _TEST_DRIVER

void vStringList::DumpStat( LPSTR pFileName )
{

  FILE *nfile;

     
	
     nfile = fopen(pFileName , "w" );

	 if ( nfile == NULL )
		 return;

      
	 fprintf( nfile , 
		 "Allocated for Table %ld %d\n", 
	  	 table_consumed , sizeof(TABLE));


     fprintf( nfile ,
		 "Allocated For String %ld\n" ,
		 str_consumed );


     
     int t_index = (table_consumed /sizeof(TABLE) );
	 
	 for(int rval = 0 ; rval < t_index ; ++rval )
	 {
		 fprintf( nfile ,"index = %d   String>#%d %s\n",indexer[rval].index, rval , ( StrBuffer + indexer[ rval].Offset )  );

     }
	 
    


	 fclose(nfile);





}

#endif

///////////////////////////////////////////////
//
//
//
//

	LPSTR  vStringList::GetMessage( ULONG message_id )
    {

    #ifdef _TEST_DRIVER
		 char buff[100];
		 sprintf( buff,"%d", message_id );
		 MessageBox(0,buff,"Error", 1);
    #endif
		 
		
     LPTABLE m_rp = 
	  (LPTABLE)	bsearch(&message_id, (LPVOID)indexer ,
		    (table_consumed)/sizeof(TABLE),
			sizeof(TABLE), 
			CmpFn );

	    if ( m_rp == NULL ) {

		 #ifdef _TEST_DRIVER
		 
		 sprintf( buff,"%d", message_id );
		 MessageBox(0,buff,"Error", 1);
        #endif
	
			
			
			return NULL;


        }
		

     
     return ( StrBuffer + m_rp->Offset );

	}


//////////////////////////////////////////////
//
//
//
//
vStringList *glb_ptr;

////////////////////////////////////////
//
//  Dll Entry Point 
//
	
extern "C" __declspec(dllexport) BOOL   WINAPI   DllMain (HANDLE hInst, 
                        ULONG ul_reason_for_call,
                        LPVOID lpReserved)

  {

       switch( ul_reason_for_call )
        {
         case  DLL_PROCESS_ATTACH:
			   glb_ptr=0;
               break;

         case  DLL_THREAD_ATTACH : 
               break;
         case  DLL_PROCESS_DETACH:
 	       break;
         case  DLL_THREAD_DETACH : 
              break;
 	}         


       return ( TRUE ); 


  }

  


   
        
/////////////////////////////////
//
//
//
//
//


 extern "C" __declspec(dllexport)  BOOL  
__stdcall L32_Object_Create( LPSTR  FileName )
{


	 if ( FileName == 0 ||
		  strlen(FileName)==0)
		  return FALSE;
	 
	 if (glb_ptr ) {
		 delete glb_ptr;
	 }
	    

	    vStringList *TempPtr = new vStringList;

        if ( TempPtr == NULL )
			 return FALSE;

#ifdef _TEST_DRIVER
		npl = fopen("E:\\dmp.TXT" , "w" );
#endif


          
          if ( !( TempPtr->Read( FileName ) ) )
		  {
		      	  
			  DEXEC(MessageBox(0,"Failed read"," ",1));

			  delete TempPtr;
			  return FALSE;
          }

#ifdef _TEST_DRIVER
   fclose(npl);
#endif


		

   if ( TempPtr->SortTable() == 0 )
   {
        delete TempPtr;
		 return FALSE;;
   }
   
     glb_ptr= TempPtr;
	  return TRUE;
}


//////////////////////////////////////////////////////////
//
//
//
//
//

extern "C" __declspec(dllexport)  long   
__stdcall L32_Object_Delete(   )
{
	vStringList *Ptr = glb_ptr;

         if ( Ptr == NULL )
	          return 0L;


         delete Ptr;
		 glb_ptr=0; 
         return 1L;  

}



///////////////////////////////////////////////////////////////
//
//
//
//
//

extern "C" __declspec(dllexport)  LPSTR   
__stdcall L32_Object_GetMessage(  long id  )
{
           if ( glb_ptr == NULL ) {
			   return 0L;
           }
		   else {
	           return ( glb_ptr->GetMessage((ULONG)( id ) ));
           }
		   
           return 0L; // should not reach here
}


////////////////////////////////////////////////////////////////
//
//
//
//
//
//
//

extern "C" __declspec(dllexport)  BOOL    
__stdcall L32_Object_ReCreate(  LPSTR FileName  )
{
	
	
   if ( FileName == 0 ||
		  strlen(FileName)==0)
		  return FALSE;

	
	vStringList *Ptr = glb_ptr;

         if ( Ptr != NULL ) {
			 delete Ptr;
			 Ptr = 0;
			 glb_ptr=0;
         } 

		 return  L32_Object_Create( FileName );
         
}








