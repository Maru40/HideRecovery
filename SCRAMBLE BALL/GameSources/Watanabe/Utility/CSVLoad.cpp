/*!
@file   CSVLoad.cpp
@brief  CSVのデータを管理するクラス実体
 */

#include "stdafx.h"
#include "CSVLoad.h"
#include "DataExtracter.h"

namespace basecross {
	// インスタンスの初期化
	unique_ptr<CSVLoad, CSVLoad::Deleter> CSVLoad::m_instance = nullptr;

	void CSVLoad::RegisterFile(const wstring& key, const wstring& filePath) {
		CsvFile csvFile;
		csvFile.SetFileName(filePath);
		// 失敗したら
		if (!csvFile.ReadCsv()) {
			throw BaseException(
				L"ファイルの読み込みに失敗しました。",
				L"if (!csvFile.ReadCsv())",
				L"void CSVLoad::RegisterFile()"
			);
		}
		m_data[key] = csvFile.GetCsvVec();
	}

	vector<wstring> CSVLoad::GetData(const wstring& key)const {
		return m_data.at(key);
	}

	bool CSVLoad::CheckData(const wstring& key) {
		return m_data.count(key) != 0;
	}

	int CSVHelper::SearchIndexDataTarget(const vector<wstring>& data, const wstring& searchData, UINT searchCol) {
		int index = -1;
		for (int i = 0; i < data.size(); i++) {
			auto tokens = DataExtracter::DelimitData(data[i]);
			if (tokens[searchCol] == searchData) {
				index = i;
				break;
			}
		}
		return index;
	}
}
//end basecross