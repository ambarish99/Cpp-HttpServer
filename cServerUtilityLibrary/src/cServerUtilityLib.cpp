#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<cServerUtilityLib.h>
using namespace std;


void cServerUtility::setAttribute(REQUEST* request,const char* key,const char* value)
{
request->key[request->index]=key;
request->value[request->index]=value;
cout<<"key"<<request->key[request->index]<<"  value"<<request->value[request->index]<<endl;
request->index++;
cout<<"INDEX OF REQUEST="<<request->index<<endl;
}

const char* cServerUtility::getAttribute(REQUEST* request,const char* key)
{
int i=0;
for(i=0;i<request->index;i++)
{
if(strcmp(key,request->key[i])==0)
{
return request->value[i];
}
}
return NULL;
}


void* cServerUtility::getAllCookies(REQUEST* request)
{
return request->cookies;
}


void cServerUtility::setCookie(REQUEST* request,COOKIE* cookie)
{
if(request->cookies==NULL)
{
request->cookies=(COOKIE**)malloc(sizeof(COOKIE*));
request->cookies[0]=cookie;
request->cookieCount++;
}
else
{
request->cookies=(COOKIE**)realloc(request->cookies,sizeof(COOKIE*)*(request->cookieCount+1));
request->cookies[request->cookieCount]=cookie;
request->cookieCount++;
}

printf("request->cookies[0]->key==%s  request->cookies[0]->value==%s\n",request->cookies[0]->key,request->cookies[0]->value);
}



void cServerUtility::freeRequest(REQUEST *request)
{
printf("To delete REQUEST\n");
int i;
for(i=0;i<request->index;i++)
{
free((char*)request->key[i]);
free((char*)request->value[i]);
delete request;
}
printf("REQUEST DELETED\n");
}



void cServerUtility::put(RESPONSE *response,const char* text)
{
if(response->responseText==NULL)
{
cout<<"RESPONSE TEXT KI VALUE NULL HAI"<<endl;
response->responseText=(char*)malloc(sizeof(char));
response->responseText[0]='\0';
}
cout<<"RESPONSE tEXT IN PUT METHOD"<<response->responseText<<endl;
int initial=strlen(response->responseText)+1;
int final=initial+strlen(text)+1;
response->responseText=(char*)realloc(response->responseText,sizeof(char)*final);
strcat(response->responseText,text);
strcat(response->responseText,"\n");
}


char* cServerUtility::get(RESPONSE* response)
{
return response->responseText;
}

void cServerUtility::setHeader(RESPONSE *response,const char* header)
{
response->header=header;
}

void cServerUtility::setContentType(RESPONSE *response,const char* contentType)
{
response->contentType=contentType;
}

void cServerUtility::setStatus(RESPONSE *response,int status)
{
response->status=status;
}


void cServerUtility::addCookie(RESPONSE* response,COOKIE* cookie)
{
if(response->cookies==NULL)
{
response->cookies=(COOKIE**)malloc(sizeof(COOKIE*));
response->cookies[0]=cookie;
response->cookieCount++;
}
else
{
response->cookies=(COOKIE**)realloc(response->cookies,sizeof(COOKIE*)*(response->cookieCount+1));
response->cookies[response->cookieCount]=cookie;
response->cookieCount++;
}
}

void cServerUtility::freeResponse(RESPONSE* response)
{
printf("TO delete response\n");
free(response->responseText);
free((char*)response->header);
free((char*)response->contentType);
delete response;
printf("RESPONSE FREE\n");
}
