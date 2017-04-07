/*
  error.c
  
  This file provides the implemented error message.
  
  Created by: Eli Berg
  Date: 04/26/16
*/

#include "error.h"

/* Pass in the error code, get the error message */
void error_message( int err )
{
  if ( err == INVALID_HEADER_ID3_TAG )
    printf("ID3 tag not found; should be first 3 bytes of file\n");
  else if ( err == INVALID_HEADER_VERSION )
    printf("Unexpected ID3v2 error version of 0xFF\n");
  else if ( err == INVALID_HEADER_REVISION )
    printf("Unexpected ID3v2 error revision of 0xFF\n");
  else if ( err == INVALID_HEADER_FLAGS )
    printf("huh?\n");
  else if ( err == INVALID_HEADER_SIZE_BYTE )
  {
    printf("Unexpected header size byte of $80 at offset %i\n", (int)error_data);
  }
  else
    printf("Unknown error code: %i\n", err);
}