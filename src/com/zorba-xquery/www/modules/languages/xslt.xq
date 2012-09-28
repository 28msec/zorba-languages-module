xquery version "1.0";

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
 : This module provides XSLT 1.0 transformation functionality.  
 :
 : <p>For details on XSLT see 
 : <a href="http://www.w3.org/TR/xslt">XSLT 1.0 specification</a>.</p>  
 :
 : <p>This module implements the invoking of an XSLT transformation from XQuery
 : described in <a href="http://lists.w3.org/Archives/Member/w3c-xsl-wg/2008Apr/0052.html">
 :    Michael Kay's proposal</a>.</p>
 : 
 :
 : <p>Example:
 : <pre class="ace-static" ace-mode="xquery"><![CDATA[import module namespace
 :        xslt = "http://www.zorba-xquery.com/modules/languages/xslt";
 : 
 : let $source := 
 :     <catalog>
 :         <cd>
 :           <title>Empire Burlesque</title>
 :           <artist>Bob Dylan</artist>
 :           <country>USA</country>
 :           <company>Columbia</company>
 :           <price>10.90</price>
 :           <year>1985</year>
 :         </cd>
 :         <cd>
 :           <title>Hide your heart</title>
 :           <artist>Bonnie Tyler</artist>
 :           <country>UK</country>
 :           <company>CBS Records</company>
 :           <price>9.90</price>
 :           <year>1988</year>
 :         </cd>
 :     </catalog>
 : 
 : let $stylesheet := 
 :   <xsl:stylesheet version="1.0" 
 :       xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
 : 
 :     <xsl:template match="/">
 :       <html>
 :       <body>
 :       <h2>Music Collection&lt;/h2>
 :         <table border="1">
 :           <tr bgcolor="lightblue">
 :             <th>Title&lt;/th>
 :             <th>Artist&lt;/th>
 :           </tr>
 :           <xsl:for-each select="catalog/cd">
 :            <tr>
 :              <td>&lt;xsl:value-of select="title"/></td>
 :              <td>&lt;xsl:value-of select="artist"/></td>
 :           </tr>
 :           </xsl:for-each>
 :         </table>
 :       </body>
 :       </html>
 :     </xsl:template>
 :   </xsl:stylesheet>
 : 
 : return
 :   xslt:transform( $source, $stylesheet)]]></pre></p>
 :
 : @author Cezar Andrei
 : @library <a href="http://xmlsoft.org/XSLT/">libxslt Library</a>
 : @project Zorba/Programming Languages/XSLT
 :
 :)
module namespace xslt = "http://www.zorba-xquery.com/modules/languages/xslt";

declare namespace ver = "http://www.zorba-xquery.com/options/versioning";
declare option ver:module-version "1.0";

(:~
 :<p>Invokes an XSLT transformation.</p>
 :
 : @param $source the input document to the transformation
 : @param $stylesheet the XSLT stylesheet module
 : @return the result tree produced by the transformation
 :
 : @error  xslt:XSLT001 if $stylesheet is not a valid XSLT stylesheet
 : @error  xslt:XSLT002 if result can not be imported 
 :
 : @example test_xslt/Queries/languages/xslt/xslt1.xq
 :)
declare function xslt:transform (
  $source as node(),
  $stylesheet as node()
) as node() external;
