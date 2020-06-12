#include <windows.h>
#include <stdio.h>
#include "pm.h"

#if 0

#define _CHECK_MEM_LEAK 

#endif


#ifdef  _CHECK_MEM_LEAK

#define EXEC_FUNC( x )    x;

#else

#define EXEC_FUNC( x )

#endif


   
  
      
 pAppData::pAppData()
   {
    
       obj_created = 0;   // set to False , the object creation flag
       
     
       if ( ( ArrayHandle = GlobalAlloc( GMEM_SHARE | GMEM_MOVEABLE | GMEM_ZEROINIT , 6000 ) ) == NULL ) 
        
         {
                   EXEC_FUNC( MessageBox( 0 , "Error #201" , "Diagnostics" , 1 ) );
                   return;
                   
          }          
          

                  
       curr_max = 6000 / sizeof( AppArray );  // number of item which can be stored in buffer
       
       num_elems = 0;
       obj_created =1;
       
       
     };
     

  pAppData::~pAppData( )
      { 
         GlobalFree( ArrayHandle );
         a_ptr = NULL;
      }


  int pAppData::AddItem( unsigned short id , unsigned char far *offset )
       {                                      
      
          if ( num_elems >= curr_max )
             
             {
               
                 ArrayHandle  = GlobalReAlloc( ArrayHandle ,
                                 (curr_max*sizeof( AppArray ) ) + 1200 , GMEM_ZEROINIT );

                  
                 if ( ArrayHandle == NULL )             
                   {
                    
                      EXEC_FUNC( MessageBox( 0 , "Error #202" , "Diagnostics" , 1 ) );
                      return 0;
                          
                   }       
                 curr_max += (1200 / sizeof(AppArray ));
               
               }

          
          
              if (( a_ptr =  ( AppArray * ) GlobalLock( ArrayHandle )) == NULL )
             
                  {       
                        EXEC_FUNC( MessageBox( 0 , "Error  #203" , "Diagnostics" , 1 ) );
                        return 0;
                        
                  }       

             a_ptr[ num_elems ].msg_id = id;
             
             a_ptr[ num_elems++ ].ptr    = offset;  
             
             
             GlobalUnlock( ArrayHandle);
             return 1;
        };                                              
        
             
             
             
       void pAppData::DumpData( )
        {                  
        
            
            a_ptr =  ( AppArray * ) GlobalLock( ArrayHandle );
       
                  if ( a_ptr == NULL )                         
                   {
                    
                      
                    
                        return ;
                        
                   }     
        
           for( int j = 0; j <= 100 ; ++j )
            {
              char buffer[255];
              
              sprintf( buffer , "%d %s" , a_ptr[j].msg_id, a_ptr[j].ptr );
              
              MessageBox(NULL , buffer ,NULL , 1 );            
              
             }       
                GlobalUnlock( ArrayHandle );
               
         };
      // perform Binary Search 
      //
      // 
      //
      //
      
  char far * pAppData::GetStringPtr(unsigned short data )
          {
                                 
            long last_elem   = num_elems-1;
            long first_elem  = 0;
            long mid = 0;
            char far *RetVal = 0;
            
           
            a_ptr =  ( AppArray * ) GlobalLock(ArrayHandle );
       
                 if ( a_ptr == NULL )
                  {
                  
                      EXEC_FUNC( MessageBox( 0 , "Error #204 " , "Diagnostics" , 1 ) );
                   
                        return NULL;              
                        
                  }      
                        
              
         
            while  ( last_elem >=first_elem  ) 
             {
                mid = ( first_elem +  last_elem ) /2;
                
                if ( a_ptr[mid].msg_id > data )
                    last_elem = mid-1;
                else if ( a_ptr[mid].msg_id < data )
                    first_elem = mid+1;
                else  // found 
                  { 
                     
                     RetVal = ( ( char far *) a_ptr[mid].ptr );
                     break; 
                  }
                     
             }
       
             GlobalUnlock( ArrayHandle );
             
             return RetVal;
             
        }



     
     