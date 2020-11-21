g++ -c ..\serverLibrary\src\httpServerLib.cpp -I ..\serverLibrary\include\ -o httpServerLib.o -lws2_32 -std=c++11  

ar rcs httpServerLib.lib httpServerLib.o 

g++ -c -I ..\cServerUtilityLibrary\include\  ..\cServerUtilityLibrary\src\cServerUtilityLib.cpp   -o cServerUtilityLib.o  -lws2_32 -std=c++11  


ar rcs cServerUtilityLib.lib  cServerUtilityLib.o  


g++ -I ..\serverLibrary\include\ -I ..\cServerUtilityLibrary\include\ -I ..\server\include\  -c ..\server\src\myServer.cpp -o myServer.o -std=c++11 -lws2_32

ar rcs myServer.lib myServer.o

//g++ -static c:\cServerClient\server\myServer.cpp -lhttpServerLib  -L c:\cServerClient\serverLibrary\src\ -lserverUtilityLib -L c:\cServerClient\serverUtilityLibrary\src\ -o c:\cServerClient\server\myServer.exe -I c:\cServerClient\serverLibrary\include\ -I c:\cServerClient\serverUtilityLibrary\include\  -std=gnu++11 -lws2_32 
