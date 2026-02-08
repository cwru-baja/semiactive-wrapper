from datetime import datetime
import os

# create cpp types from .dsdls

LIBS = "./dsdl_libs/"
PREFIX = ""
OUTPUT_FILE = "CPP_DSDLs.hpp"

DATE = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
AUTHOR = "Max Lawton"
OUTPUT = f"""\
/***********************************************************************

	{OUTPUT_FILE} - Generated C++ dsdl types

    Curret dslds :
        uavcan.primitive.*
        uavcan.si.*
        uavcan.time.*
        cwrubaja.*
            - cwrubaja.physics.Color.0.1.dsds
            - cwrubaja.servocontroller.Manual.1.0.dsds

	Author : {AUTHOR}
    Date   : {DATE}

***********************************************************************/

#ifndef {OUTPUT_FILE.split('.')[0].upper()}_HPP
#define {OUTPUT_FILE.split('.')[0].upper()}_HPP
"""

OUTPUT += """
// BEGIN MANUAL DEFINITIONS //

// END MANUAL DEFINITIONS // \n\n
"""


OUTPUT += "\n// BEGIN AUTO-GENERATED CONTENT // \n"

# fine all the files inside of the libs folder
found_dsdl_files:list[str] = []
def readFile(dir:str):
    if os.path.isdir(dir) :
        for file in os.listdir(dir):
            readFile(os.path.join(dir, file))
    else:
        if dir.endswith(".dsdl"):
            found_dsdl_files.append(dir)
readFile(LIBS)

# replace ./LIBS with nothing, and all . with _
def stringFixer(s:str):
    return s.replace(LIBS, '').replace('.', '_').replace('/', '_').replace('truncated ', '')

# go thourgh all found dsdl files
for f in found_dsdl_files:
    output = "\n// File: " + f + "\n"
    output += "struct " + PREFIX + stringFixer(f.split('.dsdl')[0]) + " {\n"
    with open(f, 'r') as file:
        lines = file.readlines()
        for line in lines:
            if line.strip() == '' or line.strip()[0] in ['@', '#', '%', '-', '*', '+', '/', '=', '~']:
                continue # Skip empty lines or annotations
            else :
                line = line.split('#')[0].strip() # Remove inline comments
                word_list = line.split(' ')
                correted_word_list:list[str] = []
                array_str:str = None
                for word in word_list:

                    # if we have an array type, we need to add it on the end of the next word
                    if array_str is not None :
                        word = word + array_str
                        array = None
                    elif '[' in word and ']' in word :
                        array_str = '[' + word.split('[')[-1]
                        array_str = array_str.replace('=', '').replace('>', '').replace('<', '')
                        word = word.split('[')[0]
                    else :
                        pass

                    # change all floats double and all its to just int
                    if 'float' in word :
                        correted_word_list.append('double')
                    elif 'int' in word :
                        correted_word_list.append('int')
                    elif word in ['truncated'] : # and remove truncated
                        pass
                    else :
                        correted_word_list.append(word)

                output += '\t' + ' '.join(list(map(stringFixer, correted_word_list))) + ';\n'
    output += "};\n"
    OUTPUT += output

OUTPUT += "\n// END AUTO-GENERATED CONTENT // \n#endif"

# write to output hpp file
with open(OUTPUT_FILE, 'w') as out:
    out.write(OUTPUT)
print("Wrote dsdl types to " + OUTPUT_FILE)