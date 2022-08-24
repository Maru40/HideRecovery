#include "stdafx.h"
#include "UIObjectCSVBuilder.h"

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
			//CSVファイル
			CsvFile GameStageCsv(fileName);
			GameStageCsv.ReadCsv();
			//CSVの全体の配列
			//CSVからすべての行を抜き出す
			auto& LineVec = GameStageCsv.GetCsvVec();
			for (auto& v : LineVec) {
				//トークン（カラム）の配列
				vector<wstring> Tokens;
				Util::WStrToTokenVector(Tokens, v, L',');
				CreateFromCSV(Tokens[0], stage, v);
			}
		}
		catch (...) {
			throw;
		}
	}
}