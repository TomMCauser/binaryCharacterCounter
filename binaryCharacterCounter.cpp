#include <iostream>
#include <stdint.h>
#include <fstream>
#include <string>
#include <chrono>

std::ifstream imageInput;
std::ofstream csvOutput;
uint8_t integerEquivalent;

int main(int argc, char *argv[])
{
    if (argc != 4) {
        std::cerr << "Incorrect syntax. Please type following: ./exeName (1) (2) (3)\n"
                     "(1) File name/path of the file you wish to investigate (eg: image.bmp)"
                     "(2) Your desired output file name (eg: binaryCount.txt)"
                     "(3) Either 'l' (literal) or 'd' (decimal). Literal = character will be placed in"
                     " the first column. Integer = the binary's equivalent integer equivalent will be"
                     " instead used." << std::endl;
        return 1;
    }
    
    const char* inputFileName = argv[1];
    const char* outputFileName = argv[2];
    const char* selectedMode = argv[3];

    if (!((*selectedMode == 'l') || (*selectedMode == 'd')))
    {
        std::cerr << "Incorrect mode selected. Please select either 'l' or 'd'." << std::endl;
        return 1;
    }

    auto timeStart = std::chrono::high_resolution_clock::now();

    /*
    First part - open up the files and check that they exist.
    Output file will be called output.csv
    */

    imageInput.open(inputFileName, std::ios::in | std::ios::binary);

    imageInput.seekg(0, std::ios::end);
    uint32_t fileSize = imageInput.tellg();
    imageInput.seekg(0, std::ios::beg);

    if (imageInput)
    {
        std::cout << "Image file located, with a filesize of " << fileSize << " bytes. Scanning file." << std::endl;
    }
    else
    {
        std::cerr << "Image file not found. Exiting program." << std::endl;
        return 1;
    } 

    csvOutput.open(outputFileName, std::ios::out);

    if (!csvOutput)
    {
        std::cout << "Failed to create output file." << std::endl;
    }

    /*
    Files are now open and ready to begin reading from/writing to.
    */

    uint32_t allCharactersArray[256] = {0};

    unsigned char ch = {};

    for (uint32_t i = 0; i < fileSize; i++)
    {
        ch = imageInput.get();
        if (imageInput.eof()){
            std::cout << "End of file found." << std::endl;
            break;
        }
        integerEquivalent = int(ch);
        allCharactersArray[integerEquivalent] += 1;
    }

    unsigned char charLiteral;
    int decimalValue;

    std::string d_headerNames = "Decimal;Frequency";
    std::string l_headerNames = "Literal;Frequency";

    if (*selectedMode == 'd')
    {
        csvOutput << d_headerNames << std::endl;
    }
    else if (*selectedMode == 'l')
    {
        csvOutput << l_headerNames << std::endl;
    }

    for (uint8_t i = 0; i < 255; i++)
    {
        if (*selectedMode == 'd')
        {
            decimalValue = i;
            csvOutput << decimalValue << ";" << allCharactersArray[i] << std::endl;
        }
        else if (*selectedMode == 'l')
        {
            charLiteral = i;
            csvOutput << charLiteral << ";" << allCharactersArray[i] << std::endl;
        }        
    }

    auto timeStop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = timeStop - timeStart;
    
    std::cout << "Conversion completed. Results output to " 
              << outputFileName << " (executed in " << duration.count() << " ms)." << std::endl;

    csvOutput.close();
    imageInput.close();
    return 0;
}