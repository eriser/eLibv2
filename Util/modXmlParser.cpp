#include <Util/modXercesDom.h>
#include <Util/modXercesSax2.h>
#include <Util/modXmlParser.h>

XERCES_CPP_NAMESPACE_USE;

using namespace eLibV2;

bool XmlParser::parseDOM(std::string filename)
{
	int retval = 0;
	try
	{
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException &toCatch)
	{
		ModuleLogger::print("Error during Xerces-c Initialization. Exception message: %s", StrX(toCatch.getMessage()));
		return 1;
	}

	XercesDOMParser *parser = new XercesDOMParser;
	parser->setValidationScheme(XercesDOMParser::Val_Auto);
	parser->setDoNamespaces(false);
	parser->setDoSchema(false);
	parser->setHandleMultipleImports(true);
	parser->setValidationSchemaFullChecking(false);
	parser->setCreateEntityReferenceNodes(false);

	DOMTreeErrorReporter *errReporter = new DOMTreeErrorReporter();
	parser->setErrorHandler(errReporter);

	bool errorsOccured = false;
	try
	{
		parser->parse(filename.c_str());
	}
	catch (const OutOfMemoryException&)
	{
		ModuleLogger::print("OutOfMemoryException");
		errorsOccured = true;
	}
	catch (const XMLException& e)
	{
		ModuleLogger::print("An error occurred during parsing. Message: %s", StrX(e.getMessage()));
		errorsOccured = true;
	}
	catch (const DOMException& e)
	{
		const unsigned int maxChars = 2047;
		XMLCh errText[maxChars + 1];

		ModuleLogger::print("DOM Error during parsing: '%s' DOMException code is: %li", filename.c_str(), e.code);

		if (DOMImplementation::loadDOMExceptionMsg(e.code, errText, maxChars))
			ModuleLogger::print("Message is: %s", StrX(errText));

		errorsOccured = true;
	}
	catch (...)
	{
		ModuleLogger::print("An error occurred during parsing");
		errorsOccured = true;
	}

	// If the parse was successful, output the document data from the DOM tree
	if (!errorsOccured && !errReporter->getSawErrors())
	{
		DOMPrintFilter *myFilter = 0;

		try
		{
			// get a serializer, an instance of DOMLSSerializer
			XMLCh tempStr[3] = { chLatin_L, chLatin_S, chNull };
			DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);
			DOMLSSerializer   *theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();
			DOMLSOutput       *theOutputDesc = ((DOMImplementationLS*)impl)->createLSOutput();

			// set user specified output encoding
			theOutputDesc->setEncoding(0);

			DOMErrorHandler *myErrorHandler = new DOMPrintErrorHandler();
			DOMConfiguration* serializerConfig = theSerializer->getDomConfig();
			serializerConfig->setParameter(XMLUni::fgDOMErrorHandler, myErrorHandler);

			if (serializerConfig->canSetParameter(XMLUni::fgDOMWRTSplitCdataSections, true))
				serializerConfig->setParameter(XMLUni::fgDOMWRTSplitCdataSections, true);

			if (serializerConfig->canSetParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true))
				serializerConfig->setParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true);

			if (serializerConfig->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, false))
				serializerConfig->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, false);

			if (serializerConfig->canSetParameter(XMLUni::fgDOMWRTBOM, false))
				serializerConfig->setParameter(XMLUni::fgDOMWRTBOM, false);

			XMLFormatTarget *myFormTarget;
			myFormTarget = new StdOutFormatTarget();
			theOutputDesc->setByteStream(myFormTarget);

			XERCES_CPP_NAMESPACE::DOMDocument *doc = parser->getDocument();

			theSerializer->write(doc, theOutputDesc);

			theOutputDesc->release();
			theSerializer->release();

			delete myFormTarget;
			delete myErrorHandler;
		}
		catch (const OutOfMemoryException&)
		{
			ModuleLogger::print("OutOfMemoryException");
			retval = 5;
		}
		catch (XMLException& e)
		{
			ModuleLogger::print("An error occurred during creation of output transcoder. Msg is: %s", StrX(e.getMessage()));
			retval = 4;
		}
	}
	else
		retval = 4;

	delete errReporter;
	delete parser;

	XMLPlatformUtils::Terminate();

	return retval;
}

bool XmlParser::parseSAX(std::string filename)
{
	try
	{
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch)
	{
		ModuleLogger::print("Error during initialization!");
		return 1;
	}

	SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();

	parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
	parser->setFeature(XMLUni::fgXercesDynamic, true);
	parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
	parser->setFeature(XMLUni::fgXercesSchema, true);
	parser->setFeature(XMLUni::fgXercesHandleMultipleImports, true);
	parser->setFeature(XMLUni::fgXercesSchemaFullChecking, false);
	parser->setFeature(XMLUni::fgSAX2CoreNameSpacePrefixes, false);

	int errorCount = 0, errorCode = 0;
	try
	{
		SAX2PrintHandlers handler("LATIN1", XMLFormatter::UnRep_CharRef, false);
		parser->setContentHandler(&handler);
		parser->setErrorHandler(&handler);
		parser->parse(filename.c_str());
		errorCount = parser->getErrorCount();
	}
	catch (const OutOfMemoryException&)
	{
		ModuleLogger::print("OutOfMemoryException");
		errorCode = 1;
	}
	catch (const XMLException& toCatch)
	{
		ModuleLogger::print("An error occurred");
		errorCode = 2;
	}
	XMLPlatformUtils::Terminate();

	if ((errorCount > 0) || (errorCode != 0))
		return false;
	else
		return true;
}
