The PAWS implementation requires installation of library called cxxtool, which is
available at http://www.tntnet.org/cxxtools.html. The version used is copied 
inside the files. 

Server.zip contains all the codes used for server operation.
Client.zip contains codes used for client. 

The Cxxtools provides framework for Remote-RPC including the HTTP connection, and
in the project page, it also has some tutorial about how to use their library.

Server and Client currently works fine within one computer, using self-loop IP. I 
have tested the case when database resides on a different computer, and each one
of them are identified by their IP address.