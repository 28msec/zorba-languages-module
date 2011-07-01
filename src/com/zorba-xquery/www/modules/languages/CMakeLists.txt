# Copyright 2006-2008 The FLWOR Foundation.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


#
# LibXslt (requires LibXml2)
#
IF (ZORBA_SUPPRESS_LIBXSLT)
  MESSAGE (STATUS "ZORBA_SUPPRESS_LIBXSLT is true - not searching for LibXslt.")
ELSE (ZORBA_SUPPRESS_LIBXSLT)

  #
  # LibXml2
  #
  IF (NOT LIBXML2_FOUND)
    MESSAGE (STATUS "Looking for LibXml2")
    FIND_PACKAGE (LibXml2)
  ENDIF (NOT LIBXML2_FOUND)
         
  IF (LIBXML2_FOUND)
    MESSAGE (STATUS "Found LibXml2 library -- " ${LIBXML2_LIBRARIES})
      
    INCLUDE_DIRECTORIES (${LIBXML2_INCLUDE_DIR})
    SET (requiredlibs-store ${requiredlibs-store} ${LIBXML2_LIBRARIES})
    SET (CXXFLAGS ${CXXFLAGS} ${LIBXML2_DEFINITONS})

  ELSE (LIBXML2_FOUND)
    MESSAGE (FATAL_ERROR "The libxml2 library and headers are required in order to build xslt module.")
  ENDIF (LIBXML2_FOUND)
  MESSAGE (STATUS "")

  #
  # LibXslt
  #
  MESSAGE (STATUS "Looking for LibXslt")
  FIND_PACKAGE (LibXslt)

  IF (LIBXSLT_FOUND)
    MESSAGE (STATUS "Found LibXslt library -- " ${LIBXSLT_LIBRARIES})

    INCLUDE_DIRECTORIES (${LIBXSLT_INCLUDE_DIR})
    SET (requiredlibs ${requiredlibs} ${LIBXSLT_LIBRARIES})
    SET (CXXFLAGS ${CXXFLAGS} ${LIBXSLT_DEFINITONS})
    INCLUDE_DIRECTORIES ("xslt.xq.src")
    GENERATE_MODULE_LIBRARY (xslt "${LIBXSLT_LIBRARIES}")
    ADD_TEST_DIRECTORY ("${PROJECT_SOURCE_DIR}/test_xslt")

  ELSE (LIBXSLT_FOUND)
    MESSAGE (STATUS "LibXslt library not found -- if you want to use XSLT functionality please set LIBXSLT_INCLUDE_DIR and LIBXSLT_LIBRARIES cmake parameters.")
  ENDIF (LIBXSLT_FOUND)
ENDIF (ZORBA_SUPPRESS_LIBXSLT)
MESSAGE (STATUS "")

