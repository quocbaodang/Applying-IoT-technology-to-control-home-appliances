#ifndef __MAIN_RUN_H__
#define __MAIN_RUN_H__

#include <DNSServer.h>

#include "Define.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

void initDevice();
void runLoopDevice();
void initPin();
void startConnect();
void checkConnect();
void runCallBack1(void* parameter);
void runCallBack2(void* parameter);
void runCallBack3(void* parameter);
void checkTime(void* parameter);
void checkInternet(void* parameter);

#endif /** __MAIN_RUN_H__ **/
