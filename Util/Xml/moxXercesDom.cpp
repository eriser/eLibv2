#include <Util/Xml/modXercesDom.h>

bool DOMPrintErrorHandler::handleError(const DOMError &domError)
{
	if (domError.getSeverity() == DOMError::DOM_SEVERITY_WARNING)
		XERCES_STD_QUALIFIER cerr << "\nWarning Message: ";
	else if (domError.getSeverity() == DOMError::DOM_SEVERITY_ERROR)
		XERCES_STD_QUALIFIER cerr << "\nError Message: ";
	else
		XERCES_STD_QUALIFIER cerr << "\nFatal Message: ";

	char *msg = XMLString::transcode(domError.getMessage());
	XERCES_STD_QUALIFIER cerr << msg << XERCES_STD_QUALIFIER endl;
	XMLString::release(&msg);

	return true;
}

static const XMLCh  element_person[] =
{
	chLatin_p, chLatin_e, chLatin_r, chLatin_s, chLatin_o, chLatin_n, chNull
};

static const XMLCh  element_link[] =
{
	chLatin_l, chLatin_i, chLatin_n, chLatin_k, chNull
};

DOMPrintFilter::DOMPrintFilter(ShowType whatToShow)
	:fWhatToShow(whatToShow)
{}

DOMNodeFilter::FilterAction DOMPrintFilter::
acceptNode(const DOMNode* node) const
{
	if ((getWhatToShow() & (1 << (node->getNodeType() - 1))) == 0)
		return DOMNodeFilter::FILTER_ACCEPT;

	switch (node->getNodeType())
	{
		case DOMNode::ELEMENT_NODE:
			if (XMLString::compareString(node->getNodeName(), element_person) == 0)
				return DOMNodeFilter::FILTER_SKIP;
			if (XMLString::compareString(node->getNodeName(), element_link) == 0)
				return DOMNodeFilter::FILTER_REJECT;
			return DOMNodeFilter::FILTER_ACCEPT;

		case DOMNode::COMMENT_NODE:
			return DOMNodeFilter::FILTER_REJECT;

		case DOMNode::TEXT_NODE:
			return DOMNodeFilter::FILTER_REJECT;

		case DOMNode::DOCUMENT_TYPE_NODE:
			return DOMNodeFilter::FILTER_REJECT;  // no effect

		case DOMNode::DOCUMENT_NODE:
			return DOMNodeFilter::FILTER_REJECT;  // no effect

		default:
			return DOMNodeFilter::FILTER_ACCEPT;
	}

	return DOMNodeFilter::FILTER_ACCEPT;
}

void DOMTreeErrorReporter::warning(const SAXParseException&)
{
}

void DOMTreeErrorReporter::error(const SAXParseException& toCatch)
{
	fSawErrors = true;
	XERCES_STD_QUALIFIER cerr << "Error at file \"" << StrX(toCatch.getSystemId())
		<< "\", line " << toCatch.getLineNumber()
		<< ", column " << toCatch.getColumnNumber()
		<< "\n   Message: " << StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void DOMTreeErrorReporter::fatalError(const SAXParseException& toCatch)
{
	fSawErrors = true;
	XERCES_STD_QUALIFIER cerr << "Fatal Error at file \"" << StrX(toCatch.getSystemId())
		<< "\", line " << toCatch.getLineNumber()
		<< ", column " << toCatch.getColumnNumber()
		<< "\n   Message: " << StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void DOMTreeErrorReporter::resetErrors()
{
	fSawErrors = false;
}
