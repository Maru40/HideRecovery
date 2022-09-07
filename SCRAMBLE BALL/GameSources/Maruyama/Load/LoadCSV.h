/*!
@file LoadCSV.h
@brief LoadCSV
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	ロード系Utility
	//--------------------------------------------------------------------------------------
	namespace Utility_Load
	{
		/// <summary>
		/// メディアの取得
		/// </summary>
		/// <returns>メディア</returns>
		static wstring GetMedia()
		{
			auto& app = App::GetApp();
			auto media = app->GetDataDirWString();
			return media;
		}

		/// <summary>
		/// CSVファイルのロード
		/// </summary>
		/// <param name="csvFile">ロードするCSVファイル</param>
		/// <param name="folderName">ロードするフォルダー名</param>
		/// <param name="fileName">ロードするファイル名</param>
		static void LoadCSVFile(CsvFile& csvFile, const wstring& folderName, const wstring& fileName) {
			auto media = GetMedia();
			auto path = media + folderName;

			csvFile.SetFileName(path + fileName);
			csvFile.ReadCsv();
		}
	}

	//--------------------------------------------------------------------------------------
	///	CSVのロードクラス
	//--------------------------------------------------------------------------------------
	class LoadCSV
	{
	private:
		static std::map<wstring, std::function<void(const CsvFile&)>> sm_loadFuncMap;

	public:
		/// <summary>
		/// 読み込み処理
		/// </summary>
		/// <param name="folderName">ロードするフォルダー名</param>
		/// <param name="fileName">ロードするファイル名</param>
		/// <param name="keyName">ロードするキー名</param>
		/// <param name="enumType">ロードするタイプ</param>
		template<class T, class EnumType>
		static std::vector<T> GetDatas(const wstring& folderName, const wstring& fileName, const wstring& keyName, const EnumType& enumType)
		{
			CsvFile csvFile = {};
			Utility_Load::LoadCSVFile(csvFile, folderName, fileName);  //CSVに書き込み

			vector<wstring> lineVec;
			csvFile[fileName].GetSelect(lineVec, 0, keyName);
			vector<std::shared_ptr<T>> results;

			for (auto& line : lineVec) {
				vector<wstring> tokens;
				Util::WStrToTokenVector(tokens, line, L',');

				auto value = static_cast<T>(_wtof(tokens[enumType].c_str()));

				results.push_back(value);
			}

			return results;
		}

		/// <summary>
		/// 全てのデータの取得
		/// </summary>
		/// <param name="folderName">ロードするフォルダー名</param>
		/// <param name="fileName">ロードするファイル名</param>
		/// <param name="keyName">ロードするキー名</param>
		/// <param name="enumType">ロードするタイプ</param>
		template<class T, class EnumType>
		static std::vector<T> GetAllDatas(const wstring& folderName, const wstring& fileName, const wstring& keyName, const EnumType& enumType)
		{
			CsvFile csvFile = {};
			Utility_Load::LoadCSVFile(csvFile, folderName, fileName);  //CSVに書き込み

			vector<wstring> lineVec;
			csvFile[fileName].GetSelect(lineVec, 0, keyName);

			sm_loadFuncMap[keyName](csvFile);
		}

	};

}

//endbasecross