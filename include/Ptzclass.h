#ifndef PTZ_H
#define PTZ_H

#include<curl/curl.h>

class PTZ
{
private:
    CURL *pCurlHandle=NULL;

public:
    PTZ();
    ~PTZ();
    //云台控制
    void Ptz_Control(int pan,int tilt,int zoom);
    void Ptz_StopControl();
    void Ptz_absoluteControl(int elevation,int azimuth,int absoluteZoom);
    void Ptz_relativeControl(int addElevation,int addAzimuth,int addZoom);
    
    //视频录制
    void Ptz_video();
    //获取设备状态
    void GetStatus();
    //一键聚焦
    void autoFocus();
    //复位
    void resetPosition();
    //抓图
    void capturePicture();

};

#endif