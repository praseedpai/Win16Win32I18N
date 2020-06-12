/**************************************************************************
  Routines To  Transparently Handle Edwin Internationalisation
  To be implemented as A Dynamic Link Library ,So that Strings can be
  Shared By Application. Routines are Highly Specific to 16 bit
  Windows Architecture.

  Written By :  Praseed Pai KT
                Visionics India PVT LTD

***************************************************************************/


// in order to parse japanese characters routines are to
// be made DBCS aware 
//
//




#if 0

#define  _USRDLL
#include <afx.h>
#include <afxwin.h>

#endif

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h> 
#include <fcntl.h>
#include <io.h>
#include <malloc.h>
#include <errno.h>
#include "vbsort.h"
#include "buff.h"  
#include "pm.h"



// Some Handy Debugging Macro
// to Enable macros   pass /D_DEBUG_ at the Command Line
// can be activated only in DOS MODE





#ifdef _DEBUG_
FILE *Nfile;

#define DEBUG_OPEN( x )  ( Nfile = fopen( x , "w+" ) )
#define DEBUG0( x )      fprintf( Nfile , x )
#define DEBUG1(x , y )   fprintf( Nfile , x , y )
#define DEBUG_CLOSE()    fclose( Nfile )
#define DEBUG_FLUSH()    fflush(Nfile )
#define DEXECUTE(x)      (x)
#define ASSERT_PARAM( x ) { if ( !(x)){ \
 fprintf( stderr,"Assertional Failed at %l in %s\n" ,__LINE__ , __FILE__ );\
 exit(0); }}


#else

#define DEBUG_OPEN( x )
#define DEBUG0( x ) 
#define DEBUG1(x , y ) 
#define DEBUG_CLOSE()
#define DEBUG_FLUSH()
#define DEXECUTE(x)
#define ASSERT_PARAM( x ) 

#endif


#define MAX_TREE  101                     // Maximum no of Application
#define STR_DELEMITER  '~'                // Seperator of String in Text File

#if 0

#define _CHECK_MEM_LEAK 

#endif


#ifdef  _CHECK_MEM_LEAK

#define EXEC_FUNC( x )    x;

#else

#define EXEC_FUNC( x )

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
              FirstPtr=AnsiNext( FirstPtr);
              

   
               if ( *FirstPtr == 0 )
               {
                  ret_val[0]=0;
                  return ret_val;
               }   
                  
                   
   /************** Find out the End of String ****************/
   
          iterator = FirstPtr;
          
          while ( *iterator != 0 ) 
            iterator = AnsiNext( iterator);
              
               
     char *TailPtr = AnsiPrev(FirstPtr , iterator);

   /******** Find first non white space character by Scanning backwards ***/

      while ( (*TailPtr == ' ' || *TailPtr == '\t'))
               TailPtr = AnsiPrev(FirstPtr , TailPtr);
         

    /******************* forcibly terminate with null **************/
                        
      *( AnsiNext(TailPtr)) = 0;     
      
       
      
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
         
        iterator = AnsiNext(iterator );
        
    }
        
    return NULL;
    
 }
        
  
///////////////////////////////////////////////////////////////////
// 
//   
//
//
//

class  vStringList
{
  private:  // Attributes  
  
     StrArray  *myptr;   // Structure that manages Strings
    

  // Since Edwin Consists of many modules ,
  // To minimize Search Depth , Per Process Tree is
  // maintained 

     pAppData  *Root[ MAX_TREE ];

     char   FileName[255];  // File from Which messages has to be extracted
              
     P_FILE   *infile;  // Pointer to Above see buff.cpp
     
     int GetNextLine(char *);   // Get the Next Linr from file
   

  public:    // Interface methods of class
  
 
   // Since MFC ( 16 bit ) does'nt Support C++ execption Handling ,
   // To Work around it , obj_created is a flag to indicate
   // proper initialisation of ctor
 

   int obj_created;

   // public interface functions for manipulating 
   // Translated String
   
   int AddString_T( char *id_string , char *KeyString );
   
   // Build The String Table
   
   int BuildTree_T();
   
   // Get The pointer to message for parameter id
   
   char far *GetMessage( char *message_id );
   


  // ctor
  // For Proper functioning of DLL , instance of global objects should be created
  // at the load time . so ctor doesn't load the strings from Text File
  // Worker Method CreateStringTree should be called explicitly to load the
  // strings from text file

      
    vStringList(void) 
    { 
    
       obj_created = 0;   // string table  is not created 
       myptr = NULL;      // Set Pointer to String management structure to NULL
       infile = NULL;     // Set File Pointer To NULL
       
       memset( FileName , 0 , sizeof( FileName ) );
       
       // Set Pointer to Application Specific string
       // to null 
            
         for( int i = 0; i<MAX_TREE; ++i )  
          
          {
                Root[ i ] = NULL;  
                
           }
       
    };
    
    
   // Worker method to Build the String Tree 
   // accepts one parameter , Name of The File 
       
    
    int  CreateStringTree(char *pFileName)
     {
  
     
          /********** Check For multiple invocation ***************/
                   
          
          if ( obj_created  == 1 )
                 return 1;
                
                
              
     
          obj_created = 0;   // Set To uncompleted object   
      
      
     
        
          lstrcpy( FileName , pFileName );   
          
          
          
          



   if( (_access( pFileName , 0 )) != 0 )
   {                     
   
       #if 1
       
         char ErrorBuffer[500];
         
         
         wsprintf( ErrorBuffer , " %s\n does not exists", pFileName); 
              
         MessageBox( 0 , ErrorBuffer , "EDWIN TRANSLATION TOOLKIT" , MB_ICONEXCLAMATION | MB_SYSTEMMODAL | MB_OK );
                
       #endif
             
           return 0;

   }              
   
   
   
   




     
          if (( infile = pfopen( FileName ) ) == NULL )
            
            {
                   
               #if 1
                
                char ErrorBuffer[500];
                
                wsprintf( ErrorBuffer , " %s\n   is in use \n Close this file and Restart EDWin Main", pFileName); 
                
                MessageBox( 0 , ErrorBuffer , "EDWIN TRANSLATION TOOLKIT" , MB_ICONEXCLAMATION | MB_SYSTEMMODAL |MB_OK );
                
                #endif
                   
                 return  0;
                       
             }          
                       
           
       
           if ( ( myptr = ( StrArray * ) _fmalloc(sizeof(StrArray)) ) == NULL )
             {
                             
                   EXEC_FUNC( MessageBox( 0 , "Memory Allocation failure " , "Diagnostics" , 1 ) );         
                          
                   return 0;
                               
              }                  
                  
             
          
           
            
            myptr->index_array = -1;            
            
            
            size_t max_index = ( size_t ) ( 4L * ( ( infile->FileSize / 65000L ) + 2L ) );
            
       
            
            if ( ( myptr->GlobalHandle = ( HGLOBAL * ) _fmalloc( max_index ) ) == NULL )
              {
              
                      EXEC_FUNC( MessageBox( 0 , "Global Handle Failure #2" , "Diagnostics" , 1 ) ); 
               
                      return 0; 
                                    
              }                      
              
            
            
            
            myptr->GlobalHandle[++myptr->index_array]  = GlobalAlloc( GMEM_SHARE   , 65000);
       
       
            
            if ( myptr->GlobalHandle[myptr->index_array] == NULL ) {   
            
                    
                     EXEC_FUNC( MessageBox( 0 , "Global Alloc  #3" , "Diagnostics" , 1 ) ); 
                     myptr->index_array -=1;
                       return 0;
             }
                  
               
             myptr->g_ptr  = ( unsigned char far * ) 
                             GlobalLock(  myptr->GlobalHandle[myptr->index_array]  );
           
   
             if ( myptr->g_ptr == NULL ) {
                  
                  
                   EXEC_FUNC( MessageBox( 0 , "Global Handle Failure #4" , "Diagnostics" , 1 ) ); 
                  
                  return  0;                                              
                  
                    
                    
               }     
             
             
             myptr->allocated = 65000L;
             myptr->num_chars = 0;  
        
        
             if ( BuildTree_T() == 0 ) 
               {
               
                
                  EXEC_FUNC( MessageBox( 0 , "Global Handle Failure #6" , "Diagnostics" , 1 ) ); 
                 return 0; 
                     
               }      
                     
             
             obj_created = 1;
        
             return 1;
             
             
             

    };    
    
    
    int UnLoadStringTree()
     {
    
            
            
       if ( myptr == NULL )
         {
               return 1;
         }
               
        if ( myptr->GlobalHandle == NULL )
            {
            
                 free(myptr);       
                 myptr = NULL;
                 return 1;
                 
             }          
       
        for( int t = 0; t<= myptr->index_array ; ++t )
        {
          
            GlobalUnlock( myptr->GlobalHandle[t] );
            GlobalFree( myptr->GlobalHandle [t]);
        }  
          
          
         
          
                  
         
         free( (void *) myptr->GlobalHandle );
         
         myptr->GlobalHandle = NULL;
         
         free( (void *) myptr );
         
         myptr = NULL;
         
         
                     
     
        for ( int i =0 ; i< MAX_TREE ; ++i )
         {
             if ( Root[ i ] == NULL )
                    continue;
                
                
             delete (Root[i]);
             
             Root[i] = NULL;
             
         }            
         
         
         
         obj_created = 0;
         
         return 0;
         
      };
      
      
    
    ~vStringList()    //dtor
     {   
                        
                        
        if ( obj_created == 0 )
               return;     
               
               
        if ( myptr == NULL )
          {
              return;
              
           }   
           
           if ( myptr->GlobalHandle == NULL )
            {
            
                 free(myptr);       
                 myptr = NULL;
                
                 
             }     
                     
                                       
        for( int t = 0; t<= myptr->index_array ; ++t )
          {
          
            GlobalUnlock( myptr->GlobalHandle[t] );
            GlobalFree( myptr->GlobalHandle [t]);
          }  
                        
                        
                  
         
         free( (void *) myptr->GlobalHandle );
         
         myptr->GlobalHandle = NULL;
         
         free( (void *) myptr );
         
         myptr = NULL;            
            
              
            
     
        for ( int i =0 ; i< MAX_TREE ; ++i )
         {
             if ( Root[ i ] == NULL )
                    continue;
                
                
             delete (Root[i]);
             
             Root[i] = NULL;
             
         } 
         
         
        
        obj_created = 0;
          
     };        
                


 };
 


 // Performs Classic Binary Tree insertion Alogrithm
 // Since Routine is not checking Balancing Factor
 // Tree Can Some Time degenarate into a Linear Linked
 // List. 
 

 int vStringList::AddString_T( char *id_string ,
                             char *message )
  {
      pAppData *Iter ;
      char   Prefix[3]; 
     
      memset( Prefix , 0 , 3 );
      strncpy( Prefix , id_string , 2 );

      unsigned short index = (unsigned short ) atoi( Prefix );
      unsigned short data  = (unsigned short ) atoi( id_string + 2 );
          
            

       if ( ( Iter = Root[ index ] ) == NULL )
          {

            Root[ index ] =  new pAppData();      
            
            if ( Root[ index ] ==  NULL )
                    return 0; 
                    
                    
            if ( Root[ index ]->obj_created == 0  )
                    return 0;
                   
                    
            Iter = Root[ index ];        
              
          
          }
          
         
         int s= lstrlen( message ) +1;
         
         
         if ( myptr->num_chars+s >= (myptr->allocated-10) )
          {
            myptr->GlobalHandle[++myptr->index_array]  = 
                      GlobalAlloc( GMEM_SHARE , 65000 );
       
            if ( myptr->GlobalHandle[myptr->index_array] == NULL ) {    
            
                    EXEC_FUNC( MessageBox( 0 , "Global Handle Failure #7" , "Diagnostics" , 1 ) );
                     myptr->index_array -= 1;
                     return   0;           
            }         
               
            myptr->g_ptr  = ( unsigned char far * ) GlobalLock(  myptr->GlobalHandle[myptr->index_array]  );
           
        
           if ( myptr->g_ptr == NULL )
             {
              
               EXEC_FUNC( MessageBox( 0 , "Global Handle Failure #8" , "Diagnostics" , 1 ) );
               return 0;
                   
             }      
             
             
            myptr->allocated  += 65000L;
                 
           }
              
          
         myptr->num_chars +=s;                   
         unsigned char far *old_ptr = myptr->g_ptr;  
         
          
         lstrcpy((char far * )myptr->g_ptr , ( *message == ' ' ) ? message+1 : message );
         
         myptr->g_ptr +=s;  
         
         
         
         

                   
         
         if ( Iter->AddItem( data , old_ptr ) == 0 )
                      return 0; 
              
              

         return 1;
             
         

   }


                     





 // method to Lookup for the National Language String
 // for the given msg_id


 char far *vStringList::GetMessage( char *msg_id )

  {
      char   Prefix[3];
      
       #if 0
      static int x = -1;
      #endif
      
      
        if ( obj_created == 0 ) 
            return NULL;
            
      
      memset( Prefix , 0 , 3 ); 
      
      #if 1
      
         strncpy( Prefix , msg_id  , 2 );
         
      #else
      
         * ( short * )Prefix  =  *(  short * ) msg_id ;
         
      #endif    
      
        #if 0 
             if (++x == 173)
               {
                 x = 0;
               }      
         #endif
               
      unsigned short index = (unsigned short ) atoi( Prefix );
      unsigned short data  = (unsigned short ) atoi( msg_id + 2 );



      if ( index == 0  || data == 0  ) 
      {
         return (char far * )NULL;
      }  
         
       
      if ( Root[index] == NULL )
           return (char far *)NULL; 
      
      return  (char far * )(Root[index]->GetStringPtr ( data )); 
      
     
  }
        
      
      
      
     
         
      
    
            
// Routine To Build Tree
// Calls the Inteface method AddString aS an internal
// helper Routine


int vStringList::BuildTree_T()
 {

   char Buffer[1000];                
   char re_buffer[1000];
   char CurrToken[50];   
   char ret_buffer[100];
   char far *Temp;


   start_read:

    
     /*********** IF EOF close the input file and return ***********/
     
     if ( GetNextLine(Buffer) == -1 )
      {
         pfclose( infile);  
         return 1;
      }


      Temp = (char far * ) Buffer; 

      while ( *Temp == ' ' || *Temp =='\t'  ) 
                 Temp = AnsiNext(Temp);


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
          
            
      if (AddString_T( ret_buffer , TempPtr ) == 0 )
                return 0; 
                

      goto start_read;

      return 1;



    }


 
   
int vStringList::GetNextLine( char *NextLineBuffer )
   {
     
     int n = 
     pfgets( NextLineBuffer , 500 , infile );       
     
     return n;
     
     
   }

          


///////////////////////////////////////////////////
//  Given Below Are Exported Function meant to be calleed
//  from Visual Basic DLL
//
//
//

vStringList t_tree;        
 




extern "C" int  FAR PASCAL  LZ_LoadString( char *FileName )
{                             

   if (  t_tree.CreateStringTree( FileName ) == 0 )
     {   
          t_tree.UnLoadStringTree();
          return 0;
     }     
           
    return 1;

}             
 

extern "C" char *  FAR PASCAL  LZ_GetMsg( char *id )
 {
       
       if ( strlen( id ) <= 2 ) 
             return (char far * ) NULL;
             
       if ( atol( id + 2 ) == 0L ) 
             return (char far *) NULL;  
                          
       return t_tree.GetMessage( id );

}

      
                              
extern "C" int  FAR PASCAL  LZ_UnLoadString()
  {                         
                                  
                                        
      t_tree.UnLoadStringTree(); 
      return 1;
       
  }
  
    
extern "C" int FAR PASCAL WEP(int)
 {
 
   return 1;
  
 }
           

