#include"Ptzclass.h"
#include<curl/curl.h>
#include<string>
#include<iostream>
#include<unistd.h>
#include <fstream>
#include <iostream>
#include"tinyxml2.h"


static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
    {
    std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
    if( NULL == str || NULL == buffer )
    {
    return -1;
    }
    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);
    return nmemb;
    }

PTZ::PTZ(){

    
    std::string strUrl = "http://192.168.4.64:80/ISAPI/System/deviceInfo";
    std::string strResponseData;
    pCurlHandle = curl_easy_init();
    curl_easy_setopt(pCurlHandle, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(pCurlHandle, CURLOPT_URL, strUrl.c_str());
    // 设置用户名和密码
    curl_easy_setopt(pCurlHandle, CURLOPT_USERPWD, "admin:Leapting2022");
    // 设置认证方式为摘要认证
    curl_easy_setopt(pCurlHandle, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST);
    // 开启认证参数缓存
    curl_easy_setopt(pCurlHandle, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST_IE);
    // 设置回调函数
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEFUNCTION, OnWriteData);
    // 设置回调函数的参数,获取反馈信息
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEDATA, &strResponseData);
    // 接收数据时超时设置，如果5秒内数据未接收完，直接退出
    curl_easy_setopt(pCurlHandle, CURLOPT_TIMEOUT, 5);
    // 设置重定向次数，防止重定向次数太多
    curl_easy_setopt(pCurlHandle, CURLOPT_MAXREDIRS, 1);
    // 连接超时，这个数值如果设置太短可能导致数据请求不到就断开了
    curl_easy_setopt(pCurlHandle, CURLOPT_CONNECTTIMEOUT, 5);
    CURLcode nRet = curl_easy_perform(pCurlHandle);
    if (0 == nRet)
    {
      // 输出接收的消息
      std::cout << "登陆成功" << std::endl;
    }


}

PTZ::~PTZ(){
    if(pCurlHandle)
        curl_easy_cleanup(pCurlHandle);
}
//适合手动控制
void PTZ::Ptz_Control(int pan,int tilt,int zoom){
    std::string strUrl = "http://192.168.4.64:80/ISAPI/PTZCtrl/channels/1/continuous";
    curl_easy_setopt(pCurlHandle, CURLOPT_CUSTOMREQUEST, "PUT");
    //pan正值表示顺时针旋转，负值表示逆时针旋转，tile 上仰，zoom 调焦距。只能使用整数
    std::string xml_data = 
        "<PTZData>\n" 
        "   <pan>" + std::to_string(pan) + "</pan>\n"
        "   <tilt>" + std::to_string(tilt) + "</tilt>\n"
        "   <zoom>" + std::to_string(zoom) + "</zoom>\n"
        "</PTZData>";
    curl_easy_setopt(pCurlHandle, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(pCurlHandle, CURLOPT_POSTFIELDSIZE, xml_data.size());
    curl_easy_setopt(pCurlHandle, CURLOPT_POSTFIELDS, xml_data.c_str());
    //std::string response;
    //curl_easy_setopt(pCurlHandle, CURLOPT_WRITEFUNCTION, OnWriteData);
    //curl_easy_setopt(pCurlHandle, CURLOPT_WRITEDATA, &response);
    CURLcode res=curl_easy_perform(pCurlHandle);
    if (res != CURLE_OK) {
      std::cerr << "Failed to send request(control): " << curl_easy_strerror(res) << "\n";
    } 
    
}


void PTZ::Ptz_StopControl(){
    std::string strUrl = "http://192.168.4.64:80/ISAPI/PTZCtrl/channels/1/continuous";
   
    std::string xml_data = "<PTZData>\n"
                        "   <pan>0</pan>\n"
                        "   <tilt>0</tilt>\n"
                        "   <zoom>0</zoom>\n"
                        "</PTZData>";
    curl_easy_setopt(pCurlHandle, CURLOPT_CUSTOMREQUEST, "PUT");
    
    curl_easy_setopt(pCurlHandle, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(pCurlHandle, CURLOPT_POSTFIELDSIZE, xml_data.length());
    curl_easy_setopt(pCurlHandle, CURLOPT_POSTFIELDS, xml_data.c_str());
    //curl_easy_setopt(pCurlHandle, CURLOPT_WRITEFUNCTION, OnWriteData);
    CURLcode res=curl_easy_perform(pCurlHandle);
    if (res != CURLE_OK) {
      std::cerr << "Failed to send request(stop): " << curl_easy_strerror(res) << "\n";
    } 
}
//绝对位置控制
//elevation,摄像头俯仰，rangge[-90,270],正值表示仰
//azimuth,绝对旋转角度,range[0,360],顺时针
//absoluteZoom绝对缩放,range[0,1000]

void PTZ::Ptz_absoluteControl(int elevation,int azimuth,int absoluteZoom){
	std::string strUrl = "http://192.168.4.64:80/ISAPI/PTZCtrl/channels/1/absolute";
   
    std::string xml_data = "<PTZData>\n"
						"<AbsoluteHigh>\n"
                        "<elevation>"+std::to_string(elevation*10)+"</elevation>\n"
                        "<azimuth>"+std::to_string(azimuth*10)+"</azimuth>\n"
                        "<absoluteZoom>"+std::to_string(absoluteZoom)+"</absoluteZoom>\n"
						"</AbsoluteHigh>\n"
                        "</PTZData>";
    curl_easy_setopt(pCurlHandle, CURLOPT_CUSTOMREQUEST, "PUT");
    
    curl_easy_setopt(pCurlHandle, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(pCurlHandle, CURLOPT_POSTFIELDSIZE, xml_data.length());
    curl_easy_setopt(pCurlHandle, CURLOPT_POSTFIELDS, xml_data.c_str());
	CURLcode res=curl_easy_perform(pCurlHandle);
    if (res != CURLE_OK) {
    	std::cerr << "Failed to send request(stop): " << curl_easy_strerror(res) << "\n";
    } 

}
//相对位置控制
//addElevation,摄像头俯仰，rangge[-90,270],正值表示仰
//addAzimuth,旋转角度,range[0,360],顺时针
//addZoom 缩放,range[0,1000]
void PTZ::Ptz_relativeControl(int addElevation,int addAzimuth,int addZoom){
	curl_easy_setopt(pCurlHandle, CURLOPT_CUSTOMREQUEST, "GET");
	//std::string strUrl ="http://192.168.4.64:80/ISAPI/PTZCtrl/channels/1/status";
    char str[128]="http://192.168.4.64:80/ISAPI/PTZCtrl/channels/1/status";
    curl_easy_setopt(pCurlHandle, CURLOPT_URL, str);
	std::string response;
    //curl_easy_setopt(pCurlHandle, CURLOPT_WRITEFUNCTION, OnWriteData);
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEDATA, &response);
	CURLcode res=curl_easy_perform(pCurlHandle);
    if (res != CURLE_OK) {
    	std::cerr << "Failed to send request(stop): " << curl_easy_strerror(res) << "\n";
    } 
	tinyxml2::XMLDocument doc;
    doc.Parse(response.c_str());

    tinyxml2::XMLElement* root = doc.FirstChildElement("PTZStatus");
    tinyxml2::XMLElement* absolute = root->FirstChildElement("AbsoluteHigh");

    int elevation = absolute->FirstChildElement("elevation")->IntText();
    int azimuth = absolute->FirstChildElement("azimuth")->IntText();
    int absoluteZoom = absolute->FirstChildElement("absoluteZoom")->IntText();

    std::cout << "Elevation: " << elevation << std::endl;
    std::cout << "Azimuth: " << azimuth << std::endl;
    std::cout << "Zoom: " << absoluteZoom << std::endl;
	elevation=elevation/10+addElevation;
	azimuth=azimuth/10+addAzimuth;
	absoluteZoom=absoluteZoom+addZoom;
    if(azimuth>360){
        azimuth=azimuth-360;
    }
    else if (azimuth<0)
    {
        azimuth=azimuth+360;
    }
    
    if(elevation<-90){
        std::cout<<"俯角过大"<<std::endl;
        elevation=-90;
    }
    else if (elevation>270)
    {
        std::cout<<"仰角过大"<<std::endl;
        elevation=270;
    }   
	this->Ptz_absoluteControl(elevation,azimuth,absoluteZoom);
}


//复位
void PTZ::resetPosition(){
    this->Ptz_absoluteControl(0,0,0);
}







