#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "classes.h"
#include "logger.h"
#include "utils.h"

using namespace std;


long findETX(std::ifstream& f) {

    // Find 0x03 (ETX - end of text) byte that indicates the ASCII header's end
    // and return its index. As the whole header is ASCII-encoded, this byte can
    // occur only one.

    char b;     // byte
    long i = 0; // index

    while (f.read(&b, 1)) {  // Read one byte at a time
        
        // if byte b is 0x03, return index i
        if (b == static_cast<char>(0x03)) {  
            LOG4CXX_INFO(Logger::get(), "findETX: identified ETX at byte " << i);
            return i; 
        }

        // otherwise, increment index
        i++;
    }

    // return -1 if not found
    LOG4CXX_INFO(Logger::get(), "findETX: unable to identify ETX! returning -1");
    return -1;  
}


std::vector<char> getHeader(std::ifstream& f, size_t n) {

    // Reads in and returns bytes 0 to n from file n.

    // init vector (resizable array) of type char for storing n bytes from f
    std::vector<char> buffer(n);

    // move reader head to start of file
    f.seekg(0);   

    // read n bytes
    f.read(buffer.data(), n); 

    // return the bytes
    return buffer;

}


std::map<std::string, MetaInfo> getMetaInfo()
{
    // Creates and returns a key-value mapping for processing the ASCII 
    // header's structure.

    // NOTE: makes use of https://en.wikipedia.org/wiki/Command_pattern
    
    std::map<std::string, MetaInfo> mapping = {

        // "Kennung BY: Produktlänge in byte"
        {"BY", MetaInfo("BY", 2, 7, 0, &Header::setBY)},
        
        // "Kennung VS: Format-Version"
        {"VS", MetaInfo("VS", 2, 2, 0, &Header::setVS)},
        
        // "Kennung SW: Software-Version"
        {"SW", MetaInfo("SW", 2, 8, 1, &Header::setSW)},
        
        // "Kennung PR: Genauigkeit der Daten (PRecision)"
        {"PR", MetaInfo("PR", 2, 4, 1, &Header::setPR)},
        
        // "Kennung INT: Intervalldauer in Minuten" - caution: key has length 3!
        {"IN", MetaInfo("IN", 3, 4, 0, &Header::setIN)},

        // "Kennung GP: Anzahl der Pixel (e.g. `1200x1100`)"
        {"GP", MetaInfo("GP", 2, 9, 0, &Header::setGP)},
        
        // "Kennung VV: Vorhersagezeitpunkt in Minuten nach der Messung (e.g. `075`)"
        {"VV", MetaInfo("VV", 2, 3, 1, &Header::setVV)},
        
        // "Kennung MF: Modulflags Dezimalwert der entsprechenden Binärdarstellung"
        {"MF", MetaInfo("MF", 2, 8, 1, &Header::setMF)},
        
        // "Kennung MS: Textlaenge m"
        {"MS", MetaInfo("MS", 2, 3, 0, &Header::setMS)},

    };

    return mapping;

}


std::tuple<MetaInfo, std::vector<char>> parse(std::vector<char> b, long i, const std::map<std::string, MetaInfo>& mi) {

    /*
    Helper function for parsing "non-positional" part of ASCII header.

    Reads in two bytes of b at position `b.begin() + i` and infers the field 
    from the key-value mappings in `m`.

    Then, reads in the appropriate number of bytes of that field and returns the
    field as well as the value bytes.


    Arguments
        b   header bytes vector
        i   current byte Index
        mi  CONST REFERENCE to string-to-MetaInfo map (for performance reasons)
    
    Returns
        a tuple (C++17!!) of Mapping and byte-vector

    C++17
        returns a tuple, see https://stackoverflow.com/a/16516315
    */

   LOG4CXX_INFO(Logger::get(), "parse: called with i = " << i);

   // init Mapping instance representing field;
    MetaInfo m;

    // read in the first two bytes and cast to library string
    const std::string key(b.begin()+i, b.begin()+i+2);
    
    // try to infer header field type
    try {
        m = mi.at(key);

        LOG4CXX_INFO(Logger::get(), "parse: found key = " << key);

        // start reading after the key bytes 
        auto start = b.begin()+i+m.getKeyLen(); 

        // determine payload bytes to read
        auto end = b.begin()+i+m.getLen();

        // read payload into `buff`
        std::vector<char> buff(start, end);

        // finally, return Mapping and payload
        return {m, buff};
    }
    catch (const std::out_of_range&) 
    {
        LOG4CXX_ERROR(Logger::get(), "parse: key = " << key << "not found in Mapping m!");

        std::vector<char> x {0};
        return {m, x};
    }

}
