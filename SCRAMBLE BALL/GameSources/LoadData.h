
/*!
@file LoadData.h
@brief ロードデータヘッダ　
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	テクスチャのロード用の構造体
	//--------------------------------------------------------------------------------------
	struct LoadTxParam 
	{
		wstring m_keyName;	//キー名
		wstring m_fileName; //ファイル名

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="keyName">キー名</param>
		/// <param name="fileName">ファイル名</param>
		LoadTxParam(const wstring& keyName, const wstring& fileName):
			m_keyName(keyName),
			m_fileName(fileName)
		{ }
	};

	//--------------------------------------------------------------------------------------
	///	サウンドのロード用の構造体
	//--------------------------------------------------------------------------------------
	struct LoadSoundParam
	{
		wstring m_keyName;	//キー名
		wstring m_fileName;	//ファイル名

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="keyName">キー名</param>
		/// <param name="fileName">ファイル名</param>
		LoadSoundParam(const wstring& keyName, const wstring& fileName) :
			m_keyName(keyName),
			m_fileName(fileName)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	データロードクラス
	//--------------------------------------------------------------------------------------
	class LoadData
	{
		wstring m_media;	//メディアファイル
		wstring m_path;		//パス

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		LoadData() :
			m_path(L"")
		{
			auto& app = App::GetApp();
			m_media = app->GetDataDirWString();
		}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="folderName">フォルダー名</param>
		LoadData(const wstring& folderName) 
		{
			auto& app = App::GetApp();
			m_media = app->GetDataDirWString();
			m_path = m_media + folderName;
		}

		/// <summary>
		/// パスの切り替え
		/// </summary>
		/// <param name="folderName">パスフォルダー名</param>
		void SwitchPath(const wstring& folderName)
		{
			m_path = m_media + folderName;
		}

		/// <summary>
		/// csvファイルのロード
		/// </summary>
		/// <param name="csvFile">csvファイル名</param>
		/// <param name="fileName">ファイル名</param>
		void LoadCSV(CsvFile& csvFile, const wstring& fileName)
		{
			csvFile.SetFileName(m_path + fileName);
		}

		/// <summary>
		/// csvファイルのロード
		/// </summary>
		/// <param name="csvFile">csvファイル名</param>
		/// <param name="folderName">フォルダー名</param>
		/// <param name="fileName">ファイル名</param>
		static void LoadCSV(CsvFile& csvFile, const wstring& folderName, const wstring& fileName);

		/// <summary>
		/// テクスチャーのロード関数
		/// </summary>
		/// <param name="params">ロードパラメータの配列</param>
		void LoadTextures(const vector<LoadTxParam>& params);

		/// <summary>
		/// BGM,SEのロード
		/// </summary>
		/// <param name="folderName">フォルダー名</param>
		/// <param name="params">ロードパラメータ配列</param>
		static void LoadSound(const wstring& folderName, const vector<LoadSoundParam>& params);

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

	};

}

//endbasecross