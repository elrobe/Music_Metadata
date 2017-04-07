#
# This is a test program to read binary 
# metadatdata from a music file.
#
# Created by: Eli Berg
# Date: 02/09/16
#

import array
import binascii
import tempfile

file = open("08 Blank Space.mp3", 'rb')

# read the raw data
raw_data = file.read()
data = binascii.hexlify(raw_data)

print "header\t", data[:6]
print "title\t", data[7:67]