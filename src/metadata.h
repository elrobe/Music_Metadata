/*
  metadata.h
  
  This file provides metadata structures and function headers.
  
  Created by: Eli Berg
  Date: 04/22/16
*/

/*
  A ID3v2 tag can be detected with the following pattern:
     $49 44 33 yy yy xx zz zz zz zz
   Where yy is less than $FF, xx is the 'flags' byte and zz is less than
   $80.
   
   This structure is only really needed in case I plan on doing anything 
   with it at a later time; we only need the 'size' to read all the frames.
*/
typedef struct {
  /*char* TAG; the tag is always the same, so why read it in?*/
  uint8_t version;
  uint8_t revision;
  uint8_t flags;
  uint32_t size;
} ID3v2_Header;