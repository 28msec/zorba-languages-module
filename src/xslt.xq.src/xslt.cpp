/*
 * Copyright 2006-2008 The FLWOR Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <zorba/empty_sequence.h>
#include <zorba/singleton_item_sequence.h>
#include <zorba/item.h>

#include "xslt.h"

#include <stdio.h>
#include <string.h>
#include <cassert>

#include <zorba/serializer.h>
#include <zorba/user_exception.h>

#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#include <libxml/xinclude.h>
#include <libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>


namespace zorba 
{
  namespace xsltmodule
  {

/*****************************************************************************
 * TransformFunction implementation
 ***************************************************************************/

ItemSequence_t
TransformFunction::evaluate(
  const ExternalFunction::Arguments_t& aArgs,
  const StaticContext*                 aSctxCtx,
  const DynamicContext*                aDynCtx) const
{
  Item lSourceItem, lStylesheetItem;

  assert(aArgs.size() == 2);

  Iterator_t lArg0Iter = aArgs[0]->getIterator();
  lArg0Iter->open();
  lArg0Iter->next(lSourceItem);
  lArg0Iter->close();

  Iterator_t lArg1Iter = aArgs[1]->getIterator();
  lArg1Iter->open();
  lArg1Iter->next(lStylesheetItem);
  lArg1Iter->close();

  Zorba_SerializerOptions_t lOptions;
  Serializer_t lSerializer = Serializer::createSerializer(lOptions);
  
  std::stringstream lResultStream;
  SingletonItemSequence lSrcSequence(lSourceItem);
  lSerializer->serialize(&lSrcSequence, lResultStream);
  std::string lSource = lResultStream.str();            
    
  lResultStream.str("");
    
  SingletonItemSequence lStyleSequence(lStylesheetItem);
  lSerializer->serialize(&lStyleSequence, lResultStream);
  std::string lStylesheet = lResultStream.str();
  

  // start using libxslt       
  xsltStylesheetPtr cur = NULL;
  xmlDocPtr stylesheetDoc, instDoc, res;
  const char *params[0 + 1];
  int nbparams = 0;
  params[nbparams] = NULL;

  xmlChar* ssStr = BAD_CAST lStylesheet.c_str(); //xmlCharStrdup(stylesheet.c_str());
  stylesheetDoc = xmlParseDoc( ssStr );
  cur = xsltParseStylesheetDoc(stylesheetDoc);
  
  xmlChar* srcStr = BAD_CAST lSource.c_str(); //xmlCharStrdup(source.c_str());
  instDoc = xmlParseDoc( srcStr );
  res = xsltApplyStylesheet(cur, instDoc, params);
  //for debuging: int transforResult = xsltSaveResultToFile(stdout, res, cur);
  xmlChar* xcRes;
  int len;
  int transforResult = xsltSaveResultToString(&xcRes, &len, res, cur);

  xsltFreeStylesheet(cur);
  //avoid doing xmlFreeDoc(stylesheetDoc);
  xmlFreeDoc(res);
  xmlFreeDoc(instDoc);
  
  xsltCleanupGlobals();
  xmlCleanupParser();

  std::string resStr( (char *) xcRes);   
  xmlFree(xcRes);
  
  if ( transforResult != 0 )
  {
      std::stringstream lSs;
      lSs << "Libxslt error: " << resStr.c_str();
      Item lQName = XsltModule::getItemFactory()->createQName(this->getURI(), "XSLT001");
      throw USER_EXCEPTION( lQName, lSs.str());
  }
  
  std::istringstream iss( resStr.c_str() ); 
  
  XmlDataManager_t lDM = Zorba::getInstance(0)->getXmlDataManager();
  try
  {
    return ItemSequence_t(new SingletonItemSequence(lDM->parseXML(iss)));   
  } 
  catch (ZorbaException& e)
  {
      std::stringstream lSs;
      lSs << "Cannot serialize error: " << e;
      Item lQName = XsltModule::getItemFactory()->createQName(this->getURI(), "XSLT002");
      throw USER_EXCEPTION( lQName, lSs.str());
  }
}



/*****************************************************************************
 * XsltModule implementation
 ***************************************************************************/

ItemFactory* XsltModule::theFactory = 0;

XsltModule::~XsltModule()
{
  for ( FuncMap_t::const_iterator lIter = theFunctions.begin();
        lIter != theFunctions.end();
        ++lIter)
       {
         delete lIter->second;
       }
       theFunctions.clear();
}

ExternalFunction*
XsltModule::getExternalFunction(const String& aLocalname)
{
  ExternalFunction*& lFunc = theFunctions[aLocalname];
  if (!lFunc)
  {
    if (1 == 0)
    { }
    else if (aLocalname == "transform")
    {
      lFunc = new TransformFunction(this);
    }
  }
  return lFunc;
}

void
XsltModule::destroy()
{
  if (!dynamic_cast<XsltModule*>(this))
  {
    return;
  }
  delete this;
}


  } /* namespace xsltmodule */
} /* namespace zorba */

#ifdef WIN32
#  define DLL_EXPORT __declspec(dllexport)
#else
#  define DLL_EXPORT __attribute__ ((visibility("default")))
#endif

extern "C" DLL_EXPORT zorba::ExternalModule* createModule()
{
  return new zorba::xsltmodule::XsltModule();
}
