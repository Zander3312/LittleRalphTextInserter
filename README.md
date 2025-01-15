# The Adventure of Little Ralph Text Inserter

Text insertion program for The Adventure of Little Ralph for the PlayStation 1. Intended for use with the (currently unreleased) English translation patch.

### Usage instructions

The .exe should be placed in the root of the Little Ralph English Translation's unpacked root directory (next to folders like MINIKID and OPENNING).
Program should be run from the command line with two arguments: the file the text is to be read from (preferably a .txt), and the stage the text is for. The stage argument should be a single character.
The opening is the only stage currently supported, and can be chosen by inputting "o" as the second command line argument.
After the program has run successfully, a new "inserter_output.bin" will be created. When ready, replace the original file you edited with said file to finish editing.

### Text input format

The text input should be a .txt file containing all text in the stage. There are a few special characters that help define the text:

{: Beginning of text box, all text following this will be processed as part of a single text box<br>
}: End of text box, ends processing the characters and closes off this text box<br>
#: Comment character, anything after this character on the same line will not be processed as text even if it is between curly braces<br>
/: Clear text box character, if you run out of room on the screen but still have more to say, use this character, do not put a new line after this character is used

Example of a valid .txt input:<br>
{Hello!} #This is a comment<br>
{This box has #This is also a comment<br>
two lines!}<br>
{This text box<br>
is going to be cleared!/Now I can say more!}
