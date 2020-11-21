//TO COMPILE : g++ -static startServer.cpp -o startServer.exe -I c:\cServerClient\cServerUtilityLibrary\include\ -lcServerUtilityLib -L c:\cServerClient\cServerUtilityLibrary\src\ -I c:\cServerClient\serverLibrary\include\ -lhttpServerLib -L c:\cServerClient\serverLibrary\src\ -I c:\cServerClient\server\include\ -lmyServer -L c:\cServerClient\server\src\   -std=c++11 -lws2_32


#include<iostream>
#include <cServerUtilityLib.h>
#include<myServer.h>
#include<httpServerLib.h>
using namespace std;
using namespace myServer;
using namespace tmServer;
using namespace cServerUtility;

/*
class HelloService:public GetService
{
public:
void processGetService(Request* request,Response* response)
{
PrintWriter *pw=response->getWriter();

pw->println("<html>");
pw->println("<body>");
pw->println("<h1>I am  Ambarsih Dashora </h1>");
pw->println("Welcome to hello world");
pw->println("</body>");
pw->println("</html>");

}
};


class MakeServices:public ServiceResolver
{
private:
map<string,Service*> servicesMap;
public:
MakeServices()
{
servicesMap.insert(pair<string,Service*>("/abc.com/helloWorld.ss",NULL));
}

Service* getService(string name)
{
 cout<<"GET SERViCE FUNCTION CHALA"<<endl;
 map<string,Service*>::iterator itr;
 Service* s;
 cout<<"ARGUMENTS=="<<name<<endl;
 if(strcmp(name.c_str(),"/abc.com/helloWorld.ss")==0)
 {
  itr=servicesMap.find("/abc.com/helloWorld.ss");
  if(itr!=servicesMap.end() && itr->second==NULL)
  {
   s=new HelloService();
   itr->second=s;
  }
 }

 itr=servicesMap.find(name);
cout<<"SERVICE OBJECT=="<<itr->second<<endl;
 if(itr!=servicesMap.end())
 {
  return itr->second;
 }
 else
 return NULL;
}

};

*/



void show(REQUEST* request,RESPONSE* response)
{
COOKIE* cookie=new COOKIE();
cookie->key="place";
cookie->value="Ujjain";
addCookie(response,cookie);
setContentType(response,"text/html");
cout<<"text=="<<response->responseText<<endl;
cServerUtility::put(response,"<html>");
cout<<"text=="<<response->responseText<<endl;
cServerUtility::put(response,"<body>");
cServerUtility::put(response,"<h1>I am  Ambarsih Dashora </h1>");
cServerUtility::put(response,"Welcome to hello world");
cServerUtility::put(response,"</body>");
cServerUtility::put(response,"</html>");
}


void getData(REQUEST* request,RESPONSE* response)
{
cout<<"GET DATA WALE FUNCTION MAI AAYA"<<endl;
const char* data=getAttribute(request,"college");
setContentType(response,"text/html");
cout<<"text=="<<response->responseText<<endl;
cServerUtility::put(response,"<html>");
cout<<"text=="<<response->responseText<<endl;
cServerUtility::put(response,"<body>");
cServerUtility::put(response,"<h1>I am  Ambarsih Dashora </h1>");
cServerUtility::put(response,"Welcome to hello world");
cServerUtility::put(response,getAttribute(request,"college"));
cServerUtility::put(response,"</body>");
cServerUtility::put(response,"</html>");
}

void getcookie(REQUEST *request,RESPONSE* response)
{
COOKIE** cookie=(COOKIE**)getAllCookies(request);
if(cookie==NULL) printf("cookie nul hai\n");
printf("request->cookieCount==%d\n",request->cookieCount);
const char* key=cookie[0]->key;
const char* value=cookie[0]->value;

printf("key=%s  value=%s \n",key,value);

setContentType(response,"text/html");
cServerUtility::put(response,"<html>");
cServerUtility::put(response,"<body>");
cServerUtility::put(response,"<h1>I am  Ambarsih Dashora </h1>");
cServerUtility::put(response,cookie[0]->key);
cServerUtility::put(response,cookie[0]->value);
cServerUtility::put(response,"Welcome to hello world");
cServerUtility::put(response,"</body>");
cServerUtility::put(response,"</html>");
}

RUN* mapping(const char* url)
{
if(strcmp(url,"/abc.com/helloWorld.ss")==0)
{
RUN * run;
run=(RUN*)malloc(sizeof(RUN));
run->function=show;
return run;
}

if(strcmp(url,"/abc.com/getData.ss")==0)
{
RUN * run;
run=(RUN*)malloc(sizeof(RUN));
run->function=getData;
return run;
}

if(strcmp(url,"/abc.com/getcookie.ss")==0)
{
RUN * run;
run=(RUN*)malloc(sizeof(RUN));
run->function=getcookie;
return run;
}

return NULL;
}


int main()
{
//WebServer ws;
//ServiceResolver* sr=new MakeServices();
//cout<<"SERVICE RESOLVER TO SEND=="<<sr<<endl;

//ws.registerServiceResolver(sr);
//ws.start(7000);

MyServer server;

PROCESS* process;
process=(PROCESS*)malloc(sizeof(PROCESS));
process->getProcessor=mapping;

server.startServer(8000,process);
return 0;
}
