/*
* Copyright (c) 2013，中国电子科技集团公司第五十四研究所
* All rights reserved.
* 
* 文件名称：UserDefine.h
* 摘    要：用户宏定义,以及自定义结构体
*
* 编写与修改记录：
* <作者>      <日期>        <版本号>      <编写与修改内容> 

   dgh       2013-10-9       Ver 1.0         创建
*/
#pragma once

#pragma warning(disable :4786)

//定义一帧发送的最大数据域长度
#define MAX_FRAME_LEN 1300

//宏定义安全释放内存
#define SafeDelete(p) if(p != NULL){delete p; p = NULL;}
//安全删除数组
#define FreeDeleteArray(a) if(a!=NULL){delete [] a; a=NULL;}
//得到socket错误码
//#define errno WSAGetLastError()
//缓冲池个数(可设置)
#define MAX_FRAME_BUF_COUNT 10000
#define DATA_BUF_SIZE (MAX_FRAME_LEN+20)*MAX_FRAME_BUF_COUNT  //缓冲区大小

//缓冲池个数(可设置)
#define MAX_FRAME_BUF_COUNT_F 180000
#define DATA_BUF_SIZE_F (MAX_FRAME_LEN+20)*MAX_FRAME_BUF_COUNT_F  //缓冲区大小
////////////////////////////////////////////////////////////////////
//定义传输类型
//文件收发
#define TYPE_SEND_FILE            0
#define TYPE_RECEIVE_FILE         1
//短报文收发
#define TYPE_SEND_SHORTMES        2
#define TYPE_RECEIVE_SHORTMES     3
//流媒体收发
#define TYPE_SEND_STREAM          4
#define TYPE_RECEIVE_STREAM       5
////////////////////////////////////////////////

/*
 * 协议中数据类型
 */
//文件传输
#define TRANS_FILE 1
//短报文传输
#define TRANS_SHORTMES 2
//流媒体传输
#define TRANS_STREAM 3
///////////////////////////////////////////////////////

//回调函数,反馈接收的数据给上层
typedef  VOID (CALLBACK *TransCallBack)(int nTransType, int nMesType,int nLen,unsigned char* buff);

//回调函数,反馈错误信息或者文件传输时的进度
//modify lgp 12-18 begin
//typedef void (CALLBACK *FeedBackInfo)(LONG,void*);
typedef void (CALLBACK *FeedBackInfo)(LONG,void*,void*);
//modify lgp 12-18 end

/*
 * 回调函数反馈问题
 */
#define UPDATE_PROGRESS     0 //更新进度条
#define SAFE_CHECK_ERROR    1 //安全码错误
#define BOTHWAY_CHECK_FAIL  2 //双向校验回路失败

//传输错误
#define TRANS_ERROR 3 

//传输完毕
#define TRANS_COMPLETE 4

//收到双向校验码,通知程序开始发数
#define START_SENDDATA  5

//校验码存在,双向校验回路申请失败
#define DOUBLECHECK_RESTART_ERROR 7 

//停止接收
#define STOP_RECV 8

//传输丢包
#define TRANS_LOSS_PACKET 9

//接收文件名
#define RECV_FILE_NAME 10

//数据缓冲区满
#define DATABUF_IS_FULL 11

//帧格式错误,没有此帧格式(帧头错误,帧的传输类型错误)
#define FRAME_ERROR 12

//此标识帧文件末被创建,可能丢失信息头
#define FILE_NOCREATE 13

//无法写入文件,文件句柄错误
#define WRITE_FILE_ERROR 14

//接到的帧数据类型不匹配
#define TRANSTYPE_ERROR 15

//add lgp 12-02 begin
#define RECV_PATH_NOT_EXIST 16
//add lgp 12-02 end

//创建目录结构失败!
#define CREATE_DIR_ERROR 17
/////////////////////////////////////////////////////////////////

/*
 *  传输状态标记
 */
#define TRANS_END 0 //传输完成
#define TRANS_ING 1 //正在传输


/*
 *  文件接收读写结构
 */
 typedef struct _RECVWRITEFILE_ 
 {
	 int ckFlag;      //双向校验标识
	 int curSerNum;   //当前包序号

	 int lostPacketCt; //丢包个数
	
	 LONG lCurTimes; //当前传输进度
	 LONG64 curRecvLen; //当前收取长度
	 LONG64 fileLen;   //文件总大小
	 byte fileName[90];//文件名
	 CString strReFileName; //加时间后的文件名
	 CFile *pOfile;    //文件读写指针

	 _RECVWRITEFILE_()
	 {
		 ckFlag = 0;
		 curSerNum = 1;  //从第一帧开始
		 lostPacketCt = 0;
		 lCurTimes = 0;
		 curRecvLen = 0;
		 fileLen = 0;
		 strReFileName = "";
		 memset(fileName,0x00,90);
		 pOfile = new CFile;
		 if (NULL == pOfile) //文件读写类创建失败
		 {
			 ASSERT(FALSE);
		 }
	 }
	 ~_RECVWRITEFILE_()
	 {
		 SafeDelete(pOfile);
	 }     
 }RecvWriteFile;
 ///////////////////////////////////////////////////////////////////////////