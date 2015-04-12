#ifndef MODGENERICNODE_H__
#define MODGENERICNODE_H__

#include <string>
#include <map>
#include <vector>

namespace eLibV2
{
    namespace XML
    {
        typedef std::map<std::string, std::string> StringMap;
        class GenericNode
        {
        public:
            GenericNode(std::string name, StringMap attributes)
            {
                mName = name;
                for (StringMap::iterator it = attributes.begin(); it != attributes.end(); it++)
                    mAttributes[it->first] = it->second;
            }

            std::string getName() { return mName; }
            StringMap getAttributes() { return mAttributes; }

        private:
            std::string mName;
            StringMap mAttributes;
        };

        typedef std::vector<GenericNode *> NodeVector;
    }
}

#endif
