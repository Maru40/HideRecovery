/*!
@file LoadCSV.h
@brief LoadCSV
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	���[�h�nUtility
	//--------------------------------------------------------------------------------------
	namespace Utility_Load
	{
		/// <summary>
		/// ���f�B�A�̎擾
		/// </summary>
		/// <returns>���f�B�A</returns>
		static wstring GetMedia()
		{
			auto& app = App::GetApp();
			auto media = app->GetDataDirWString();
			return media;
		}

		/// <summary>
		/// CSV�t�@�C���̃��[�h
		/// </summary>
		/// <param name="csvFile">���[�h����CSV�t�@�C��</param>
		/// <param name="folderName">���[�h����t�H���_�[��</param>
		/// <param name="fileName">���[�h����t�@�C����</param>
		static void LoadCSVFile(CsvFile& csvFile, const wstring& folderName, const wstring& fileName) {
			auto media = GetMedia();
			auto path = media + folderName;

			csvFile.SetFileName(path + fileName);
			csvFile.ReadCsv();
		}
	}

	//--------------------------------------------------------------------------------------
	///	CSV�̃��[�h�N���X
	//--------------------------------------------------------------------------------------
	class LoadCSV
	{
	private:
		static std::map<wstring, std::function<void(const CsvFile&)>> sm_loadFuncMap;

	public:
		/// <summary>
		/// �ǂݍ��ݏ���
		/// </summary>
		/// <param name="folderName">���[�h����t�H���_�[��</param>
		/// <param name="fileName">���[�h����t�@�C����</param>
		/// <param name="keyName">���[�h����L�[��</param>
		/// <param name="enumType">���[�h����^�C�v</param>
		template<class T, class EnumType>
		static std::vector<T> GetDatas(const wstring& folderName, const wstring& fileName, const wstring& keyName, const EnumType& enumType)
		{
			CsvFile csvFile = {};
			Utility_Load::LoadCSVFile(csvFile, folderName, fileName);  //CSV�ɏ�������

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
		/// �S�Ẵf�[�^�̎擾
		/// </summary>
		/// <param name="folderName">���[�h����t�H���_�[��</param>
		/// <param name="fileName">���[�h����t�@�C����</param>
		/// <param name="keyName">���[�h����L�[��</param>
		/// <param name="enumType">���[�h����^�C�v</param>
		template<class T, class EnumType>
		static std::vector<T> GetAllDatas(const wstring& folderName, const wstring& fileName, const wstring& keyName, const EnumType& enumType)
		{
			CsvFile csvFile = {};
			Utility_Load::LoadCSVFile(csvFile, folderName, fileName);  //CSV�ɏ�������

			vector<wstring> lineVec;
			csvFile[fileName].GetSelect(lineVec, 0, keyName);

			sm_loadFuncMap[keyName](csvFile);
		}

	};

}

//endbasecross