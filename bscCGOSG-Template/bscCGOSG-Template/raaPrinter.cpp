#include "stdafx.h"
#include "raaPrinter.h"

raaPrinter::raaPrinter(): osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
{
	
}

raaPrinter::~raaPrinter()
{
}

void raaPrinter::apply(osg::Node& node) 
{
	std::string s;
	for (unsigned int i = 0; i < getNodePath().size(); i++) s += "|--";

	std::cout << s << "Node -> " << node.getName() << " :: " << node.libraryName() << "::" << node.className() << std::endl;

	traverse(node);
}
