//To COMPILE BELOW CODE:  g++ -c httpServerLib.cpp -I ..\include\ -I c:\cServerClient\serverUtilityLibrary\include\ -lserverUtilityLib -L c:\cServerClient\serverUtilityLibrary\src\ -lws2_32 -std=c++11


#include<stdio.h>
#include<httpServerLib.h>

using namespace tmServer;

string BaseLocation::baseDir="";
void BaseLocation:: set(string location)
{
baseDir=location;
}

string BaseLocation::get()
{
return baseDir;
}

/*
ServiceResolver* ServiceResolverObject::serviceResolver=NULL;
void ServiceResolverObject::set(ServiceResolver* sr)
{
serviceResolver=sr;
}
ServiceResolver* ServiceResolverObject::get()
{
return serviceResolver;
}
*/


HttpServer::HttpServer()
{
//nothing
}

void HttpServer::initialize()
{
this->winsockVersion=MAKEWORD(2,2);
this->winsockState=WSAStartup(winsockVersion,&winsockData);
if(winsockState!=0)
{
this->lastOperationFailed=true;
this->error=string("Unable to initialize socket library");
return;
}
else
{
this->lastOperationFailed=false;
this->error="";
}
}

SOCKET HttpServer::createServerSocket()
{
this->serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if(serverSocketDescriptor<0)
{
this->lastOperationFailed=true;
this->error=string("Unable to initialize socket library");
WSACleanup();
return this->serverSocketDescriptor;
}
else
{
this->lastOperationFailed=false;
this->error=string("");
return this->serverSocketDescriptor;
}
}

void HttpServer::bindToPort(int portNumber)
{
this->serverPortNumber=portNumber;
this->serverAddress.sin_family=AF_INET;
this->serverAddress.sin_port=htons(serverPortNumber);
this->serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
this->bindState=bind(this->serverSocketDescriptor,(struct sockaddr*)&serverAddress,sizeof(struct sockaddr));
if(bindState<0)
{
this->lastOperationFailed=true;
this->error=string("unable to bind");

WSACleanup();
return;
}
else
{
this->lastOperationFailed=false;
this->error=string("");
}
}

void HttpServer::listenToRequest()
{
listen(serverSocketDescriptor,SOMAXCONN);
printf("server is in listening mode...\n");
}


void HttpServer::acceptRequest(Socket &soc)
{
soc.clientAddressLen=sizeof(soc.clientAddress);
soc.clientSocketDescriptor=accept(this->serverSocketDescriptor,(struct sockaddr*)&(soc.clientAddress),&(soc.clientAddressLen));

if(soc.clientSocketDescriptor<0)
{
bool lastOperationFailed=true;
this->error=string("Client connected, but unable to accept client");
WSACleanup();
}
else
{
bool lastOperationFailed=false;
this->error=string("");
printf("client connected\n");
}
}


bool HttpServer::failed()
{
return lastOperationFailed;
}

string HttpServer::getError()
{
return error;
}

HttpServer::~HttpServer()
{
closesocket(serverSocketDescriptor);
WSACleanup();
}

void HttpServer::close()
{
closesocket(serverSocketDescriptor);
WSACleanup();
}

//----------------------------------------------------------------------------------------------

Socket::Socket()
{
//nothing
}

void Socket::receiveData()
{
this->clientPortNumber=ntohs(clientAddress.sin_port);
memcpy(&clientIPAddress,&clientAddress.sin_addr.s_addr,4);
this->clientIP=inet_ntoa(clientIPAddress);
printf("Client connected from : %s to port %d RECEVE DATA CHALI\n",clientIP,clientPortNumber);

int bytesReceived=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer),0);

printf("RECEVING DATA receiveData function chala\n");

if(bytesReceived<0)
{
lastOperationFailed=true;
error=string("Unable to extract bytes\n");

cout<<"DID NOT RECEVE DATA"<<endl;
closesocket(clientSocketDescriptor);
return;
}
else
{
lastOperationFailed=false;
error=string("");
}
requestBuffer[bytesReceived]='\0';
printf("*********************************\n");
printf("RECEVED DATA IN BUFFER====%s\n",requestBuffer);
printf("*********************************\n");
}



string Socket::getRequest()
{
char r[5000];
int i,j;
if(requestBuffer[0]=='G' && requestBuffer[1]=='E' && requestBuffer[2]=='T')
{
 for(i=4,j=0;requestBuffer[i]!=' ';i++,j++)
 {
 r[j]=requestBuffer[i];
 }
}
if(requestBuffer[0]=='P' && requestBuffer[1]=='O' && requestBuffer[2]=='S' && requestBuffer[3]=='T')
{
 for(i=5,j=0;requestBuffer[i]!=' ';i++,j++)
 {
 r[j]=requestBuffer[i];
 }
}
r[j]='\0';

string request=r;
return request;
}


string Socket::getRequestType()
{
/*
if(requestBuffer[0]=='G' && requestBuffer[1]=='E' && requestBuffer[2]=='T')
{
return string("GET");
}
if(requestBuffer[0]=='P' && requestBuffer[1]=='O' && requestBuffer[2]=='S' && requestBuffer[3]=='T')
{
return string("POST");
}
return "Unknown"; //later will be changed
*/

char type[20];
int i=0;
for(i=0;requestBuffer[i]!=' ' && requestBuffer[i]!='\0' && requestBuffer[i]!='\n';i++)
{
type[i]=requestBuffer[i];
}
type[i]='\0';

return string(type);
}


string Socket::getRequestHost()
{
int i;
for(i=0;requestBuffer[i]!='\n';i++);
i+=1;
char host[100];
int j;
for(j=0,i=i+6;requestBuffer[i]!='\n';j++,i++)
{
host[j]=requestBuffer[i];
}
host[j]='\0';
string h=host;
return h;
}


void Socket::setRequestStatus(int status)
{
this->requestStatus=status;
}


int Socket::getRequestStatus()
{
string location=BaseLocation::get();
string req=this->getRequest();
char path[5000];
int i=0;

if(strlen(req.c_str())==0)
{
this->requestStatus=400;
return this->requestStatus;
}

for(i=0;req[i]!='\n' && req[i]!='?' && req[i]!='\0';i++)
{
path[i]=req[i];
}
path[i]='\0';
location=location+string(path);
FILE *file=NULL;
const char* absPath=location.c_str();
strcpy(path,absPath);
file=fopen(path,"r");

if(strcmp(this->getRequestContentType().c_str(),"serverSide")==0)
{
return this->requestStatus;
}
else if(file==NULL)
{
this->requestStatus=404;
return this->requestStatus;
}
else
{
this->requestStatus=200;
return this->requestStatus;
fclose(file);
}
}



string Socket::getAbsolutePath()
{
string location=BaseLocation::get();
if(strlen(requestBuffer)==0)
{
return location;
}
string req=this->getRequest();
char path[5000];
int i=0;
for(i=0;req[i]!='\n' && req[i]!='?' && req[i]!='\0';i++)
{
path[i]=req[i];
}
path[i]='\0';
location=location+string(path);
return location;
}


string Socket::getRequestContentType()
{
if(strlen(requestBuffer)==0)
{
return "undefined type";
}

string path=this->getAbsolutePath();
const char *p=path.c_str();
int i,j;
for(i=0;p[i]!='\0';i++);
i--;
char ext[10];
for(j=0,i=i;p[i]!='.';i--,j++)
{
ext[j]=p[i];
}
ext[j]='\0';
i=0;
j--;
char temp;
while(i<j)
{
temp=ext[i];
ext[i]=ext[j];
ext[j]=temp;
i++;
j--;
}
const char* extension=ext;

if(strcmp(extension,"txt")==0 || strcmp(extension,"doc")==0 || strcmp(extension,"docx")==0)
{
return "text/plain";
}
if(strcmp(extension,"css")==0)
{
return "text/css";
}
if(strcmp(extension,"htm")==0 || strcmp(extension,"html")==0)
{
return "text/html";
}
if(strcmp(extension,"js")==0)
{
return "text/javascript";
}
if(strcmp(extension,"apng")==0) 
{
return "image/apng";
}
if(strcmp(extension,"jpeg")==0 || strcmp(extension,"jpg")==0 || strcmp(extension,"jfif")==0 || strcmp(extension,"pjpeg")==0 || strcmp(extension,"pjp")==0)
{
return "image/jpeg";
}
if(strcmp(extension,"gif")==0)
{
return "image/gif";
}
if(strcmp(extension,"ico")==0 || strcmp(extension,"cur")==0)
{
return "image/x-icon";
}
if(strcmp(extension,"bmp")==0)
{
return "image/bmp";
}
if(strcmp(extension,"png")==0)
{
return "image/png";
}
if(strcmp(extension,"svg")==0)
{
return "image/svg+xml";
}
if(strcmp(extension,"tif")==0 || strcmp(extension,"tiff")==0)
{
return "image/tiff";
}
if(strcmp(extension,"webp")==0)
{
return "image/webp";
}
if(strcmp(extension,"ss")==0)
{
return "serverSide";
}
return "application/octet-stream";
}




char* Socket::getRequestContent(int &aSize)
{
if(strlen(requestBuffer)==0)
{
aSize=0;
this->lastOperationFailed=true;
this->error=string("bad request, can't fetch content as no file found");
return NULL;
}
else
{
this->lastOperationFailed=false;
this->error="";
}

string p=this->getAbsolutePath();
const char* absPath=p.c_str();
char * bytes;
FILE* file;
string type=this->getRequestContentType();
int size;
if(type.find("text")!=string::npos)
{
 file=fopen(absPath,"r");
  cout<<"FILE	 OPEND IN R MODE"<<endl;
 if(file==NULL)
 {
 this->lastOperationFailed=true;
 this->error=string("file not found on path= ")+absPath; 
 cout<<"File not found on path=="<<p<<endl;
 return NULL;
 }
 else
 {
 this->lastOperationFailed=false;
 this->error="";
 }


 fseek(file,0,SEEK_END);
 size=ftell(file);
 fseek(file,0,SEEK_SET);
 bytes=(char*)malloc(sizeof(char)*(size+1));
/* while(!feof(file))
 {
  fgets(read,4000,file);
  strcat(*bytes,read);
 }
*/
 fread(bytes,1,size,file);
 bytes[size]='\0';
 aSize=size+1;
 fclose(file);
}
else 
{
 file=fopen(absPath,"rb");

 if(file==NULL)
 {
 this->lastOperationFailed=true;
 this->error=string("file not found on path= ")+p;
 return NULL;
 }
 else
 {
 this->lastOperationFailed=false;
 this->error="";
 } 

// fseek(file,0,SEEK_END);
// size=ftell(file);
// fseek(file,0,SEEK_SET);

//code to find image file size
 struct stat filestat;
 int fileDescriptor;
 if (((fileDescriptor = _open(absPath, _O_RDONLY)) < -1) || (fstat(fileDescriptor, &filestat) < 0) ) 
 {
  this->lastOperationFailed=true;
  this->error=("file not found on path= ")+string(p);
  return NULL;
 }
 else
 {
  this->lastOperationFailed=false;
  this->error="";
 }

 size=filestat.st_size;
 _close(fileDescriptor); 
  
 bytes=(char*)malloc(sizeof(char)*(size+1));
 fread(bytes,1,size+1,file);
 cout<<"SIZE OF IMAGE FILE=="<<size<<endl;
 cout<<"================================================"<<endl;
 fclose(file);
 aSize=size;
}

return bytes;
}



char* Socket::getFileContent(string filePath,int &aSize)
{
FILE* file;
char* bytes;
int size;
const char *absPath=filePath.c_str();
 file=fopen(absPath,"r");
 if(file==NULL)
 {
 this->lastOperationFailed=true;
 this->error=string("file not found on path= ")+string(absPath);
 return NULL;
 }
 else
 {
 this->lastOperationFailed=false;
 this->error="";
 }

 fseek(file,0,SEEK_END);
 size=ftell(file);
 fseek(file,0,SEEK_SET);
 bytes=(char*)malloc(sizeof(char)*(size+1));
/* while(!feof(file))
 {
  fgets(read,4000,file);
  strcat(*bytes,read);
 }
*/
 fread(bytes,1,size,file);
 bytes[size]='\0';
 aSize=size+1;
 fclose(file);
 return bytes;
}


char* Socket::getImageContent(string filePath,int &aSize)
{
FILE* file;
char* bytes;
int size;
const char* absPath=filePath.c_str();
 file=fopen(absPath,"rb");
 if(file==NULL)
 {
 this->lastOperationFailed=true;
 this->error=string("file not found on path= ")+string(absPath);
 return NULL;
 }
 else
 {
 this->lastOperationFailed=false;
 this->error="";
 } 

// fseek(file,0,SEEK_END);
// size=ftell(file);
// fseek(file,0,SEEK_SET);

//code to find image file size
 struct stat filestat;
 int fileDescriptor;
 if (((fileDescriptor = _open(absPath, _O_RDONLY)) < -1) || (fstat(fileDescriptor, &filestat) < 0) ) 
 {
  this->lastOperationFailed=true;
  this->error=("file not found on path= ")+string(absPath);
  return NULL;
 }
 else
 {
  this->lastOperationFailed=false;
  this->error="";
 }

 size=filestat.st_size;
 _close(fileDescriptor); 
  
 bytes=(char*)malloc(sizeof(char)*(size+1));
 fread(bytes,1,size+1,file);
 fclose(file);
 aSize=size;
return bytes;
}




string Socket::getRequestHeader()
{
string requestHeader;
size_t found;
string request_buffer=this->requestBuffer;
found=request_buffer.find("\n\n");
if(found!=string::npos)
{
requestHeader=request_buffer.substr(0,found);
return requestHeader;
}
else
return request_buffer;
}



string Socket::getRequestBuffer()
{
string request_buffer=this->requestBuffer;
return request_buffer;
}



void Socket::sendResponse(int aStatus,string CT,char* content,string cookie,int size)
{
char status[50]="HTTP/1.1 ";
char contentType[100]="Content-Type: ";
char contentLength[40]="Content-Length: ";
char data[1000000];
char to_send[1000500];

to_send[0]='\0';

char contentL[15];
if(content!=NULL)
{
int x;
for(x=0;x<size;x++)
{
data[x]=content[x];
}

int len=size;
stringstream strs;
  strs << len;
  string length = strs.str();
const char* l=length.c_str();
strcpy(contentL,l);
cout<<"SIZE OF CONTENT TO SEND"<<contentL<<endl;
}

int lengthToSend;
if(aStatus==404)
{
strcat(status,"404 Not Found\n");
strcat(contentType,CT.c_str());
strcat(contentType,"\n");
strcat(contentLength,"0");
strcat(contentLength,"\n\n");
strcat(to_send,status);
strcat(to_send,contentType);
if(strcmp(cookie.c_str(),"")!=0) strcat(to_send,cookie.c_str());
strcat(to_send,contentLength);
strcat(to_send,"");
}
else
{
strcat(status,"200 OK\n");
strcat(contentType,CT.c_str());
strcat(contentType,"\n");
cout<<"CONTENT TYPE++"<<contentType;
strcat(contentLength,contentL);
strcat(contentLength,"\n\n");
strcat(to_send,status);
strcat(to_send,contentType);
if(strcmp(cookie.c_str(),"")!=0) strcat(to_send,cookie.c_str());
strcat(to_send,contentLength);
//strcat(to_send,data);

int i,j;
i=0;j=0;
for(i=0;to_send[i]!='\0';i++);
for(j=0;j<size;j++,i++)
{
to_send[i]=data[j];
}
to_send[i]='\0';
}

lengthToSend=strlen(status)+strlen(contentType)+strlen(contentLength)+size+cookie.length();


if(send(this->clientSocketDescriptor,to_send,lengthToSend,0)<0)
{
this->lastOperationFailed=true;
this->error=string("can't send data");
}
else
{
this->lastOperationFailed=false;
this->error="";
}

free(content);
}


/*

void Socket::processGetRequest()
{
printf("PROCESS GET REQUEST MAI AAYA\n");
this->request=new Request();
this->response=new Response();
string str=this->getRequest();
string url;
url=str;
//------code to parse the query string and get all attributes
size_t found=str.find("?");
string substring;
string name,value;
if(found!=string::npos)
{

url=str.substr(0,found-1);

size_t index=0;
index=str.find("&");
if(index!=string::npos)
 substring=str.substr(found+1,index-found-1);
else
 substring=str.substr(found+1);

cout<<substring<<endl;
int i;
i=substring.find("=");
name=substring.substr(0,i);
value=substring.substr(i+1);
cout<<name<<" "<<value<<endl;
request->setParameter(name,value);

size_t index2;
if(index!=string::npos)
{
 while(1)
 {
 index2=str.find("&",index+1);
 if(index2==string::npos)
 { 
  substring=str.substr(index+1);
  cout<<substring<<endl;
  int i;
  i=substring.find("=");
  name=substring.substr(0,i);
  value=substring.substr(i+1);
  cout<<name<<" "<<value<<endl;
  request->setParameter(name,value);
  break;
 }
 else
 {
  substring=str.substr(index+1,index2-index-1);
  index=index2;
  cout<<substring<<endl;
  int i;
  i=substring.find("=");
  name=substring.substr(0,i);
  value=substring.substr(i+1);
  cout<<name<<" "<<value<<endl;
  request->setParameter(name,value);
 }
 }
}
}
//code toparse query string ends------------

ServiceResolver* sr=ServiceResolverObject::get();

cout<<"Service resolver object in function=="<<sr<<endl;

cout<<"SIZE OF SR=="<<sizeof(*sr)<<endl;

Service* service=sr->getService(url);

cout<<"Service object in function=="<<service<<endl;

if(service==NULL)
{
requestStatus=404;
}
else
{
service->processGetService(request,response);
requestStatus=200;
}
}



void Socket::processPostRequest()
{
this->request=new Request();
this->response=new Response();
string str=this->getRequest();
string url;
url=str;

//----parsing of post request
size_t found=str.find("\n\n");
string substring;
string name,value;
if(found!=string::npos)
{
size_t index=found+1;

size_t index2;
if(index!=string::npos)
{
 while(1)
 {
 index2=str.find("&",index+1);
 if(index2==string::npos)
 { 
  substring=str.substr(index+1);
  cout<<substring<<endl;
  int i;
  i=substring.find("=");
  name=substring.substr(0,i);
  value=substring.substr(i+1);
  cout<<name<<" "<<value<<endl;
  break;
 }
 else
 {
  substring=str.substr(index+1,index2-index-1);
  index=index2;
  cout<<substring<<endl;
  int i;
  i=substring.find("=");
  name=substring.substr(0,i);
  value=substring.substr(i+1);
  cout<<name<<" "<<value<<endl;
 }
 }
}
}
//------parsing of post request ends

ServiceResolver* sr=ServiceResolverObject::get();
Service* service=sr->getService(url);
if(service==NULL)
{
requestStatus=404;
}
else
{
service->processPostService(request,response);
requestStatus=200;
}

}


*/



void Socket::close()
{
closesocket(clientSocketDescriptor);
}

bool Socket::failed()
{
return lastOperationFailed;
}

string Socket::getError()
{
return this->error;
}

Socket::~Socket()
{
closesocket(clientSocketDescriptor);
}