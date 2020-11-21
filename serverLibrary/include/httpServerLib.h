#ifndef HTTPSERVER__$__
#define HTTPSERVER__$__
#include<iostream>
#include<winsock2.h>
#include<bits/stdc++.h>
#include<string>
#include<thread>
#include<stdlib.h>
//for finding size of image file
#include <sys/stat.h>
#include <io.h>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */


//#pragma comment(lib,"ws2_32.lib")
using namespace std;

namespace tmServer
{

/*
class ServiceResolverObject
{
private:
static ServiceResolver* serviceResolver;
public:
static void set(ServiceResolver* seviceRes);
static ServiceResolver* get();
};

*/

class BaseLocation
{
private:
static string baseDir;
public:
static void set(string loc);
static string get();
};


class Socket;
class HttpServer
{
private:
WORD winsockVersion;
WSADATA winsockData;
int winsockState;
int serverSocketDescriptor;       //we can also write it as SOCKET serverSocketDescriptor
struct sockaddr_in serverAddress;
struct sockaddr_in clientAddress;
int serverPortNumber;

int bindState;
bool lastOperationFailed;
string error;

public:
HttpServer();
void initialize();
SOCKET createServerSocket();    
void bindToPort(int portNumber);
void listenToRequest();
void acceptRequest(Socket& soc);

bool failed();
string getError();
void close();
virtual ~HttpServer();
//void* operator new(size_t size);
//void operator delete(void* d);
};

class Socket
{
private:
struct sockaddr_in clientAddress;
int clientSocketDescriptor;
int clientAddressLen;
int bindState;
struct in_addr clientIPAddress;
char *clientIP;
int clientPortNumber;
char requestBuffer[50000];
bool lastOperationFailed;
string error;

int requestStatus;


public:
Socket();
void receiveData();
string getRequest();
string getRequestType();
string getRequestHost();
int getRequestStatus();
void setRequestStatus(int status);
string getRequestHeader();
string getRequestBuffer();
char* getRequestContent(int &size);
char* getFileContent(string filePath,int &size);
char* getImageContent(string filePath,int &size);
string getAbsolutePath();
string getRequestContentType();
void sendResponse(int status,string contentType,char* content,string cookie,int size);

/*
void processGetRequest();
void processPostRequest();
char* getGeneratedResponse(int &size);
string getGeneratedResponseContentType();
*/

bool failed();
string getError();
virtual ~Socket();
void close();
//void* operator new(size_t size);
//void operator delete(void* d);

friend class HttpServer;
};


}
#endif