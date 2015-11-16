#ifndef MODBASENAME_H_
#define MODBASENAME_H_

#include <string>

namespace eLibV2
{
    namespace Base
    {
        /**
        this module provides naming of its subclasses
        since it is a const, the setting has to be done during constructor initialization
        */
        class BaseName
        {
        public:
            BaseName(std::string name = "not initialized") :
                mModuleName(name)
            {}
            virtual ~BaseName() {}

            /**
            get name for module
            @return the name of the module
            */
            virtual std::string getModuleName(void) const { return mModuleName; }

        private:
            const std::string mModuleName; ///< internal module name
        };
    }
}

#endif
