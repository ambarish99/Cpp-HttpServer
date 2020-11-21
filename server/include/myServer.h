#ifndef __MYSERVER__$__
#define __MYSERVER__$__
#include<iostream>
#include<httpServerLib.h>
#include<cServerUtilityLib.h>

using namespace std;
using namespace tmServer;
using namespace cServerUtility;
namespace myServer
{

typedef struct run
{
void (*function)(REQUEST*,RESPONSE*);
}RUN;

typedef struct process
{
RUN* (*getProcessor)(const char*);
}PROCESS;

class MyServer
{
private:
int port;
HttpServer server;
PROCESS *process;

public:
void startServer(int portNumber,PROCESS* process);

private:
void handleRequest();

void processGetRequest(Socket &socket,REQUEST *request,RESPONSE *response);
void processPostRequest(Socket &socket,REQUEST *request,RESPONSE *response);
char* getGeneratedResponse(int &size,RESPONSE* response);
string getGeneratedResponseContentType(RESPONSE* response);
void setReceivedCookieInRequest(Socket &socket,REQUEST* request);
string getGeneratedCookieFromResponse(RESPONSE* response);

};



}




#endif