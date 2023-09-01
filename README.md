# Parser for RV-Produkt ASCII-Headers

The goal of this C++ project is to implement a *positional* parser for 
*RV-Produkt* file headers. 
These files are provided by the German Weather Service DWD and describe the forecasted amount of precipitation in mm/h over the next 120min.

## Usage
Call `./prvh <file>` in Terminal/CLI to **p**arse **RV**-Produkt **h**eader.

**Example**

```sh
$./prvh DE1200_RV_LATEST/DE1200_RV2108242045_005
ASCII Header of 'DE1200_RV_LATEST/DE1200_RV2108242045_005'
 PI:   RV
 TS:   2108242045
 WMO:  10000
 SW:   P200003H
 PR:   E-02
 GP:   200x1100
 MF:   00000008
 MS:   103
 TX:   <deasb,deboo,dedrs,deeis,deess,defbg,defld,dehnr,deisn,demem,deneu,denhb,deoft,depro,deros,detur,deumd>
```

## Technical Details

### ASCII Header

The *RV Produkt* files include a so called **ASCII header**, which features **ASCII-encoded (!)** file meta data.
These headers are common in specific scientific or engineering domains, because they allow humans to glean some context about the file's contents without specialized tools. 
Simply open the file in a text editor or call `head -c 64 <file>` in Terminal/CLI, where  `-c` specifies the number of bytes to print.


Hence (and to give an explicit example), the `DDhhmm` measurement timestamp in UTC at bytes `3` to `5` is given in **ASCII encoding**:
```
0x32 0x34   0x32 0x30   0x34 0x35
```
translates to ASCII characters:
```
2 4   2 0   4 5
```

The end of the header is signalled by a `0x03` byte ("ETX-Oktett"), which translates to `ETX` in ASCII and represents the `end of text` control character ([Wiki](https://en.wikipedia.org/wiki/End-of-Text_character)).
 		

### Resources

Official DWD documentation and other useful resources:
- https://dwd-geoportal.de/products/RADAR_RV/
- https://www.dwd.de/DE/leistungen/radarprodukte/radarkomposit_rv.pdf
- https://www.dwd.de/DE/leistungen/radarprodukte/formatbeschreibung_rv.pdf
- ASCII Table via [DuckDuckGo](https://duckduckgo.com/?t=ffab&q=ascii+table&ia=answer&iax=answer)


## Implementation Details

### Modern C++ Features

Due to the use of `std::tuple<...>` for return function types, this project should be compiled with `-std=c++17`.

### Log4Cxx Setup

On MacOS ...
1. download latest version from [the official website](https://logging.apache.org/log4cxx/latest_stable/download.html) and verify the integrity
2. ensure `cmake` is installed (if not, run `brew install cmake`)
3. build log4cxx as described [here](https://logging.apache.org/log4cxx/latest_stable/build-cmake.html) with:
```sh
cd apache-Log4cxx-x.x.x
mkdir build
cd build
cmake ..
sudo make install
```
4. link the headers and library in the `Makefile`, e.g.:
```make
-llog4cxx -I/usr/local/include/log4cxx -L/usr/local/lib
```

### Integration via Pointers (approach A)

Include the following code into `main.cpp`:
```cpp
using namespace log4cxx;
using namespace log4cxx::helpers;

LoggerPtr logger(Logger::getLogger("MyApp"));

int main() 
{
    BasicConfigurator::configure();
    
    LOG4CXX_INFO(logger, "This is an informational message.");
}
```

To access the logger from other files, include the following:
```cpp
#include <log4cxx/logger.h>
using namespace log4cxx;

void someFunction() {
    LOG4CXX_INFO(logger, "This is info from someFunction");
    //...
}
```

### Integration via Singleton (approach B)

For more complex applications, it might be beneficial to have a dedicated singleton that abstracts the logger's configuration and usage.
For this approach, create the following files:

```cpp
//logger.h
#include <log4cxx/logger.h>

class Logger {
private:
    Logger() = default; // c++11
    static log4cxx::LoggerPtr logger; // static member!

public:
    static log4cxx::LoggerPtr& getInstance();
};
```

```cpp
// logger.cpp
#include "logger.h"
#include <log4cxx/basicconfigurator.h>

// init static member variable at program begin → no constructor!
log4cxx::LoggerPtr Logger::logger;

log4cxx::LoggerPtr& Logger::getInstance() {
    if (!logger) {
        log4cxx::BasicConfigurator::configure();
        logger = log4cxx::Logger::getLogger("App");
    }
    return logger;
}
```

Access the logger in any `.cpp` file via:

```cpp
#include "logger.h"

void someFunction() {
    LOG4CXX_INFO(Logger::getInstance(), "This is info from someFunction");
    //...
}
```

### Development

A command such as `make && ./prvh DE1200_RV_LATEST/DE1200_RV2108242045_005` for building and running on a example input file may be helpful in development.
