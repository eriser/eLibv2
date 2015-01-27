#ifndef MODXERCESDOM_H_
#define MODXERCESDOM_H_

#include <msxml.h>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/dom/DOMLSSerializerFilter.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>

#include <Util/Xml/modXercesUtil.h>

#include <string.h>
#include <stdlib.h>

#include <iostream>

XERCES_CPP_NAMESPACE_USE

class DOMPrintErrorHandler : public DOMErrorHandler
{
public:

	DOMPrintErrorHandler(){};
	~DOMPrintErrorHandler(){};

	/** @name The error handler interface */
	bool handleError(const DOMError& domError);
	void resetErrors(){};

private:
	/* Unimplemented constructors and operators */
	DOMPrintErrorHandler(const DOMErrorHandler&);
	void operator=(const DOMErrorHandler&);
};

class DOMPrintFilter : public DOMLSSerializerFilter {
public:

	DOMPrintFilter(ShowType whatToShow = DOMNodeFilter::SHOW_ALL);
	~DOMPrintFilter(){};

	virtual FilterAction acceptNode(const DOMNode*) const;
	virtual ShowType getWhatToShow() const { return fWhatToShow; };

private:
	// unimplemented copy ctor and assignement operator
	DOMPrintFilter(const DOMPrintFilter&);
	DOMPrintFilter & operator = (const DOMPrintFilter&);

	ShowType fWhatToShow;
};

class DOMTreeErrorReporter : public ErrorHandler
{
public:
	// -----------------------------------------------------------------------
	//  Constructors and Destructor
	// -----------------------------------------------------------------------
	DOMTreeErrorReporter() : fSawErrors(false) {}
	~DOMTreeErrorReporter() {}

	void warning(const SAXParseException& toCatch);
	void error(const SAXParseException& toCatch);
	void fatalError(const SAXParseException& toCatch);
	void resetErrors();

	bool getSawErrors() const;
	bool fSawErrors;
};

inline bool DOMTreeErrorReporter::getSawErrors() const
{
	return fSawErrors;
}

#endif
