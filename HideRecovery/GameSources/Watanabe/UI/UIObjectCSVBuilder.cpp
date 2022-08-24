﻿#include "stdafx.h"
#include "UIObjectCSVBuilder.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	UIObjectCSVBuilder::UIObjectCSVBuilder() {}
	UIObjectCSVBuilder::~UIObjectCSVBuilder() {}

	shared_ptr<UIObjectBase> UIObjectCSVBuilder::CreateFromCSV(const wstring& name, const shared_ptr<Stage>& stage, const wstring& line) {
		auto it = m_creatorMap.find(name);
		if (it == m_creatorMap.end()) {
			return nullptr;
		}
		else {
			auto ptr = (*it).second->Create(stage, line);
			return ptr;
		}
	}

	void UIObjectCSVBuilder::Build(const shared_ptr<Stage>& stage, const wstring& fileName) {
		try {
			// CSVファイル
			CsvFile gameStageCsv(fileName);
			gameStageCsv.ReadCsv();
			// CSVからすべての行を抜き出す
			auto& lineVec = gameStageCsv.GetCsvVec();
			for (auto& v : lineVec) {
				//トークン（カラム）の配列
				vector<wstring> tokens = DataExtracter::DelimitData(v);
				auto uiObject = CreateFromCSV(tokens[0], stage, v);
				m_uiObjectMap[tokens[1]] = uiObject;
			}
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<UIObjectBase> UIObjectCSVBuilder::GetUIObject(const wstring& uiName) {
		if (m_uiObjectMap.count(uiName) > 0) {
			return m_uiObjectMap[uiName];
		}
		return nullptr;
	}
}