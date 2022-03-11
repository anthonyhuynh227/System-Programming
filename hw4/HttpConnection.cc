/*
 * Copyright ©2022 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2022 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdint.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <map>
#include <string>
#include <vector>

#include "./HttpRequest.h"
#include "./HttpUtils.h"
#include "./HttpConnection.h"

using std::map;
using std::string;
using std::vector;
using boost::split;
using boost::token_compress_on;
using boost::is_any_of;
using boost::trim;
using boost::to_lower;
#define BUFSIZE 1024

namespace hw4 {

static const char* kHeaderEnd = "\r\n\r\n";
static const int kHeaderEndLen = 4;

bool HttpConnection::GetNextRequest(HttpRequest* const request) {
  // Use WrappedRead from HttpUtils.cc to read bytes from the files into
  // private buffer_ variable. Keep reading until:
  // 1. The connection drops
  // 2. You see a "\r\n\r\n" indicating the end of the request header.
  //
  // Hint: Try and read in a large amount of bytes each time you call
  // WrappedRead.
  //
  // After reading complete request header, use ParseRequest() to parse into
  // an HttpRequest and save to the output parameter request.
  //
  // Important note: Clients may send back-to-back requests on the same socket.
  // This means WrappedRead may also end up reading more than one request.
  // Make sure to save anything you read after "\r\n\r\n" in buffer_ for the
  // next time the caller invokes GetNextRequest()!

  // STEP 1:
  size_t found = buffer_.find(kHeaderEnd);
  // if end of request header not in this request...
  if (found == string::npos) {
    // perform wrapped read

    // Loop, reading data from client_fd and writing it to stdout.
    unsigned char buf[BUFSIZE];
    int res;
    while (1) {
      res = WrappedRead(fd_, buf, BUFSIZE);
      if (res == 0) { // connection closed
        break;
      }
      if (res < 0) {  // error
        return false;
      }
      // append read to buffer
      buffer_ += string(reinterpret_cast<char*>(buf), res);

      // check buffer to see if "\r\n\r\n" is there
      // if it is then stop reading
      found = buffer_.find(kHeaderEnd);
      if (found != string::npos) {
        break;
      }
    }
  }

  if (found == string::npos)
    return false;

  // put header into output param
  *request = ParseRequest(buffer_.substr(0,found + kHeaderEndLen));

  // return false if the request is not well-formatted
  if ((*request).uri() == "BAD_") {
    return false;
  }

  // Make sure to save anything you read after "\r\n\r\n" in buffer_
  // for the next time the caller invokes GetNextRequest()
  
  buffer_ = buffer_.substr(found + kHeaderEndLen);

  return true;  // You may want to change this.
}

bool HttpConnection::WriteResponse(const HttpResponse& response) const {
  string str = response.GenerateResponseString();
  int res = WrappedWrite(fd_,
                         reinterpret_cast<const unsigned char*>(str.c_str()),
                         str.length());
  if (res != static_cast<int>(str.length()))
    return false;
  return true;
}

HttpRequest HttpConnection::ParseRequest(const string& request) const {
  HttpRequest req("/");  // by default, get "/".

  // Plan for STEP 2:
  // 1. Split the request into different lines (split on "\r\n").
  // 2. Extract the URI from the first line and store it in req.URI.
  // 3. For the rest of the lines in the request, track the header name and
  //    value and store them in req.headers_ (e.g. HttpRequest::AddHeader).
  //
  // Hint: Take a look at HttpRequest.h for details about the HTTP header
  // format that you need to parse.
  //
  // You'll probably want to look up boost functions for:
  // - Splitting a string into lines on a "\r\n" delimiter
  // - Trimming whitespace from the end of a string
  // - Converting a string to lowercase.
  //
  // Note: If a header is malformed, skip that line.

  // STEP 2:
  // split the request into different lines (split on "\r\n").
  vector<string> lines;
  split(lines, request, is_any_of("\r\n"),
                      token_compress_on);
  // check if request is in correct form with \r\n
  if (lines.size() < 2) {
    req.set_uri("BAD_");
    return req;
  }

  // trimming whitespace from the end of each line
  for (size_t i = 0; i < lines.size(); i++) {
    trim(lines[i]);
  }

  // split first line into tokens on " "
  vector<string> fst_line;
  split(fst_line, lines[0], is_any_of(" "),
                  token_compress_on);

  // check the format the first line
  if (fst_line.size() == 0) {
    // we get default: GET/ HTTP/ 1.1
  }  else if ( fst_line[0] != "GET") {
    // check if the first line of the request is GET
    req.set_uri("BAD_");
    return req;
  } else if (fst_line.size() == 2) {
    // if the first line has 2 tokens, it should be either 
    // GET [URI] \r\n or GET HTTP/ 
    if (fst_line[0] != "GET" ||
        (fst_line[1][0] != '/' &&
         fst_line[1].find("HTTP/") == string::npos)) {
      req.set_uri("BAD_");
      return req;
    }
  } else if (fst_line.size() == 3) {
    // if the first line has 3 tokens, it should be 
    // in format GET [URI] HTTP/..., otherwise, it's not correct formatted.
    if (fst_line[0] != "GET" ||
        fst_line[1][0] != '/' ||
        fst_line[2].find("HTTP/") == string::npos) {
      req.set_uri("BAD_");
      return req;
    }
    req.set_uri(fst_line[1]);
  } else {
    // if number of tokens in the first line is not 2 or 3,
    // then the request is not well-formatted.
    req.set_uri("BAD_");
    return req;
  }

  // split the rest of the lines in the request into header name and value.
  for (size_t i = 1; i < lines.size() -1; i++) {
    size_t col = lines[i].find(": ");

    // check if each line is in correct format
    if (col == string::npos) {
      req.set_uri("BAD_");
      return req;
    }

    // convert headername to lowercase and store them in req.header_
    string header_name = lines[i].substr(0, col);
    to_lower(header_name);
    string header_value = lines[i].substr(col +2);
    req.AddHeader(header_name, header_value);

  }

  return req;
}

}  // namespace hw4
