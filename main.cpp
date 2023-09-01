#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "classes.h"
#include "logger.h"
#include "utils.h"

using namespace std;

int main(int argc, char* argv[]) {

    /* init Header instance ------------------------------------------------- */

    Header h;

    
    /* init Mapping instance ------------------------------------------------ */

    // get key-value mapping for processing the ASCII header's structure
    std::map<std::string, Mapping> mapping = getMapping();


    /* handle file ---------------------------------------------------------- */

    // documentation on opening files: https://cplusplus.com/doc/tutorial/files/

    // check if user provided a filename
    if (argc < 2) {
        LOG4CXX_ERROR(Logger::get(), "main: invalid program invocation!");
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;

        // return with error
        return 1;
    }

    // save file name to variable
    string filename = argv[1];

    // set header attribute
    h.setFN(filename);

    // open file in binary mode and check for errors
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        LOG4CXX_ERROR(Logger::get(), "main: unable to open file " << filename);
        std::cerr << "Could not open file " << filename << std::endl;
        return 1; // exit with error
    }

    LOG4CXX_INFO(Logger::get(), "main: opened file " << argv[1] << " successfully!");


    /* import header -------------------------------------------------------- */

    // get ETX byte position
    long etxIndex = findETX(file);

    // EXAMPLE: yields 191 = '0x000000b0' = 176 + 7 + 8 = 191 :)
    LOG4CXX_INFO(Logger::get(), "ETX index: " << etxIndex);

    // load header into local variable
    std::vector<char> hb = getHeader(file, etxIndex);


    // close file
    file.close();
    LOG4CXX_INFO(Logger::get(), "main: closed file");


    /* handle fixed-positioned meta data ------------------------------------ */
    
    // "Produktkennung"
    
    // get bytes at position 1 and 2
    std::vector<char> bytes(hb.begin(), hb.begin() + 2);

    // set header attribute
    h.setPI(bytes);


    // time stamp 
    // → read 3 "data fields" at once, then split and build time stamp
    bytes.assign(
        hb.begin()+2,           // begin reading after "Produktkennung"
        hb.begin()+2+6+5+4);    // read 6 ts A + 5 WMO + 4 ts B bytes


    // extract and create timestamp in "YYMMDDhhmm" format
    char ts[11] = {
        bytes[13], bytes[14], // YY
        bytes[11], bytes[12], // MM
        bytes[0], bytes[1],   // DD
        bytes[2], bytes[3],   // hh
        bytes[4], bytes[5],   // mm
        '\0'};                // end c-string with null-terminator
    
    // set header attribute
    h.setTS(ts);
    

    // "WMO Nummer"

    // extract "WMO-Nummer": offset is 6B from ts A, length is 5B
    bytes.assign(hb.begin()+8, hb.begin()+8+5);

    // set header attribute
    h.setWN(bytes);
    

    /* LOGGING --------------------------------------------- */
    
    LOG4CXX_INFO(Logger::get(), "main: ProductId = " << h.getPI());
    LOG4CXX_INFO(Logger::get(), "main: timestamp = " << h.getTS());
    LOG4CXX_INFO(Logger::get(), "main: WMO       = " << h.getWN());
    
    
    /* read non-positional data --------------------------------------------- */

    /* "... der Parser (sollte) so implementiert sein, dass er den Inhalt (...)
     *  anhand der jeweils einleitenden Kennung verarbeitet." */


    // start looking for identifiers at byte 17, read until `etxIndex`

    long i = 17;


    // we expect 9 other key-value pairs; while-loop would be another option;
    for (int j = 0; j<9; j++)
    {
        // get mapping and bytes
        // NOTE: mapping is passed by reference → no special syntax required!
        auto [mm, by] = parse(hb, i, mapping);

        
        LOG4CXX_INFO(Logger::get(), "main: getKey : " << mm.getKey() << " (" << mm.getKeyLen() << "+" << mm.getValLen() << "=" << mm.getLen() << ")");
        
        std::string logs;
        for (auto b : by) 
            logs += b;
        LOG4CXX_INFO(Logger::get(), "main: buff   : " << logs);
        
        LOG4CXX_INFO(Logger::get(), "main: i: " << i << " -> " << (i+mm.getLen()));
        
        // get setter function from mapping and assign respective value
        Mapping::SetterFunction setterFunc = mm.getSetter();
        (h.*setterFunc)(by); 

        // add number of processed bytes to index i
        i += mm.getLen();


        // in case we arrived at "MS", read in the subsequent text
        if (mm.getKey() == "MS")
        {
            // get length of text in byte
            int textLen = h.getMS();

            // read in text
            std::string text(hb.begin()+i, hb.begin()+i+textLen);

            LOG4CXX_INFO(Logger::get(), "main: text : " << text);

            // set header attribute
            h.setText(text);

            LOG4CXX_INFO(Logger::get(), "main: i: " << i << " -> " << (i+textLen));

            // update index
            i += textLen;
        }

    }

    // print header to console
    cout << h << endl;

    // exit with status code 0
    return 0;

}
