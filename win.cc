
#ifdef WIN32

#include "headers.h"
#include <windows.h>

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
  v8::Local<v8::Value> display = DisplayImpl::NewInstance(
    lprcMonitor->top, lprcMonitor->left, 
    lprcMonitor->right - lprcMonitor->left, 
    lprcMonitor->bottom - lprcMonitor->top
  );
  
  v8::Local<v8::Array> displays =  *((v8::Local<v8::Array> *)dwData);
  Nan::Set(displays, displays->Length(), display);
  
  return TRUE;
}

void DumpDevice(const DISPLAY_DEVICE& dd, size_t nSpaceCount )
{
    printf("%*sDevice Name: %s\n", nSpaceCount, "", dd.DeviceName );
    printf("%*sDevice String: %s\n", nSpaceCount, "", dd.DeviceString );
    printf("%*sState Flags: %x\n", nSpaceCount, "", dd.StateFlags );
    printf("%*sDeviceID: %s\n", nSpaceCount, "", dd.DeviceID );
    printf("%*sDeviceKey: ...%s\n\n", nSpaceCount, "", dd.DeviceKey+42 );
}

NAN_METHOD(GetDisplays) {
    v8::Local<v8::Array> displays = Nan::New<v8::Array>();

//    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&displays);
   DISPLAY_DEVICE dd;

    dd.cb = sizeof(DISPLAY_DEVICE);
    DWORD deviceNum = 0;
     while( EnumDisplayDevices(NULL, deviceNum, &dd, 0) ){
      
        v8::Local<v8::Value> display = DisplayImpl::NewInstance(
    0,0,0,deviceNum
  );
        DISPLAY_DEVICE newdd = { 0 };
        newdd.cb = sizeof(DISPLAY_DEVICE);
        DWORD monitorNum = 0;


  while (EnumDisplayDevices(dd.DeviceName, monitorNum, &newdd, 0))
        {
           // DumpDevice(newdd, 4);
            monitorNum++;
             Nan::Set(displays, displays->Length(), display);
        }
       
        deviceNum++;
    }

//    return 0;

//    info.GetReturnValue().Set(deviceNum);

    info.GetReturnValue().Set(displays);
}

#endif