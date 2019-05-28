#!C:\Users\sglvladi\Anaconda3\python.exe

import os
import sys

import argparse

from arduinokeywords.arduinokeywords import parse_header, parse_library, output_keywords, get_keywords_fullpath
from arduinokeywords.version import __version__

# Setup command line parameters
parser = argparse.ArgumentParser(description='Generate a keywords.txt file for the Arduino IDE.')
parser.add_argument(type=str, dest='source', metavar="dir", default=os.getcwd(), nargs='?',
                    help='Your library\'s root directory. Defaults to current directory')
parser.add_argument('-o', '--output', type=str, dest='output', metavar="dir", default="./",
                    help='The folder in which the keywords.txt file will be created. Defaults to --source')
parser.add_argument('-c', '--constant', type=str, dest='constants', default=[], action="append",
                    help='Specify additional constants to be written to keywords.txt. Multiple constants can be '
                         'defined by repeating this argument', metavar="CONSTANT")
parser.add_argument('-f', '--header', '--header-file', type=str, dest='headers', default=[], action="append",
                    help='Manually specify the header files to be parsed. Multiple files can be '
                         'defined by repeating this argument', metavar="file")
parser.add_argument('-d', '--depth', type=int, dest='depth', default=1,
                    help='Specify how deep the automatic header search should look')
parser.add_argument('-v', '--version', help='Print version and quit', action='store_true')

args = parser.parse_args()

if args.version:
    print(__version__)
    sys.exit(0)

args.source = os.path.abspath(args.source)

if not os.path.exists(args.source):
    print("The source path {path} doesn't exist, quitting...".format(path=args.source))
    sys.exit(1)

if not os.path.isdir(args.source):
    print("The source path {path} isn't a directory, quitting...".format(path=args.source))
    sys.exit(1)

# If header files are specifed use those, otherwise search 'source' to a max depth of 'depth'
if len(args.headers) == 0:
    print("Searching for header files in {path}".format(path=args.source))
    print("Search max depth is: {depth}".format(depth=args.depth))

    classes = parse_library(args.source, args.depth)
else:
    print("Parsing the following header files:")
    classes = []
    for header in args.headers:
        print("\t{header}".format(header=header))
        try:
            classes.extend(parse_header(os.path.join(args.source, header)))
        except FileNotFoundError:
            print("\tFile {file} not found, skipping".format(file=header))

print("")
print("Classes found: {count}".format(count=len(classes)))
for clazz in classes:
    filename = os.path.relpath(clazz.filename, args.source)
    print("\t {class_name} (./{filename})".format(class_name=clazz.name, filename=filename))
print("")

if len(args.constants) > 0:
    print("Additional constants:")
    for constant in args.constants:
        print("\t {constant}".format(constant=constant))

keywords_filename = get_keywords_fullpath(os.path.join(args.source, args.output))
print("Outputting keywords.txt to: {path}".format(path=keywords_filename))

with open(keywords_filename, 'w+') as keywords_file:
    output_keywords(classes, keywords_file, args.constants)

print("Done")
