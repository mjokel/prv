#pragma once

#include <map>
#include <string>
#include <vector>

using namespace std;


/* HEADER ------------------------------------------------------------------- */

class Header {
private:

    // name of file
    std::string filename;

    // product id ("Produktkennung")
    std::string productId;

    // file time stamp in YYMMDDhhmm format
    std::string timestamp;

    // "WMO-Nummer"
    std::string wmo;

    // "Produktlänge in Byte"
    int by;

    // "Format-Version"
    int vs;

    // "Software-Version"
    std::string sw;

    // "Genauigkeit der Daten"
    std::string pr;

    // "Intervalldauer in Minuten"
    int in;

    // "Anzahl der Pixel"
    std::string gp;
    
    // "Vorhersagezeitpunkt"
    int vv;

    // "Modulflags"
    std::string mf;

    // Textlaenge m
    int ms;

    // text "<...>""
    std::string text;


public:
    Header();                 // constructor
    ~Header();                // destructor

    // to string
    friend std::ostream& operator<<(std::ostream& os, const Header& h);

    const std::string getFN() const;
    const std::string getPI() const;
    const std::string getTS() const;
    const std::string getWN() const;
    
    const int getBY() const;
    const int getVS() const;
    const std::string getSW() const;
    const std::string getPR() const;
    const int getIN() const;
    const std::string getGP() const;
    const int getVV() const;
    const std::string getMF() const;
    const int getMS() const;
    const std::string getText() const;


    void setFN(std::string s);
    void setPI(std::vector<char> b);
    void setTS(char ts[]);
    void setWN(std::vector<char> b);
    
    void setBY(std::vector<char> b);
    void setVS(std::vector<char> b);
    void setSW(std::vector<char> b);
    void setPR(std::vector<char> b);
    void setIN(std::vector<char> b);
    void setGP(std::vector<char> b);
    void setVV(std::vector<char> b);
    void setMF(std::vector<char> b);
    void setMS(std::vector<char> b);
    void setText(std::string);


    // helpers
    int evalBufferSize(std::vector<char> b, int n);

};


/* METAINFO ----------------------------------------------------------------- */

// short for "Metainformation"

class MetaInfo {

public:
    // typedef for a member function pointer
    using SetterFunction = void (Header::*)(std::vector<char>);  


private:
    // the key ("Kennung") is at most 3 chars long
    string key;

    // number of bytes per field
    int keyLen;
    int valLen;

    // number of bytes of the val field to ignore (from leftmost position)
    int valIgn;

    // typedef for a member function pointer -> associated setter
    SetterFunction setter;


public:
    MetaInfo();
    MetaInfo(string, int, int, int, SetterFunction);
    ~MetaInfo();

    const string getKey() const;
    const int getKeyLen() const;
    const int getValLen() const;
    const int getLen() const;

    SetterFunction getSetter();
};


/* -------------------------------------------------------------------------- */
