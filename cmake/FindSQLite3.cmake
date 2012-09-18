# - Try to find SQLite3
# Once done this will define
#
#  SQLITE3_FOUND - system has SQLite3
#  SQLITE3_INCLUDE_DIR - the SQLite3 include directory
#  SQLITE3_LIBRARIES - Link these to use SQLite3
#  SQLITE3_DEFINITIONS - Compiler switches required for using SQLite3

# Copyright (c) 2006, Alexander Neundorf, <neundorf@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


IF (SQLITE3_INCLUDE_DIR AND SQLITE3_LIBRARIES)
    SET(SQLite3_FIND_QUIETLY TRUE)
ENDIF (SQLITE3_INCLUDE_DIR AND SQLITE3_LIBRARIES)

FIND_PATH(SQLITE3_INCLUDE_DIR sqlite3.h)

FIND_LIBRARY(SQLITE3_LIBRARIES NAMES sqlite3)

# handle the QUIETLY and REQUIRED arguments and set SQLite3_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SQLite3 DEFAULT_MSG SQLITE3_LIBRARIES SQLITE3_INCLUDE_DIR)

MARK_AS_ADVANCED(SQLITE3_INCLUDE_DIR SQLITE3_LIBRARIES)

