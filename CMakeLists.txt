# Copyright 2006-2010 The FLWOR Foundation.
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

MESSAGE(STATUS "")
MESSAGE(STATUS "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
MESSAGE(STATUS "Configuring external module <languages>")
MESSAGE(STATUS "-------------------------------------------------------------")
MESSAGE(STATUS "")

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (zorba_languages_module)
ENABLE_TESTING ()
INCLUDE (CTest)

IF (WIN32)
  # On Windows we use proxy modules that try to guess first the location
  # of the required third party libraries. This will search in order in:
  # 1. the path pointed by ZORBA_THIRD_PARTY_REQUIREMENTS
  # 2. the Program Files directory available on the users computer
  # 3. the PATH environment variable
  # The logic is implemented by the macros in the ProxyFindModule.cmake module.
  LIST (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules/Windows")
ENDIF (WIN32)
LIST (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules")

FIND_PACKAGE (Zorba REQUIRED HINTS "${ZORBA_BUILD_DIR}")
INCLUDE ("${Zorba_USE_FILE}")

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
    
    MESSAGE(STATUS "")
    MESSAGE(STATUS "-------------------------------------------------------------")  
    MESSAGE(STATUS "Configuration FAILED for external module <languages>")
    
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
    
    ADD_SUBDIRECTORY("src")    
    ADD_TEST_DIRECTORY ("${PROJECT_SOURCE_DIR}/test_xslt")
    
    MESSAGE(STATUS "")
    MESSAGE(STATUS "-------------------------------------------------------------")
    MESSAGE(STATUS "Configuration SUCCEEDED for external module <languages>")

  ELSE (LIBXSLT_FOUND)
    MESSAGE (STATUS "LibXslt library not found -- if you want to use XSLT functionality please set LIBXSLT_INCLUDE_DIR and LIBXSLT_LIBRARIES cmake parameters.")
    
    MESSAGE(STATUS "")
    MESSAGE(STATUS "-------------------------------------------------------------")  
    MESSAGE(STATUS "Configuration FAILED for external module <languages>")
    
  ENDIF (LIBXSLT_FOUND)
ENDIF (ZORBA_SUPPRESS_LIBXSLT)
DONE_DECLARING_ZORBA_URIS()

MESSAGE(STATUS "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
