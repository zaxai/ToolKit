/*
** Version  1.0.0.7
** Date     2019.09.26
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com All Rights Reserved
*/
#pragma once
#include "ZOperateExcelFile.h"
#include <vector>
#include <thread>
#include <future>

class ZExcelImport
{
private:
	std::thread m_thread;
	bool m_bIsRun;
	void ImportThread(std::promise<void> prom);
	ZExcelImport(const ZExcelImport & zxlsimport) = delete;
	ZExcelImport & operator = (const ZExcelImport & zxlsimport) = delete;
protected://���������ж�IsRun()==false����������protected��Ա����
	CString m_strSrcPath;
	int m_nRowOffset;
	int m_nColumnOffset;
	std::vector<std::vector<CString>> m_vec2_strDataImport;
public:
	ZExcelImport();
	ZExcelImport(ZExcelImport && zxlsimport) noexcept;
	ZExcelImport & operator=(ZExcelImport && zxlsimport) noexcept;
	virtual ~ZExcelImport();
	bool IsRun();
	bool StartImport();
	void StopImport();
	virtual void OnImportProgressUpdate(int nProgress) = 0;
	virtual void OnImportResult(bool bResult) = 0;
};

