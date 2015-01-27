#include <Util/Xml/modXercesDom.h>
#include <Util/Xml/modXmlParser.h>

#include <map>

XERCES_CPP_NAMESPACE_USE;

using namespace eLibV2;

void parseNodes(std::vector<GenericNode*> *nodes, XERCES_CPP_NAMESPACE::DOMNode *node)
{
	if (nodes && node)
	{
		if (node->getNodeType() == DOMNode::ELEMENT_NODE)
		{
			StringMap attributes;
			if (node->hasAttributes())
			{
				char *nodename = XMLString::transcode(node->getNodeName());
				DOMNamedNodeMap *pAttributes = node->getAttributes();
				for (XMLSize_t i = 0; i < pAttributes->getLength(); ++i)
				{
					DOMAttr *pAttributeNode = (DOMAttr*)pAttributes->item(i);
					char *name = XMLString::transcode(pAttributeNode->getName());
					char *value = XMLString::transcode(pAttributeNode->getValue());
					attributes[name] = value;
					XMLString::release(&name);
					XMLString::release(&value);
				}
				nodes->push_back(new GenericNode(nodename, attributes));
				XMLString::release(&nodename);
			}

			if (node->hasChildNodes())
			{
				for (DOMNode *child = node->getFirstChild(); child != 0; child = child->getNextSibling())
					parseNodes(nodes, child);
			}
		}
	}
}

// TODO: const for reading stream
XmlParser::XmlParser(std::string filename)
{
	bool retval = true;

	try
	{
		XMLPlatformUtils::Initialize();
		DOMTreeErrorReporter *errReporter = new DOMTreeErrorReporter();
		XercesDOMParser *parser = new XercesDOMParser;

		parser->setValidationScheme(XercesDOMParser::Val_Auto);
		parser->setDoNamespaces(false);
		parser->setDoSchema(false);
		parser->setHandleMultipleImports(true);
		parser->setValidationSchemaFullChecking(false);
		parser->setCreateEntityReferenceNodes(false);

		parser->setErrorHandler(errReporter);
		parser->parse(filename.c_str());

		XERCES_CPP_NAMESPACE::DOMDocument *doc = parser->getDocument();
		XERCES_CPP_NAMESPACE::DOMNode *node = (DOMNode*)doc->getDocumentElement();

		nodes = new std::vector<GenericNode*>();
		parseNodes(nodes, node);

		delete errReporter;
		delete parser;
		XMLPlatformUtils::Terminate();
	}
	catch (...)
	{
		ModuleLogger::print("An error occurred during parsing");
	}
}
