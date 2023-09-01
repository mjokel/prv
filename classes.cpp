#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "utils.h"
#include "classes.h"

using namespace std;

/* HEADER ------------------------------------------------------------------- */

// constructor A
Header::Header() {}

// destructor: for now empty, as no files opened, etc.
Header::~Header() {}


/* to string - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */


std::ostream& operator<<(std::ostream& os, const Header& h) {
    os << 
        "ASCII Header of '" << h.filename << "'\n" <<
        " PI:   " << h.productId << "\n" <<
        " TS:   " << h.timestamp << "\n" <<
        " WMO:  " << h.wmo << "\n" <<
        " SW:   " << h.sw << "\n" <<
        " PR:   " << h.pr << "\n" <<
        " GP:   " << h.gp << "\n" <<
        " MF:   " << h.mf << "\n" <<
        " MS:   " << h.ms << "\n" <<
        " TX:   " << h.text;

    return os;
}


/* getter - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

const std::string Header::getFN() const { return filename; }
const std::string Header::getPI() const { return productId; }
const std::string Header::getTS() const { return timestamp; }
const std::string Header::getWN() const { return wmo; }
const std::string Header::getSW() const { return sw; }
const std::string Header::getPR() const { return pr; }
const std::string Header::getGP() const { return gp; }
const std::string Header::getMF() const { return mf; }
const std::string Header::getText() const { return text; }
  
const int Header::getBY() const { return by; }
const int Header::getVS() const { return vs; }
const int Header::getIN() const { return in; }
const int Header::getVV() const { return vv; }
const int Header::getMS() const { return ms; }


/* helpers - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

int Header::evalBufferSize(std::vector<char> b, int n)
{
    // Check for expected number of bytes in argument b and throw exception.
    
    if (b.size() != n) 
    {
        std::string msg = "b must be of size " + std::to_string(n) + "!";
        throw std::invalid_argument(msg);
        return 1;
    }
    return 0;
}


/* "string" setter - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

void Header::setFN(std::string s) 
{ 
    // set file name
    filename = s;
    return;
}

void Header::setPI(std::vector<char> b) 
{ 
    // ensure correct number of bytes in argument b
    evalBufferSize(b, 2);

    // cast bytes to string and assign attribute value
    productId = std::string (b.begin(), b.end());
    return;
}

void Header::setTS(char ts[]) 
{
    // ensure correct number of bytes in argument b
    // NOTE: strlen does not include the \0 char in the count!
    if (std::strlen(ts) != 10)
        throw std::invalid_argument("b must be of size 11!");

    // cast from char array to string and assign attribute value;
    timestamp = std::string(ts);
    return;
}

void Header::setWN(std::vector<char> b) 
{
    // ensure correct number of bytes in argument b
    evalBufferSize(b, 5);

    // cast bytes to string and assign attribute value
    wmo = std::string(b.begin(), b.end());
    return;
}

void Header::setSW(std::vector<char> b) 
{
    // ensure correct number of bytes in argument b
    evalBufferSize(b, 9);

    // cast bytes to string and assign attribute value
    sw = std::string(b.begin()+1, b.end()); // ignore leftmost byte!
    return;
}

void Header::setPR(std::vector<char> b) 
{
    // ensure correct number of bytes in argument b
    evalBufferSize(b, 5);

    // cast bytes to string and assign attribute value
    pr = std::string(b.begin()+1, b.end()); // ignore leftmost byte!
    return;
}

void Header::setGP(std::vector<char> b) 
{
    // ensure correct number of bytes in argument b
    evalBufferSize(b, 9);

    // cast bytes to string and assign attribute value
    gp = std::string(b.begin()+1, b.end()); // ignore leftmost byte!
    return;
}

void Header::setMF(std::vector<char> b) 
{
    // ensure correct number of bytes in argument b
    evalBufferSize(b, 9);

    // cast bytes to string and assign attribute value
    mf = std::string(b.begin()+1, b.end()); // ignore leftmost byte!
    return;
}


/* "int" setter - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void Header::setBY(std::vector<char> b) 
{
    // ensure correct number of bytes in argument b
    evalBufferSize(b, 7);

    // init string variable for storing interim result
    std::string s = "0";

    for (int i=0; i<b.size(); i++)
        s[i] = b[i];

    // cast from string to int
    by = std::stoi(s);
    return;
}

void Header::setVS(std::vector<char> b) 
{
    // ensure correct number of bytes in argument b
    evalBufferSize(b, 2);

    // init string variable for storing interim result
    std::string s = "0";

    for (int i=0; i<b.size(); i++)
        s[i] = b[i];

    // cast from string to int
    vs = std::stoi(s);
    return;
}

void Header::setIN(std::vector<char> b) 
{
    // ensure correct number of bytes in argument b
    evalBufferSize(b, 4);

    // init string variable for storing interim result
    std::string s = "0";

    for (int i=0; i<b.size(); i++)
        s[i] = b[i];

    // cast from string to int
    vs = std::stoi(s);
    return;
}

void Header::setVV(std::vector<char> b) 
{
    // ensure correct number of bytes in argument b
    evalBufferSize(b, 4);

    // init string variable for storing interim result
    std::string s = "0";

    // VW identifier yields 4 bytes, of which the leftmost is IGNORED! hence, 
    // the loop starts at 0; however, s is a regular string starting at index 0.
    // Therefore, we counter the index-shift when assigning values to s!

    for (int i=1; i<b.size(); i++) // NOTE: index shift to ignore first byte!
        s[i-1] = b[i]; // NOTE: counter index shift in assignment

    // cast from string to int
    vv = std::stoi(s);
    return;
}



void Header::setMS(std::vector<char> b) 
{
    // ensure correct number of bytes in argument b
    evalBufferSize(b, 3);

    // init string variable for storing interim result
    std::string s = "0";

    for (int i=0; i<b.size(); i++)
        s[i] = b[i];

    // cast from string to int
    ms = std::stoi(s);
    return;
}

void Header::setText(std::string s)
{
    text = s;
    return;
}



/* MAPPING ------------------------------------------------------------------ */

// constructor A and B
MetaInfo::MetaInfo(){}
MetaInfo::MetaInfo(string k, int kl, int vl, int vi, MetaInfo::SetterFunction ss)
{
    key = k;
    keyLen = kl;
    valLen = vl;
    valIgn = vi;
    setter = ss; // https://en.wikipedia.org/wiki/Command_pattern
}

// destructor: for now empty, as no files opened, etc.
MetaInfo::~MetaInfo() {}

// standard getters
const string MetaInfo::getKey() const { return key; }
const int MetaInfo::getKeyLen() const { return keyLen; }
const int MetaInfo::getValLen() const { return valLen; }
const int MetaInfo::getLen() const { return (keyLen + valLen + valIgn); }

// special getter: associated setter function
MetaInfo::SetterFunction MetaInfo::getSetter() { return setter; }



/* -------------------------------------------------------------------------- */
