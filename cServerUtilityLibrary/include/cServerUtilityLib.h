#ifndef __CSERVER_UTILITY_LIB__$__
#define __CSERVER_UTILITY_LIB__$__
#include<stdio.h>
#include<stdlib.h>
namespace cServerUtility
{
typedef struct cookie
{
const char* key=NULL;
const char* value=NULL;
const char* expires=NULL;
const char* maxAge=NULL;
const char* domain=NULL;
const char* path=NULL;
bool httpOnly=0;
bool secure=0;
const char* sameSite=NULL;
}COOKIE;

typedef struct request
{
const char* key[10000];
const char* value[10000];
int index=0;
COOKIE** cookies=NULL;
int cookieCount=0;
}REQUEST;

void setAttribute(REQUEST* request,const char* key,const char* value);
const char* getAttribute(REQUEST* request,const char* key);
void* getAllCookies(REQUEST* request);
void setCookie(REQUEST* request,COOKIE* cookie);
void freeRequest(REQUEST *request);

typedef struct response
{
char* responseText=NULL;
const char* header=NULL;
const char* contentType=NULL;
int status=0;
COOKIE** cookies=NULL;
int cookieCount=0;
}RESPONSE;

void put(RESPONSE *response,const char* text);
char* get(RESPONSE *response);
void setHeader(RESPONSE *response,const char* header);
void setContentType(RESPONSE *response,const char* header);
void setStatus(RESPONSE *response,int status);
void addCookie(RESPONSE* response,COOKIE* cookie);
void freeResponse(RESPONSE* response);

}

#endif