#include <Util/modLogger.h>

#include <string>
#include <iostream>

class XmlParser
{
public:
	bool parseDOM(std::string filename);
	bool parseSAX(std::string filename);
};

