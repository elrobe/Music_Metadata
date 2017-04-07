/*
  metadata.c
  
  This file reads in a ID3v2 MP3 file and prints out
  the relevant metadata from the byte code.
  
  Created by: Eli Berg
  Date: 02/12/16
  History: 
    04/22/16 - Moved header struct to .h file and checked for ID3 tag in first three bytes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "metadata.h"
#include "error.h"
#include "text_frame.h"

/*
  read_header:
    Reads each byte until the ID3 header is found.
    Returns the ID3 metadata frame via reference.
*/
int read_header(FILE* in, ID3v2_Header** header)
{
  char cur;
  int i, found;
  uint8_t version, revision, flags;
  uint32_t size;
  
  /* The ID3v2 header tag should be the first three bytes in the file */
  for ( i = 0; i < 3; i++ )
  {
    cur = getc( in );
    if ( i == 0 && cur != 'I' ) break;
    if ( i == 1 && cur != 'D' ) break;
    if ( i == 2 && cur != '3' ) break;
    else found = 1;
  }
  
  if ( !found )
    return 1;
  
  /* 
    Major version and minor version.
    According to the ID3v2 spec, the version numbers must be
    less than 0xFF.
  */
  version = getc( in );
  if ( version == 0xff )
    return 2;
  
  revision = getc( in );
  if ( revision == 0xff )
    return 3;
  
  /* flags byte can be anything */
  flags = getc( in );
  
  /* TO DO: Error checking for each size byte to be less than 0x80 */
  size = getc( in ); /* 1st size byte, 3 more after */
  for ( i = 0; i < 3; i++ )
  {
    cur = getc(in);
    if ( ((int)cur) >= 0x80 )
    {
      error_data = (void*) i;
      return 5;
    }
    size = (size << 7) | (cur);
  }
  
  /* if we made it this far, we found the header */
  (*header) = malloc(sizeof(ID3v2_Header));
  (*header)->version = version;
  (*header)->revision = revision;
  (*header)->flags = flags;
  (*header)->size = size;
  
  return 0;
}

/*
  find_text_frame:
    Searches the buffer 1 character at a time until one of the
    text frame tags is found from text_tags.h
    Returns the tag string.
*/
char* find_text_frame(char* buffer, int length, int* idx)
{
  char cur, *tag;
  int cnt;
  
  tag = malloc(sizeof(char)*3);
  
  cnt = 0;
  while ( cnt != 3 && (*idx) < length )
  {
    cur = buffer[*idx];
    (*idx)++;
    
    if ( cur == 'T' )
    {
      tag[cnt] = cur;
      cnt++;
    }
    else if ( cnt==1 && (cur>='0'&&cur<='Z') )
    {
      tag[cnt] = cur;
      cnt++;
    }
    else if ( cnt==2 && (cur>='0'&&cur<='Z') )
    {
      tag[cnt] = cur;
      cnt++; /* this will quit the loop */
    }
    else
      cnt = 0;
  }
  
  if ( cnt == 3 )
    return tag;
  else
    return NULL;
}

/*
  read_metadata:
    Searches for all of the metadata text tags defined in
*/
int read_metadata(FILE* in, FILE* out, uint32_t bytes)
{
  int i, idx;
  uint32_t size;
  char *buffer, *tag;
  size_t total_read;
  ID3v2_Text_Frame *frame;
  
  idx = 0;
  buffer = malloc(sizeof(char)*bytes);
  
  total_read = fread(buffer, 1, bytes, in);
  
  /* DEBUG PRINT */
  for ( i = 0; i < total_read; i++ )
    printf("%c", buffer[i]);
  printf("\n");
  /* END DEBUG */
  
  while ( idx < total_read ) 
  {
    tag = find_text_frame(buffer, total_read, &idx);
    if ( tag == NULL )
      return 1;
    
    frame = malloc(sizeof(ID3v2_Text_Frame));
    frame->tag = tag;
    printf("%s\n",frame->tag);
    
    /* The frame size is 3 bytes long */
    size = 0;
    for ( i = 0; i < 3; i++ )
    {
      printf("-->%x\n",buffer[idx]);
      size = (size << 8) | (buffer[idx]);
      printf("---->%i\n",size);
      idx++;
      if ( idx >= total_read )
        return 1;
    }
    frame->size = size;
    frame->encoding = buffer[idx];
    idx++;
    if ( idx >= total_read )
        return 1;
    
    frame->text = malloc(sizeof(char)*size);
    for ( i = 0; i < size; i++ )
    {
      frame->text[i] = buffer[idx];
      idx++;
      if ( idx >= total_read )
        return 1;
    }
    
    printf("%s\n",frame->text);
  }
}

/* 
  main:
    Runs the main program, used:
    ./metadata input-file.mp3 [output-file]
    
    Prints to stdout if no output file is specified.
*/
int main(int argc, char *argv[])
{	
  int err;
	FILE *id3v2,*output;
  ID3v2_Header *header;
  uint32_t frame_size;
  
  if ( argc < 2 || argc > 3 )
  {
    fprintf(stderr, "\nUsage: ./metadata input-file.mp3 [output-file]\n");
    exit(1);
  }
  
  if ( argc < 3 )
    output = stdout;
  else
    output = fopen(argv[2], "w");
  
	id3v2 = fopen(argv[1], "r");
  if ( id3v2 == NULL )
  {
    fprintf(stderr, "Error opening .mp3 file: %s", argv[1]);
    return 0;
  }
  
  /* look for the ID3 header */
  err = read_header(id3v2, &header);
  if ( err ) 
  {
    error_message( err );
    return 0;
  }
  
  return 0;
  
  /* if we found the header, parse out the text metadata */
  if ( frame_size > 0 )
    err = read_metadata(id3v2, output, frame_size);
  else
    return 0;
  if ( err )
    fprintf(output, "Unexpected end of metadata!");
  
  fclose( output );
  fclose( id3v2 );
}