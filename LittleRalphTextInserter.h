#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
using namespace std;

string currentLine, boxText, address{};
int totalChars, charsOnLine, maxCharsOnLine, prevDots, numBoxes{0};
int maxLines{1};
int lines{1};
map<char, vector<string>> stageMap = { //       char    prop     extra   size  exsize  length
            { 'o', { "OPENNING/OPENNING.BIN", "19000", "19784", "59776", "782", "144", "19" } }, //vector order: write file, char write location, property write location, extra space location, space amount, extra space amount, number of text boxes
            { '1', { "MINIKID/MINIKID.BIN", "307092", "307808", "268560", "714", "304", "19" } },
            { '2', { "MINIKID/MINIKID.BIN", "308186", "308524", "268800", "338", "64", "12" } },
            { '3', { "MINIKID/MINIKID.BIN", "308764", "309168", "269824", "404", "160", "11" } },
            { 't', { "DEKAKID/DEKAKID.BIN", "82916", "83364", "0", "446", "0", "12" } },
            { '5', { "MINIKID/MINIKID.BIN", "309388", "309796", "269984", "408", "240", "15" } },
            { 'v', { "DEKAKID/DEKAKID.BIN", "83584", "84396", "0", "812", "0", "21" } },
            { 'e', { "ENDING/ENDING1.BIN", "20576", "22388", "59104", "1812", "160", "13" } },
            { 'f', { "DEKAKID/DEKAKID.BIN", "84816", "85672", "0", "856", "0", "23" } },
            { 'd', { "DEKAKID/DEKAKID.BIN", "86132", "86864", "0", "732", "0", "16" } },
            { 'n', { "ENDING/ENDING2.BIN", "24496", "27108", "297906", "2612", "174", "27" } },
            { 'h', { "MINIKID/MINIKID.BIN", "310096", "310792", "272425", "694", "129", "14" } } 
};

map<char, int> charMap = {
    {'A', 0},
    {'B', 1},
    {'C', 2},
    {'D', 3},
    {'E', 4},
    {'F', 5},
    {'G', 6},
    {'H', 7},
    {'I', 8},
    {'J', 9},
    {'K', 10},
    {'L', 11},
    {'M', 12},
    {'N', 13},
    {'O', 14},
    {'P', 15},
    {'Q', 16},
    {'R', 17},
    {'S', 18},
    {'T', 19},
    {'U', 20},
    {'V', 21},
    {'W', 22},
    {'X', 23},
    {'Y', 24},
    {'Z', 25},
    {'a', 26},
    {'b', 27},
    {'c', 28},
    {'d', 29},
    {'e', 30},
    {'f', 31},
    {'g', 32},
    {'h', 33},
    {'i', 34},
    {'j', 35},
    {'k', 36},
    {'l', 37},
    {'m', 38},
    {'n', 39},
    {'o', 40},
    {'p', 41},
    {'q', 42},
    {'r', 43},
    {'s', 44},
    {'t', 45},
    {'u', 46},
    {'v', 47},
    {'w', 48},
    {'x', 49},
    {'y', 50},
    {'z', 51},
    {'.', 52},
    {'!', 53},
    {'?', 54},
    {'\'', 55},
    {',', 56},
    {'-', 57},
    {' ', 59},
    {'0', 60},
    {'1', 61},
    {'2', 62},
    {'3', 63},
    {'4', 64},
    {'5', 65},
    {'6', 66},
    {'7', 67},
    {'8', 68},
    {'9', 69},
    {'~', 70},
    {'/', 254},
    {'\n', 255}
};