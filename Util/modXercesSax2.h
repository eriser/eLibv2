#ifndef MODXERCESSAX2_H_
#define MODXERCESSAX2_H_

#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

#include <Util/modXercesUtil.h>

#include <iostream>

XERCES_CPP_NAMESPACE_USE

class SAX2PrintHandlers : public DefaultHandler, private XMLFormatTarget
{
public:
	// -----------------------------------------------------------------------
	//  Constructors
	// -----------------------------------------------------------------------
	SAX2PrintHandlers(const char* encodingName, const XMLFormatter::UnRepFlags unRepFlags, const bool expandNamespaces);
	~SAX2PrintHandlers();

	void writeChars(const XMLByte* toWrite);
	virtual void writeChars(const XMLByte* toWrite, const XMLSize_t count, XMLFormatter* const formatter);

	void characters(const XMLCh* chars, const XMLSize_t length);
	void ignorableWhitespace(const XMLCh* chars, const XMLSize_t length);
	void processingInstruction(const XMLCh* target, const XMLCh* data);

	void startDocument();
	void startElement(const XMLCh* uri, const XMLCh* localname, const XMLCh* qname, const Attributes& attributes);
	void endElement(const XMLCh* uri, const XMLCh* localname, const XMLCh* qname);
	void endDocument();

	void warning(const SAXParseException& exc);
	void error(const SAXParseException& exc);
	void fatalError(const SAXParseException& exc);

	void notationDecl(const XMLCh* name, const XMLCh* publicId, const XMLCh* systemId);
	void unparsedEntityDecl(const XMLCh* name, const XMLCh* publicId, const XMLCh* systemId, const XMLCh* notationName);

private:
	XMLFormatter fFormatter;
	bool fExpandNS;
};

#endif