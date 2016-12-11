char* not_found_response_template = 
      "HTTP/1.1 404 Not Found\n"
      "Content-type: text/html\n"
      "\r"
      "\n"
      "\r"
      "\n"
      "<html>\n"
      " <body>\n"
      "  <h1>Not Found</h1>\n"
      "  <p>The requested URL was not found on this server.</p>\n"
      " </body>\n"
      "</html>\n";

char* DEFAULT_RESPONSE_SUCCESS_TEMPLATE = 
    "HTTP/1.1 200 Success\n"
    "\r"
    "\n"
    "\r"
    "\n";
char* METHOD_NOT_ALLOWED = 
    "HTTP/1.1 405 Method Not Allowed\n"
    "\r"
    "\n"
    "\r"
    "\n";
