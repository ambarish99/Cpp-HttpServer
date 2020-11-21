//TO COMPILE : g++ -I c:\cServerClient\serverLibrary\include\ -I c:\cServerClient\cServerUtilityLibrary\include\ -I ..\include\  -c myServer.cpp -std=c++11 -lws2_32

#include<httpServerLib.h>
#include<stdlib.h>
#include<myServer.h>

using namespace tmServer;
using namespace cServerUtility;
using namespace myServer;

void MyServer::startServer(int portNo,PROCESS* process)
{
this->port=portNo;
this->process=process;

BaseLocation::set("C:/cServerClient/apps");

server.initialize();
if(server.failed())
{
printf("ERROR =======%s\n",server.getError().c_str());
return;
}
SOCKET s=server.createServerSocket();
if(server.failed())
{
printf("ERROR =======%s\n",server.getError().c_str());
return;
}
server.bindToPort(port);
if(server.failed())
{
printf("ERROR =======%s\n",server.getError().c_str());
return;
}

while(1)
{
server.listenToRequest();
this->handleRequest();
}
}//startServer


void MyServer::handleRequest()
{
Socket client;

REQUEST *request;
RESPONSE *response;

request=new REQUEST();
response=new RESPONSE();

printf("handle request called\n");

if(response->responseText==NULL)
 printf("RESPONSE INNITAILLY==is NULL\n");
else
 printf("RESPONSE==%s\n",response->responseText);

server.acceptRequest(client);
if(server.failed())
{
cout<<"ERROR ======="<<server.getError()<<endl;
freeRequest(request);
freeResponse(response);
return;
}
client.receiveData();
if(client.failed())
{
freeRequest(request);
freeResponse(response);
cout<<"ERROR ======="<<server.getError()<<endl;
return;
}
cout<<"received data"<<endl;

string req=client.getRequest();
cout<<"Request is== "<<req<<endl;

string type=client.getRequestType();
cout<<"Request type is== "<<type<<endl;

string host=client.getRequestHost();
cout<<"host is=="<<host<<endl;


string abspath=client.getAbsolutePath();
cout<<"absolute path=="<<abspath<<endl;


string contentType=client.getRequestContentType();
cout<<"requested content type== "<<contentType<<endl;


//If request is for server side-----

int length;
char* content;
string responseContentType;
string cookies;

if(strcmp(contentType.c_str(),"serverSide")==0)
{
 if(strcmp(type.c_str(),"GET")==0)
 {
cout<<"GET HAI REQUEST SERVER SIDE KI HAI"<<endl;
  this->processGetRequest(client,request,response);

cout<<"PROCESSING COMPLETE"<<endl;
  content=this->getGeneratedResponse(length,response);
cout<<"CONTENT TO SEND---"<<content<<endl;

  responseContentType=this->getGeneratedResponseContentType(response);
  cookies=this->getGeneratedCookieFromResponse(response);
  if(strcmp(content,"none")==0)
  {
  content=="";
  }
  if(strcmp(responseContentType.c_str(),"none")==0)
  {
  responseContentType="text/html";
  }
  if(strcmp(cookies.c_str(),"none")==0)
  {
  cookies="";
  }
 }
 if(strcmp(type.c_str(),"POST")==0)
 {
  this->processPostRequest(client,request,response);
  content=this->getGeneratedResponse(length,response);
  responseContentType=this->getGeneratedResponseContentType(response);
  cookies=this->getGeneratedCookieFromResponse(response);
  if(strcmp(content,"none")==0)
  {
  content=="";
  }
  if(strcmp(responseContentType.c_str(),"none")==0)
  {
  responseContentType="text/html";
  }
  if(strcmp(cookies.c_str(),"none")==0)
  {
  cookies="";
  }
 }
}
else
{
 content=client.getRequestContent(length);
 if(client.failed())
 {
  cout<<"ERROR ======="<<server.getError()<<endl;
  cout<<"unable to get request content"<<endl;
  freeRequest(request);
  freeResponse(response);
  return;
 }
}


int status=client.getRequestStatus();
cout<<"request status is=="<<status<<endl;


if(strcmp(contentType.c_str(),"serverSide")==0)
{
  client.sendResponse(status,responseContentType,content,cookies,length);
 if(client.failed())
 {
 cout<<"ERROR ======="<<server.getError()<<endl;
  freeRequest(request);
  freeResponse(response);
 return;
 }
}
else
{
 client.sendResponse(status,contentType,content,"",length);
 if(client.failed())
 {
 cout<<"ERROR ======="<<server.getError()<<endl;
  freeRequest(request);
  freeResponse(response);
 return;
 }
}
  freeRequest(request);
  freeResponse(response);
}













void MyServer::processGetRequest(Socket &client,REQUEST *request,RESPONSE *response)
{
cout<<"PROCESS GET REQUEST MAI AAYA"<<endl;
string str=client.getRequest();
string url;
url=str;
//------code to parse the query string and get all attributes
size_t found=str.find("?");
string substring;
string name,value;
if(found!=string::npos)
{

url=str.substr(0,found);

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

setAttribute(request,name.c_str(),value.c_str());

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
setAttribute(request,name.c_str(),value.c_str());
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
setAttribute(request,name.c_str(),value.c_str());
 }
 }
}
}
//code toparse query string ends------------

this->setReceivedCookieInRequest(client,request);

PROCESS* process=this->process;
cout<<"process ka object myserver side "<<process<<endl;

if(process==NULL)
{
client.setRequestStatus(404);
}
else
{
cout<<"get processor ka adderss myServer side"<<process->getProcessor<<endl;
RUN* run=process->getProcessor(url.c_str());
 if(run==NULL)
 {
  client.setRequestStatus(404);
 }
 else
 {
  cout<<"REQUESTED FUNCTION KO CALL KARA "<<url<<endl;
  run->function(request,response);
  if(response->status!=0)
   client.setRequestStatus(response->status);
  else
   client.setRequestStatus(200);
 }
}
}



void MyServer::processPostRequest(Socket &client,REQUEST *request,RESPONSE *response)
{
string str=client.getRequestBuffer();
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

setAttribute(request,name.c_str(),value.c_str());

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

setAttribute(request,name.c_str(),value.c_str());

  cout<<name<<" "<<value<<endl;
 }
 }
}
}
//------parsing of post request ends

/*
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

*/

this->setReceivedCookieInRequest(client,request);

PROCESS* process=this->process;

if(process==NULL)
{
client.setRequestStatus(404);
}
else
{
RUN* run=process->getProcessor(url.c_str());
 if(run==NULL)
 {
  client.setRequestStatus(404);
 }
 else
 {
  run->function(request,response);
  if(response->status!=0)
   client.setRequestStatus(response->status);
  else
   client.setRequestStatus(200);
 }
}

}



char* MyServer::getGeneratedResponse(int &size,RESPONSE *response)
{
if(response->responseText!=NULL)
{
//PrintWriter* pw=response->getWriter();
//string responseText=pw->getResponseString();

int length=strlen(response->responseText);

size=length+1;
char* responseToSend=response->responseText;
return responseToSend;
}
else
{
 char* n=(char*)malloc(sizeof(char*)*5);
 strcpy(n,"none");
 return n;
}
}

string MyServer::getGeneratedResponseContentType(RESPONSE* response)
{
if(response->contentType!=NULL)
{
string ct=response->contentType;
return ct;
}
else
return "none";
}



void MyServer::setReceivedCookieInRequest(Socket &client,REQUEST* request)
{
string str=client.getRequestHeader();

printf("setReceivedCookieInRequest CHALA \n");
//CODE TO PARSE COOKIE FROM THE REQUEST---------------
int i=0;
size_t end;
string cookie;
string key;
string value;
COOKIE* create_cookie;
size_t found=str.find("Cookie: ");
if(found!=string::npos)
{
create_cookie=new COOKIE();

end=str.find("\n",found);
cookie=str.substr(found,end-found-1);
size_t data=8;
size_t equalTo;
equalTo=cookie.find("=");
if(equalTo!=string::npos)
{
 key=cookie.substr(data,equalTo-data);
}
size_t semicolon;
semicolon=cookie.find("; ");
if(semicolon!=string::npos)
{
 value=cookie.substr(equalTo+1,semicolon-equalTo-1);
}
else
{
 value=cookie.substr(equalTo+1);
}
cout<<key<<" "<<value<<endl;
create_cookie->key=key.c_str();
create_cookie->value=value.c_str();
setCookie(request,create_cookie);

if(semicolon!=string::npos)
{
 i=semicolon;
 while(1)
 {
  create_cookie=new COOKIE();
  equalTo=cookie.find("=",i);
  key=cookie.substr(semicolon+2,equalTo-semicolon-2);

  i=equalTo;
  
  semicolon=cookie.find("; ",i);
  if(semicolon!=string::npos)
  {
   value=cookie.substr(equalTo+1,semicolon-equalTo-1);
   i=semicolon;
   cout<<key<<" "<<value<<endl;

   create_cookie->key=key.c_str();
   create_cookie->value=value.c_str();
   setCookie(request,create_cookie);   
  }
  else
  {
   value=cookie.substr(equalTo+1);
   cout<<key<<" "<<value<<endl;

   create_cookie->key=key.c_str();
   create_cookie->value=value.c_str();
   setCookie(request,create_cookie);
   break;
  }
 }
} //if further cookie values exists
} //If cookie found

//CODE TO PARSE COOKIE FROM THE REQUEST----ENDS---------------
}






string MyServer::getGeneratedCookieFromResponse(RESPONSE* response)
{
COOKIE** cookieList=response->cookies;

if(cookieList==NULL) return "none";

int count=response->cookieCount;
string cookieString;
cookieString="\0";
int i;
for(i=0;i<count;i++)
{
cookieString=cookieString+"Set-Cookie: ";
cookieString=cookieString+string(cookieList[i]->key);
cookieString=cookieString+"=";
cookieString=cookieString+string(cookieList[i]->value);
 if(cookieList[i]->expires!=NULL)
 {
  cookieString=cookieString+"; Expires=";
  cookieString=cookieString+string(cookieList[i]->expires);
 }
 if(cookieList[i]->maxAge!=NULL)
 {
  cookieString=cookieString+"; Max-Age=";
  cookieString=cookieString+string(cookieList[i]->maxAge);
 } 
 if(cookieList[i]->domain!=NULL)
 {
  cookieString=cookieString+"; Domain=";
  cookieString=cookieString+string(cookieList[i]->domain);
 }
 if(cookieList[i]->path!=NULL)
 {
  cookieString=cookieString+"; Path=";
  cookieString=cookieString+string(cookieList[i]->path);
 } 
 if(cookieList[i]->sameSite!=NULL)
 {
  cookieString=cookieString+"; SameSite=";
  cookieString=cookieString+string(cookieList[i]->sameSite);
 } 
 if(cookieList[i]->secure!=0)
 {
  cookieString=cookieString+"; Secure";
 }
 cookieString=cookieString+"\n";
}

return cookieString;
}

