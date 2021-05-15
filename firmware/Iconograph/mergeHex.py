#!/usr/bin/env python3

# Python script to merge two or more HEX files.
#
#   by Adam Pierce <adam@levaux.com>
#   for Levaux Pty. Ltd. Australia
#
# Usage: mergeHex.py <file1.hex> <file2.hex> [<file3.hex> ...]
# Outputs merged file to STDOUT.

import sys
from intelhex import IntelHex

# Get the list of filenames to merge.
filenames = sys.argv[1:]

# Load the HEX files.
files = []
for fn in filenames:
	x = IntelHex()
	x.fromfile(fn, format='hex')
	files.append(x)

# Merge them all
for i in range(1,len(files)):
	files[0].merge(files[i], overlap='replace')

# Output the result to STDOUT.
files[0].write_hex_file(sys.stdout)
