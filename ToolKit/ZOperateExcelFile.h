/*
** Version  1.0.0.3
** Date     2019.09.26
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com
*/
#pragma once
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "CApplication.h"
//�����еĴ���lSheetIndex,lRow,lColumn���Ǵ�0��ʼ
class ZOperateExcelFile
{
private:
	CApplication m_excelapplication;//EXCEL�Ľ���ʵ��
	CWorkbooks m_excelbooks;//Excel Book���ϣ�������ļ�ʱ��
	CWorkbook m_excelworkbook;//��ǰʹ�õ�Book����ǰ������ļ�
	CWorksheets m_excelsheets;//EXCEL��sheets����
	CWorksheet m_excelworksheet;//��ǰʹ��sheet
	CRange m_excelrange;//��ǰ�Ĳ�������
	CString m_strFileName;//�򿪵�Excel�ļ���
	int m_nIsInitCount;
	BOOL m_nIsPreloadSucceed;//�Ƿ��Ѿ�Ԥ������ĳ��sheet������
	COleSafeArray m_olesafearray;//Create the SAFEARRAY from the VARIANT ret.
	BOOL PreloadSheet();//Ԥ�ȼ���
private:
	static COleVariant m_covOptional;
public:
	ZOperateExcelFile();
	virtual ~ZOperateExcelFile();
	BOOL InitExcel(CString * p_strError = NULL);//��ʼ��EXCEL
	void ReleaseExcel();//�ͷ�EXCEL
	void ShowInExcel(BOOL nIsShow);
	BOOL IsCellString(long lRow, long lColumn);//���һ��CELL�Ƿ����ַ���
	BOOL IsCellInt(long lRow, long lColumn);//���һ��CELL�Ƿ�����ֵ
	CString GetCellString(long lRow, long lColumn);//�õ�һ��CELL��String
	int GetCellInt(long lRow, long lColumn);//�õ�����
	double GetCellDouble(long lRow, long lColumn);//�õ�double������
	long GetRowCount();//ȡ���е�����
	long GetColumnCount();//ȡ���е�����
	BOOL LoadSheet(long lSheetIndex, BOOL nIsPreload = TRUE);//ʹ��ĳ��shet��shit��shit
	BOOL LoadSheet(const CString & strSheet, BOOL nIsPreload = TRUE);//ͨ������ʹ��ĳ��sheet��
	CString GetSheetName(long lSheetIndex);//ͨ�����ȡ��ĳ��Sheet������
	long GetSheetCount();//�õ�Sheet������
	BOOL OpenExcelFile(const CString & strFileName);//���ļ�
	void CloseExcelFile(BOOL nIsSave = FALSE);//�رմ򿪵�Excel �ļ�����ʱ���EXCEL�ļ���Ҫ
	void SaveasXSLFile(const CString & strFilePath);//���Ϊһ��EXCEL�ļ�
	void Save();//����EXCEL�ļ�
	CString GetFileName();	//ȡ�ô��ļ�������
	CString GetLoadSheetName();//ȡ�ô�sheet������
	void SetCellInt(long lRow, long lColumn, int nData);//д��һ��CELLһ��int
	void SetCellString(long lRow, long lColumn, const CString & strData);//д��һ��CELLһ��string
	void PrintOut(long lCopies = 1, long lFrom = -1, long lTo = -1);//��ӡ��ǰʹ�õ�Book
public:
	static HRESULT InitOle();
	static void UninitOle();
	static char * GetColumnName(long lColumn);//ȡ���е����ƣ�����27->AA	
};
