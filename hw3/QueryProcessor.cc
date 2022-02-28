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

#include "./QueryProcessor.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <set>
#include <string>
#include <vector>

#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>
#include <sstream>

extern "C" {
  #include "./libhw1/CSE333.h"
}

using std::list;
using std::set;
using std::sort;
using std::string;
using std::vector;

using std::cout;
using std::endl;
using std::stringstream;

namespace hw3 {

QueryProcessor::QueryProcessor(const list<string>& index_list, bool validate) {
  // Stash away a copy of the index list.
  index_list_ = index_list;
  array_len_ = index_list_.size();
  Verify333(array_len_ > 0);

  // Create the arrays of DocTableReader*'s. and IndexTableReader*'s.
  dtr_array_ = new DocTableReader* [array_len_];
  itr_array_ = new IndexTableReader* [array_len_];

  // Populate the arrays with heap-allocated DocTableReader and
  // IndexTableReader object instances.
  list<string>::const_iterator idx_iterator = index_list_.begin();
  for (int i = 0; i < array_len_; i++) {
    FileIndexReader fir(*idx_iterator, validate);
    dtr_array_[i] = fir.NewDocTableReader();
    itr_array_[i] = fir.NewIndexTableReader();
    idx_iterator++;
  }
}

QueryProcessor::~QueryProcessor() {
  // Delete the heap-allocated DocTableReader and IndexTableReader
  // object instances.
  Verify333(dtr_array_ != nullptr);
  Verify333(itr_array_ != nullptr);
  for (int i = 0; i < array_len_; i++) {
    delete dtr_array_[i];
    delete itr_array_[i];
  }

  // Delete the arrays of DocTableReader*'s and IndexTableReader*'s.
  delete[] dtr_array_;
  delete[] itr_array_;
  dtr_array_ = nullptr;
  itr_array_ = nullptr;
}

// This structure is used to store a index-file-specific query result.
typedef struct {
  DocID_t doc_id;  // The document ID within the index file.
  int     rank;    // The rank of the result so far.
} IdxQueryResult;

vector<QueryProcessor::QueryResult>
QueryProcessor::ProcessQuery(const vector<string>& query) const {
  Verify333(query.size() > 0);

  // STEP 1.
  // (the only step in this file)
  vector<QueryProcessor::QueryResult> final_result;

  DocTableReader* doc_table_reader; // per index iteration (i)
  IndexTableReader* index_table_reader; // per index iteration (i)
  DocIDTableReader* doc_id_table_reader; // per word in index iteration (j)
  
  // iterate through list of index files
  for (int i = 0; i < array_len_; i++) {
    // get corresponding doctablereader and indextable reader for this file
    doc_table_reader = dtr_array_[i];
    index_table_reader = itr_array_[i];

    list<DocIDTableReader*> doc_ID_tables;
    set<DocID_t> doc_IDs;
    list<DocIDElementHeader> ID_headers; // headers for this ID
    // iterate through list of words in query
    for (uint j = 0; j < query.size(); j++) {
      doc_id_table_reader = index_table_reader->LookupWord(query[j]);
      if (doc_id_table_reader == nullptr) {
        // no match found so move onto next index file
        // clear ID headers (all words in query must be present in document)
        doc_ID_tables.clear();
        break;
      }
      // it was found in the index...
      // add doc ID table
      doc_ID_tables.push_back(doc_id_table_reader);

      // add headers in this doc id list to Doc ID set
      for (DocIDElementHeader header : doc_id_table_reader->GetDocIDList()) {
        doc_IDs.insert(header.doc_id);
        ID_headers.push_back(header);
      }
    }

    // look at doc ID list, if its empty go to next index file.
    if (doc_ID_tables.empty()) {
      continue;
    }

    // iterate through doc IDs, make sure that all document IDs that get added to final result exist in
    // all of the doc ID tables
    list<DocIDElementHeader> ID_headers_final;
    for (DocID_t id : doc_IDs) {
      bool exists_in_all_tables = true;
      for (DocIDTableReader* doc_id_table : doc_ID_tables) {
        list<DocPositionOffset_t> dummy;
        if (!doc_id_table->LookupDocID(id, &dummy)) {
          exists_in_all_tables = false;
        }
      }

      if (exists_in_all_tables) {
        for (DocIDElementHeader header : ID_headers) {
          if (header.doc_id == id) {
            ID_headers_final.push_back(header);
          }
        }
      }
    }

    // if there were results for this doc id, then we go and
    // use the data from the headers for the list of queryresults
    for (DocIDElementHeader header : ID_headers_final) {
      QueryResult result;
      bool found_result = false;
      // convert docID to filename, write to query result
      doc_table_reader->LookupDocID(header.doc_id, &result.document_name);
      // update rank for document
      result.rank = header.num_positions;

      for (QueryResult &res : final_result) {
        if (res.document_name.compare(result.document_name) == 0) {
          res.rank += result.rank;
          found_result = true;
          break;
        }
      }

      if (!found_result) {
        // add new query result to final list
        final_result.push_back(result);
      }
    }
  }

  // Sort the final results.
  sort(final_result.begin(), final_result.end());
  return final_result;
}

}  // namespace hw3
