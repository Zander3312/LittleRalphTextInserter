#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include "LittleRalphTextInserter.h"
using namespace std;

string hexToAddress(string hex) { //Converts a hex value to an address that can be inserted into the game, which involves changing to little endian and adding 0x80000000. Currently unused.
    string newHex{ "80" };
    while (hex.length() < 6) {
        hex.insert(0, "0");
    }
    newHex.insert(0, hex.substr(0, 2));
    newHex.insert(0, hex.substr(2, 2));
    return newHex.insert(0, hex.substr(4, 2));
}

string intToAddress(int num) { //Converts an int to an address that can be inserted into the game, which involves changing to hex in little endian and adding 0x80000000.
    string newHex{ "80" };
    stringstream stream{};
    stream << hex << num;
    string hexStr{stream.str()};
    while (hexStr.length() < 6) {
        hexStr.insert(0, "0");
    }

    newHex.insert(0, hexStr.substr(0, 2));
    newHex.insert(0, hexStr.substr(2, 2));
    return newHex.insert(0, hexStr.substr(4, 2));
}

string intToTwoBytes(int num) { //Converts int into two byte string in little endian. Used for preparing properties to be put in the game.
    string newHex{to_string(num/255)};
    newHex.insert(0, "0");
    num = num % 256;
    stringstream stream{};
    stream << hex << num / 16;
    stream << hex << num % 16;
    return newHex.insert(0, stream.str());
}

string hexToInsertable(string hexStr) { //Converts string of hex to its equivalent series of characters for insertion. Argument should always be a string of even length.
    string insertable{};
    string byte{};

    for (int i{ 0 }; i < hexStr.length(); i = i+2) {
        stringstream stream{};
        int x{0};
        byte = hexStr.substr(i, 2);
        stream << hex << byte;
        stream >> x;
        insertable.push_back(static_cast<unsigned char>(x));
    }

    return insertable;
}

int main(int argc, char* argv[])
{
    if (argc != 3) { //If the argument amount is incorrect
        cout << ("Usage: LittleRalphTextInserter.exe <txt> <stage>\nRefer to the readme for more usage info.") << endl;
        return 0;
    }
    else {
        //Initialize input/output files
        string fileName{ argv[1] };
        char stage{ argv[2][0] };
        ifstream textFile(fileName);
        ifstream readFile(stageMap[stage][0], ios::binary);
        ofstream writeFile("inserter_output.bin", ios::binary);
        writeFile << readFile.rdbuf(); //Copy stage file to the write file
        readFile.close(); //Close stage input file

        //Get values for the stage
        int charLocation{ stoi(stageMap[stage][1]) };
        int propLocation{ stoi(stageMap[stage][2]) };
        int extraLocation{ stoi(stageMap[stage][3]) };
        int charSpace{ stoi(stageMap[stage][4]) };
        int extraSpace{ stoi(stageMap[stage][5]) };
        int totalBoxes{ stoi(stageMap[stage][6]) };

        writeFile.seekp(charLocation, ios::beg);
        bool writing{ false };

        while (getline(textFile, currentLine)) { //while there are still lines in the input to be read
            for (int i{ 0 }; i < currentLine.length(); i++) {
                char currentChar{ currentLine[i] };

                switch (currentChar) {
                case '{': //start writing character
                    writing = true;
                    continue;

                case '}': //end writing character, text box is processed and ready to be inserted into game
                    writing = false;

                    if (prevDots > 0) { //for triple dot character
                        for (int j{ 0 }; j < prevDots; j++) {
                            boxText.push_back(static_cast<unsigned char>(charMap['.']));
                            charsOnLine++;
                            maxCharsOnLine = max(charsOnLine, maxCharsOnLine);
                        }
                        prevDots = 0;
                    }

                    if (boxText.length() > charSpace) { //figure out where there is space for the text and write characters there
                        if (boxText.length() > extraSpace) { //completely out of space, could potentially add more extra space locations but I don't anticipate needing them
                            cerr << "Error: Out of space. Consider rewriting text to be shorter." << endl;
                            return 1;
                        }

                        writeFile.seekp(extraLocation, ios::beg); //not enough space in the main character area, so go to where there's free space
                        writeFile.write(boxText.c_str(), boxText.length()); //write characters
                        address = intToAddress(extraLocation + 845120); //get address, add 845120 (0xCE540) to account for position in RAM
                        extraLocation = extraLocation + boxText.length(); //move where cursor will start later if extra space is needed again
                        extraSpace = extraSpace - boxText.length(); //update how much extra space we have left
                    }
                    else {
                        writeFile.seekp(charLocation, ios::beg); //not enough space in the main character area, so go to where there's free space
                        writeFile.write(boxText.c_str(), boxText.length()); //write characters
                        address = intToAddress(charLocation + 845120); //get address, add 845120 (0xCE540) to account for position in RAM
                        charLocation = charLocation + boxText.length(); //move where cursor will start later
                        charSpace = charSpace - boxText.length(); //update how much character space we have left
                    }

                    //write the properties of the text box to the correct location; horizontal positioning is fucky, should fix that later
                    writeFile.seekp(propLocation, ios::beg); //move cursor to the text box properties
                    writeFile.write(hexToInsertable(address).c_str(), 4); //write the address of the characters
                    writeFile.write(hexToInsertable(intToTwoBytes(boxText.length())).c_str(), 2); //write the amount of characters in the box
                    writeFile.seekp(4, ios::cur); //move past horizontal and vertical text box position
                    writeFile.write(hexToInsertable(intToTwoBytes(maxCharsOnLine)).c_str(), 2); //write box width, add 1 for insurance on text box space to reduce chance of cutting off characters
                    writeFile.write(hexToInsertable(intToTwoBytes(maxLines)).c_str(), 2); //write box height
                    propLocation = propLocation + 20; //get cursor position ready for the next text box
                    boxText = ""; //reset text box stuff
                    charsOnLine = 0;
                    maxCharsOnLine = 0;
                    lines = 1;
                    maxLines = 1;
                    numBoxes++;
                    if (numBoxes == totalBoxes) { //prevents writing past where it should if user inputs too many boxes which could cause some nasty overwrite issues
                        cout << "Successfully wrote to all text boxes." << endl;
                        textFile.close();
                        writeFile.close();
                        return 0;
                    }

                case '#': //comment character should stop reading from the current line and move to the next one
                    break;
                }

                if (writing) {
                    if (currentChar == '.') { //for triple dot character
                        if (prevDots == 2) {
                            boxText.push_back(static_cast<unsigned char>(58)); //58 = 3A which is the set of three dots, insert that here
                            charsOnLine++;
                            maxCharsOnLine = max(charsOnLine, maxCharsOnLine);
                            prevDots = 0;
                            continue;
                        }
                        else {
                            prevDots++;
                            continue;
                        }
                    }

                    if (prevDots > 0) { //also for triple dot character, if this is read then the current character is not a dot and we should insert any dots we saw previously
                        for (int j{ 0 }; j < prevDots; j++) {
                            boxText.push_back(static_cast<unsigned char>(charMap['.']));
                            charsOnLine++;
                            maxCharsOnLine = max(charsOnLine, maxCharsOnLine);
                        }
                        prevDots = 0;
                    }

                    charsOnLine++;

                    if (currentChar == '/') { //clear text box character, clears characters in box but doesn't start a new box
                        lines = 1;
                        charsOnLine = 0;
                    }

                    maxCharsOnLine = max(charsOnLine, maxCharsOnLine);
                    boxText.push_back(static_cast<unsigned char>(charMap[currentChar])); //add character to the string, will add all in the text box at once after we verify there's space for it
                }
            }
            if (writing) { //after the line is over, if we are writing then add new line character
                if (prevDots > 0) { //for triple dot character
                    for (int j{ 0 }; j < prevDots; j++) {
                        boxText.push_back(static_cast<unsigned char>(charMap['.']));
                        charsOnLine++;
                        maxCharsOnLine = max(charsOnLine, maxCharsOnLine);
                    }
                    prevDots = 0;
                }
                boxText.push_back(static_cast<unsigned char>(255));
                lines++;
                maxLines = max(maxLines, lines);
                charsOnLine = 0;
            }
        } //while end, should only reach this if there are less text boxes given than there are in the stage
        cout << "Wrote to some text boxes, but not all. If this was not intended, you probably missed a box somewhere." << endl;
        textFile.close();
        writeFile.close();
        return 0;
    }
}
