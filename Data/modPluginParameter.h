#ifndef PLUGINPARAMETER_H_
#define PLUGINPARAMETER_H_

// std headers
#include <string>
#include <sstream>
#include <vector>

namespace eLibV2
{
    enum ParameterType
    {
        ParameterTypeBoolean = 1,
        ParameterTypeInteger,
        ParameterTypeDouble,
    };

    class PluginParameter
    {
    public:
        PluginParameter(const std::string name, const std::string label, ParameterType type, double minValue, double maxValue, double initValue = 0.0);

        std::string getName() const { return mParameterName; }
        std::string getLabel() const { return mUnitLabel; }
        double getValue(const double in) const;
        double getInitialValue() const { return mInitialValue; }
        std::string getValueAsString(const double in) const;

        double getRawValue(const double in) const;

        double getMinValue() const { return mMinValue; }
        double getMaxValue() const { return mMaxValue; }

        bool isBooleanType() const { return (mType == ParameterTypeBoolean); }
        bool isIntegerType() const { return (mType == ParameterTypeInteger); }
        bool isDoubleType() const { return (mType == ParameterTypeDouble); }

    private:
        std::string mParameterName;
        std::string mUnitLabel;
        ParameterType mType;
        double mMinValue;
        double mMaxValue;
        double mInitialValue;
    };

    typedef std::vector<PluginParameter> PluginParameters;
    typedef PluginParameters::iterator ParameterIterator;
}

#endif
