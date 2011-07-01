(:
 : Copyright 2006-2009 The FLWOR Foundation.
 :
 : Licensed under the Apache License, Version 2.0 (the "License");
 : you may not use this file except in compliance with the License.
 : You may obtain a copy of the License at
 :
 : http://www.apache.org/licenses/LICENSE-2.0
 :
 : Unless required by applicable law or agreed to in writing, software
 : distributed under the License is distributed on an "AS IS" BASIS,
 : WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 : See the License for the specific language governing permissions and
 : limitations under the License.
:)

(:~
 : This module provides XSLT transformation functionality. 
 :
 : See Mickael Kay's proposal:
 : http://lists.w3.org/Archives/Member/w3c-xsl-wg/2008Apr/0052.html
 :
 : Example:
 : import module namespace
 :     xslt = "http://www.zorba-xquery.com/modules/languages/xslt"; 
 :
 : xslt:transform(doc("cat.xml"), doc("t1.xslt"))
 :
 :
 : @author Cezar Andrei
 : @library <a href="http://xmlsoft.org/XSLT/">libxslt Library</a>
 : @project data-processing/data-convertors
 :
 :)
module namespace xslt = "http://www.zorba-xquery.com/modules/languages/xslt";


(:~
 :<p>Tranforms the $source by applying the XSLT stylesheet</p>
 :
 : @param $source the input document
 : @param $stylesheet the XSLT stylesheet
 : @return the result
 :)
declare function xslt:transform (
  $source as node(),
  $stylesheet as node()
) as node() external;
