#ifndef MODXMLPARSER_H_
#define MODXMLPARSER_H_

#include <Util/Xml/GenericNode.h>

#include <string>
#include <iostream>
#include <vector>

namespace eLibV2
{
    namespace XML
    {
        typedef std::vector<GenericNode*> nodeVector;
        class XmlParser
        {
        public:
            XmlParser(void *memory, const unsigned int size);
            XmlParser(std::string filename);
            nodeVector* getNodes() { return nodes; }

        private:
            nodeVector *nodes;
        };
    }
}

#endif
