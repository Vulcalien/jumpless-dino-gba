#!/bin/python

import sys, argparse
from sys import exit
from PIL import Image

# Setup argparse
parser = argparse.ArgumentParser()

parser.add_argument('map_filename',
                    type=argparse.FileType('rb'))
parser.add_argument('array_name')

parser.add_argument('-o', '--output',
                    type=argparse.FileType('w'), default=sys.stdout)
parser.add_argument('-s', '--static',
                    action=argparse.BooleanOptionalAction,
                    help='add the \'static\' modifier to the output ' +
                         'array')

args = parser.parse_args()

# Open image
img = Image.open(args.map_filename).convert('RGB')

# Parse image and generate output
f = args.output

if args.static:
    f.write('static ')

f.write('const u8 ' + args.array_name + '[4 * 2048] = {\n')

for x in range(2048):
    for y in range(4):
        pix = img.getpixel((x, y))

        if pix == (0, 0, 0):
            f.write('1,')
        elif pix == (255, 0, 0):
            f.write('2,')
        else:
            f.write('0,')
    f.write('\n')

f.write('};\n')
