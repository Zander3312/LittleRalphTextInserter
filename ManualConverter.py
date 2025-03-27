chartobyte = {
    'A' : "00 ",
    'B' : "01 ",
    'C' : "02 ",
    'D' : "03 ",
    'E' : "04 ",
    'F' : "05 ",
    'G' : "06 ",
    'H' : "07 ",
    'I' : "08 ",
    'J' : "09 ",
    'K' : "0A ",
    'L' : "0B ",
    'M' : "0C ",
    'N' : "0D ",
    'O' : "0E ",
    'P' : "0F ",
    'Q' : "10 ",
    'R' : "11 ",
    'S' : "12 ",
    'T' : "13 ",
    'U' : "14 ",
    'V' : "15 ",
    'W' : "16 ",
    'X' : "17 ",
    'Y' : "18 ",
    'Z' : "19 ",
    'a' : "1A ",
    'b' : "1B ",
    'c' : "1C ",
    'd' : "1D ",
    'e' : "1E ",
    'f' : "1F ",
    'g' : "20 ",
    'h' : "21 ",
    'i' : "22 ",
    'j' : "23 ",
    'k' : "24 ",
    'l' : "25 ",
    'm' : "26 ",
    'n' : "27 ",
    'o' : "28 ",
    'p' : "29 ",
    'q' : "2A ",
    'r' : "2B ",
    's' : "2C ",
    't' : "2D ",
    'u' : "2E ",
    'v' : "2F ",
    'w' : "30 ",
    'x' : "31 ",
    'y' : "32 ",
    'z' : "33 ",
    '.' : "34 ",
    '!' : "35 ",
    '?' : "36 ",
    "'" : "37 ",
    ',' : "38 ",
    '-' : "39 ",
    ' ' : "3B ",
    '0' : "3C ",
    '1' : "3D ",
    '2' : "3E ",
    '3' : "3F ",
    '4' : "40 ",
    '5' : "41 ",
    '6' : "42 ",
    '7' : "43 ",
    '8' : "44 ",
    '9' : "45 ",
    '~' : "46 ",
    '/' : "FE ",
    '\n' : "FF "
}

import sys

def bigend_to_insertable(big_endian):
    big_endian = big_endian[2:]
    while(len(big_endian) < 6):
        big_endian = "0" + big_endian
    return big_endian[4:6] + ' ' + big_endian[2:4] + ' ' + big_endian[0:2] + ' 80 '

if (len(sys.argv) != 3):
    print("Usage: lr_text_inserter.py <text> <starting address>")
    exit(0)
infile = sys.argv[1]
write_address = sys.argv[2]
write_address = int(write_address, 16)
output_chars = open('outputchars.txt', 'w')
output_props = open('outputprops.txt', 'w')
chars_in_box, chars_on_line, max_chars_on_line, max_lines, lines = 0, 0, 0, 1, 1
prevdots = 0
is_writing = False
with open(infile, 'r') as file:
    for line in file:
        for char in line:
            if (char == '{'): #start of new box character
                is_writing = True
                continue
            if (char == '}'): #end current box character
                if(prevdots > 0):
                    for i in range (0, prevdots):
                        output_chars.write("34 ")
                        chars_in_box, chars_on_line = chars_in_box + 1, chars_on_line + 1
                        max_chars_on_line = max(chars_on_line, max_chars_on_line)
                    prevdots = 0
                output_chars.write("\n")
                is_writing = False
                new_address = write_address + (chars_in_box)
                chars_in_box, max_chars_on_line, max_lines = hex(chars_in_box)[2:], hex(max_chars_on_line)[2:], hex(max_lines)[2:]
                if (len(chars_in_box) ==  3):
                    chars_in_box = chars_in_box[1:3] + " 0" + chars_in_box[0] + ' '
                elif (len(chars_in_box) == 1):
                    chars_in_box = '0' + chars_in_box + ' 00 '
                else:
                    chars_in_box = chars_in_box + " 00 "
                if (len(max_chars_on_line) == 1):
                    max_chars_on_line = "0" + max_chars_on_line
                if (len(max_lines) == 1):
                    max_lines = "0" + max_lines
                output_props.write(bigend_to_insertable(hex(write_address)) + chars_in_box + f"XX XX XX XX {max_chars_on_line} 00 {max_lines} 00\n")
                write_address = new_address
                chars_in_box, chars_on_line, max_chars_on_line, max_lines, lines = 0, 0, 0, 1, 1
                continue
            if(is_writing):
                if(char == '.'):
                    if(prevdots == 2):
                        output_chars.write("3A ")
                        chars_in_box, chars_on_line = chars_in_box + 1, chars_on_line + 1
                        max_chars_on_line = max(chars_on_line, max_chars_on_line)
                        prevdots = 0
                        continue
                    else:
                        prevdots = prevdots + 1
                        continue
                if(prevdots > 0):
                    for i in range (0, prevdots):
                        output_chars.write("34 ")
                        chars_in_box, chars_on_line = chars_in_box + 1, chars_on_line + 1
                        max_chars_on_line = max(chars_on_line, max_chars_on_line)
                    prevdots = 0
                chars_in_box, chars_on_line = chars_in_box + 1, chars_on_line + 1
                if(char == '/'): #clear text box character
                    lines = 1
                    chars_on_line = 0
                if(char == '\n'): #new line character
                    lines = lines+1
                    max_lines = max(max_lines, lines)
                    chars_on_line = 0
                max_chars_on_line = max(chars_on_line, max_chars_on_line)
                output_chars.write(chartobyte[char])
print("Character and property data written successfully.")