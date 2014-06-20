#ifndef VSTBASEPARAMETER_H_
#define VSTBASEPARAMETER_H_

// std headers
#include <string>
#include <sstream>

// vst headers
#include <audioeffectx.h>

namespace eLibV2
{
    namespace VSTBase
    {
        enum ParameterType
        {
            ParameterTypeDouble = 1,
            ParameterTypeInteger
        };

        class VSTBaseParameter
        {
        public:
            VSTBaseParameter();
            VSTBaseParameter(const std::string name, const std::string label, double minValue, double maxValue, double initValue = 0.0);
            VSTBaseParameter(const std::string name, const std::string label, ParameterType type, double minValue, double maxValue, double initValue = 0.0);
            ~VSTBaseParameter() {}

            std::string getName() { return mParameterName; }
            std::string getLabel() { return mUnitLabel; }
            std::string getValue(const double in);

            double getMinValue() const { return mMinValue; }
            double getMaxValue() const { return mMaxValue; }

            bool isIntegerType() const { return (mType == ParameterTypeInteger); }

        private:
            std::string mParameterName;
            std::string mUnitLabel;
            ParameterType mType;
            double mMinValue;
            double mMaxValue;
            double mInitialValue;
            bool mCanBeAutomated;
        };
    }
}

#endif /* VSTBASEPARAMETER_H_ */