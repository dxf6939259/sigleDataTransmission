// DogClass.h: interface for the CDogClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOGCLASS_H__4542D586_228D_48CB_A0D8_228B3E15DD79__INCLUDED_)
#define AFX_DOGCLASS_H__4542D586_228D_48CB_A0D8_228B3E15DD79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CDogClass : public CObject  
{
private :
	int m_nNum;
	CString m_sData;
public:
	int ShowDlg();
	void Init();
	CDogClass();
	virtual ~CDogClass();
	void SetNum(int num) { m_nNum = num;};
	int  GetNum()        { return m_nNum; };
	void SetData(CString data) { m_sData = data;};
	CString  GetData()        { return  m_sData; };

};

#endif // !defined(AFX_DOGCLASS_H__4542D586_228D_48CB_A0D8_228B3E15DD79__INCLUDED_)
