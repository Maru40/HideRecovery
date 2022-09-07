/*!
@file LoadData.cpp
@brief LoadDataのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"
#include "LoadData.h"

namespace basecross {

	void LoadData::LoadTextures(const vector<LoadTxParam>& params)
	{

		auto& app = App::GetApp();

		for (auto& param : params)
		{
			app->RegisterTexture(param.m_keyName ,m_path + param.m_fileName);
		}
	}


	void LoadData::LoadSound(const wstring& folderName, const vector<LoadSoundParam>& params)
	{
		auto& app = App::GetApp();
		auto media = app->GetDataDirWString();

		auto path = media + folderName;

		for (auto& param : params)
		{
			app->RegisterWav(param.m_keyName, path + param.m_fileName);
		}
	}

	void LoadData::LoadCSV(CsvFile& csvFile, const wstring& folderName, const wstring& fileName)
	{
		auto media = GetMedia();
		auto path = media + folderName;

		csvFile.SetFileName(path + fileName);
		csvFile.ReadCsv();
	}

}