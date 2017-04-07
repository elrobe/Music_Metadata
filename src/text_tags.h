/*
  text_tags.c
  
  This file keeps a list of the ID3v2 text frame tags
  that we care to use in collecting metadata about
  any given MP3 file.
  
  Created by: Eli Berg
  Date: 02/14/16 (Happy Valentine's Day you single piece of shit)
*/

#ifndef TEXT_FRAME_H
#define TEXT_FRAME_H

/*
  ID3v2 text information frame.
  
  The text information frames are the most important frames, containing
  information like artist, album and more. There may only be one text
  information frame of its kind in an tag. If the textstring is followed
  by a termination ($00 (00)) all the following information should be
  ignored and not be displayed. All the text information frames have the
  following format:

  Text information identifier     "T00" - "TZZ" , excluding "TXX"
    Frame size                    $xx xx xx
    Text encoding                 $xx
    Information                   <textstring>
*/
typedef struct {
  char tag[3];
  uint32_t size;
  uint8_t encoding;
  char* info;
} ID3v2_Text_Frame;


/*
  This structure is meant as a wrapper to 

typedef struct {
  char* tag;
  char* description;
} Tag_Type;

/* List of all the relevant tags we care to look for 
static const Tag_Type TAGS[NUM_TAGS][3] = {
  {'T','A','L'}, /* 1) Album/Movie/Show title 
  {'T','O','A'}, /* 2) Original artist(s)/performer(s) 
  {'T','P','1'}, /* 3) Lead artist(s)/Lead performer(s)/Soloist(s)/Performing group 
  {'T','P','2'}, /* 4) Band/Orchestra/Accompaniment 
  {'T','C','M'}  /* 5) Composer 
};*/

#endif