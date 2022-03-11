# Bug 1

## A) How is your program acting differently than you expect it to?
- After letting the http333d.cc hang for a while (running the webserver) and putting queries, after some time the program would exit and say:

what():  basic_string::substr: __pos (which is 3) > this->size() (which is 0)
Aborted (core dumped)

## B) Brainstorm a few possible causes of the bug
- There were a few places we were calling substring. The error told us that at some point an empty string was being not checks and substring was being called, but that was about it. It did not tell us where the substring was failing. At first we thought it would be in the http3d.cc when substring gets called, and tried to put in a check for empty string there. But it was not there (plus the if statement already checked for this anyway)
- Another place substring was called was in HttpConnection. Here when a request was put in substring would be called. It could have been that here an empty string was not being checked for.
- Ditto for HttpServer, substring could get called on empty string before finding the suffix of the file.

## C) How you fixed the bug and why the fix was necessary
- Turns out it was the second option, we did not check to see if the end of request header was reached, so when it was it was trying to put this into a substring which was not valid. So we had to put the check:

  if (found == string::npos)
    return false;


# Bug 2

## A) How is your program acting differently than you expect it to?
- It was the same issue as the first bug with the core dump -- however, this time it would take a lot longer before the core dump would happen (presumably because the first bug had been fixed). Actually when we fixed the first bug we thought it was completely fixed until we let it hang for much longer and it got the core dump message again :(

## B) Brainstorm a few possible causes of the bug
- This time we let our previous bug fix influence this one. We could be more sure that the issue was in HttpConnection. Perhaps there was some other thing that we were not checking for that could have been part of the request that was not failing as it should? In ParseRequest we set the uri to be "BAD_" if it was not well formatted, this was not checked for.
- Could have been that some other part of ParseRequest was returning an empty string for an unchecked case, and this was not being checked for.
- Or it could have truly been in HttpServer as this still was not checked for empty string!!!

## C) How you fixed the bug and why the fix was necessary
- It was the fact that we did not catch the BAD_ label we set in ParseRequest. So we had to do:

  if ((*request).uri() == "BAD_") {
    return false;
  }


# Bug 3

## A) How is your program acting differently than you expect it to?
- HttpConnection test was failing. Line 196, getnextrequest was meant to return false but it was meant to be true.

## B) Brainstorm a few possible causes of the bug
- 
- 
- 

## C) How you fixed the bug and why the fix was necessary
- We forgot to check one case, if the first line had 3 tokens the format needed to be different.
