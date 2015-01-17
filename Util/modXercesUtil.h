#ifndef MODXERCESUTIL_H_
#define MODXERCESUTIL_H_

#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

#include <iostream>

XERCES_CPP_NAMESPACE_USE

class StrX
{
public:
	StrX(const XMLCh* toTranscode) { fLocalForm = XMLString::transcode(toTranscode); }
	~StrX() { XMLString::release(&fLocalForm); }

	const char* localForm() const { return fLocalForm; }

private:
	char* fLocalForm;
};

inline XERCES_STD_QUALIFIER ostream& operator<<(XERCES_STD_QUALIFIER ostream& target, const StrX& toDump)
{
	target << toDump.localForm();
	return target;
}

#endif
