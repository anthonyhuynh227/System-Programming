/*
 * Copyright ©2022 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to the students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2022 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdio.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>

extern "C" {
  #include "libhw2/FileParser.h"
}

#include "./HttpUtils.h"
#include "./FileReader.h"

using std::string;

namespace hw4 {

bool FileReader::ReadFile(string* const contents) {
  string full_file = basedir_ + "/" + fname_;

  // Read the file into memory, and store the file contents in the
  // output parameter "str."  Be careful to handle binary data
  // correctly; i.e., you probably want to use the two-argument
  // constructor to std::string (the one that includes a length as a
  // second argument).
  //
  // You might find ::ReadFileToString() from HW2 useful here. Remember that
  // this function uses malloc to allocate memory, so you'll need to use
  // free() to free up that memory after copying to the string output
  // parameter.
  //
  // Alternatively, you can use a unique_ptr with a malloc/free
  // deleter to automatically manage this for you; see the comment in
  // HttpUtils.h above the MallocDeleter class for details.

  // Return false if:
  // 1. The file could not be found or opened
  // 2. The file exists above the basedir in the file system hierarchy (e.g.,
  //    filename is "../../../escape.html")

  // STEP 1:

  // false cond 2
  if (!IsPathSafe(basedir_, full_file)) {
    return false;
  }

  int file_size;
  char* file_str = ReadFileToString(full_file.c_str(), &file_size);

  // false cond 1
  if (file_str == NULL) {
    return false;
  }

  // output param
  *contents = string(file_str, file_size);
  // The caller is responsible for freeing the returned pointer
  // in ReadFileToString
  free(file_str);

  return true;
}

}  // namespace hw4
