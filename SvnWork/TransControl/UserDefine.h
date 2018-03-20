/*
* Copyright (c) 2013���й����ӿƼ����Ź�˾����ʮ���о���
* All rights reserved.
* 
* �ļ����ƣ�UserDefine.h
* ժ    Ҫ���û��궨��,�Լ��Զ���ṹ��
*
* ��д���޸ļ�¼��
* <����>      <����>        <�汾��>      <��д���޸�����> 

   dgh       2013-10-9       Ver 1.0         ����
*/
#pragma once

#pragma warning(disable :4786)

//����һ֡���͵���������򳤶�
#define MAX_FRAME_LEN 1300

//�궨�尲ȫ�ͷ��ڴ�
#define SafeDelete(p) if(p != NULL){delete p; p = NULL;}
//��ȫɾ������
#define FreeDeleteArray(a) if(a!=NULL){delete [] a; a=NULL;}
//�õ�socket������
//#define errno WSAGetLastError()
//����ظ���(������)
#define MAX_FRAME_BUF_COUNT 10000
#define DATA_BUF_SIZE (MAX_FRAME_LEN+20)*MAX_FRAME_BUF_COUNT  //��������С

//����ظ���(������)
#define MAX_FRAME_BUF_COUNT_F 180000
#define DATA_BUF_SIZE_F (MAX_FRAME_LEN+20)*MAX_FRAME_BUF_COUNT_F  //��������С
////////////////////////////////////////////////////////////////////
//���崫������
//�ļ��շ�
#define TYPE_SEND_FILE            0
#define TYPE_RECEIVE_FILE         1
//�̱����շ�
#define TYPE_SEND_SHORTMES        2
#define TYPE_RECEIVE_SHORTMES     3
//��ý���շ�
#define TYPE_SEND_STREAM          4
#define TYPE_RECEIVE_STREAM       5
////////////////////////////////////////////////

/*
 * Э������������
 */
//�ļ�����
#define TRANS_FILE 1
//�̱��Ĵ���
#define TRANS_SHORTMES 2
//��ý�崫��
#define TRANS_STREAM 3
///////////////////////////////////////////////////////

//�ص�����,�������յ����ݸ��ϲ�
typedef  VOID (CALLBACK *TransCallBack)(int nTransType, int nMesType,int nLen,unsigned char* buff);

//�ص�����,����������Ϣ�����ļ�����ʱ�Ľ���
//modify lgp 12-18 begin
//typedef void (CALLBACK *FeedBackInfo)(LONG,void*);
typedef void (CALLBACK *FeedBackInfo)(LONG,void*,void*);
//modify lgp 12-18 end

/*
 * �ص�������������
 */
#define UPDATE_PROGRESS     0 //���½�����
#define SAFE_CHECK_ERROR    1 //��ȫ�����
#define BOTHWAY_CHECK_FAIL  2 //˫��У���·ʧ��

//�������
#define TRANS_ERROR 3 

//�������
#define TRANS_COMPLETE 4

//�յ�˫��У����,֪ͨ����ʼ����
#define START_SENDDATA  5

//У�������,˫��У���·����ʧ��
#define DOUBLECHECK_RESTART_ERROR 7 

//ֹͣ����
#define STOP_RECV 8

//���䶪��
#define TRANS_LOSS_PACKET 9

//�����ļ���
#define RECV_FILE_NAME 10

//���ݻ�������
#define DATABUF_IS_FULL 11

//֡��ʽ����,û�д�֡��ʽ(֡ͷ����,֡�Ĵ������ʹ���)
#define FRAME_ERROR 12

//�˱�ʶ֡�ļ�ĩ������,���ܶ�ʧ��Ϣͷ
#define FILE_NOCREATE 13

//�޷�д���ļ�,�ļ��������
#define WRITE_FILE_ERROR 14

//�ӵ���֡�������Ͳ�ƥ��
#define TRANSTYPE_ERROR 15

//add lgp 12-02 begin
#define RECV_PATH_NOT_EXIST 16
//add lgp 12-02 end

//����Ŀ¼�ṹʧ��!
#define CREATE_DIR_ERROR 17
/////////////////////////////////////////////////////////////////

/*
 *  ����״̬���
 */
#define TRANS_END 0 //�������
#define TRANS_ING 1 //���ڴ���


/*
 *  �ļ����ն�д�ṹ
 */
 typedef struct _RECVWRITEFILE_ 
 {
	 int ckFlag;      //˫��У���ʶ
	 int curSerNum;   //��ǰ�����

	 int lostPacketCt; //��������
	
	 LONG lCurTimes; //��ǰ�������
	 LONG64 curRecvLen; //��ǰ��ȡ����
	 LONG64 fileLen;   //�ļ��ܴ�С
	 byte fileName[90];//�ļ���
	 CString strReFileName; //��ʱ�����ļ���
	 CFile *pOfile;    //�ļ���дָ��

	 _RECVWRITEFILE_()
	 {
		 ckFlag = 0;
		 curSerNum = 1;  //�ӵ�һ֡��ʼ
		 lostPacketCt = 0;
		 lCurTimes = 0;
		 curRecvLen = 0;
		 fileLen = 0;
		 strReFileName = "";
		 memset(fileName,0x00,90);
		 pOfile = new CFile;
		 if (NULL == pOfile) //�ļ���д�ഴ��ʧ��
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