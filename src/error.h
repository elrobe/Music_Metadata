/*
  error.h
  
  This file provides error codes.
  
  Created by: Eli Berg
  Date: 04/26/16
*/

/* Error codes */
int INVALID_HEADER_ID3_TAG   = 1;
int INVALID_HEADER_VERSION   = 2;
int INVALID_HEADER_REVISION  = 3;
int INVALID_HEADER_FLAGS     = 4;
int INVALID_HEADER_SIZE_BYTE = 5;

/* Extra error data that can be used for the output message */
void* error_data;

void error_message( int err );