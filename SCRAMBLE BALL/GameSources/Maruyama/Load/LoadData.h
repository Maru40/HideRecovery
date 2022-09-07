
/*!
@file LoadData.h
@brief ���[�h�f�[�^�w�b�_�@
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�e�N�X�`���̃��[�h�p�̍\����
	//--------------------------------------------------------------------------------------
	struct LoadTxParam 
	{
		wstring m_keyName;	//�L�[��
		wstring m_fileName; //�t�@�C����

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="keyName">�L�[��</param>
		/// <param name="fileName">�t�@�C����</param>
		LoadTxParam(const wstring& keyName, const wstring& fileName):
			m_keyName(keyName),
			m_fileName(fileName)
		{ }
	};

	//--------------------------------------------------------------------------------------
	///	�T�E���h�̃��[�h�p�̍\����
	//--------------------------------------------------------------------------------------
	struct LoadSoundParam
	{
		wstring m_keyName;	//�L�[��
		wstring m_fileName;	//�t�@�C����

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="keyName">�L�[��</param>
		/// <param name="fileName">�t�@�C����</param>
		LoadSoundParam(const wstring& keyName, const wstring& fileName) :
			m_keyName(keyName),
			m_fileName(fileName)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	�f�[�^���[�h�N���X
	//--------------------------------------------------------------------------------------
	class LoadData
	{
		wstring m_media;	//���f�B�A�t�@�C��
		wstring m_path;		//�p�X

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		LoadData() :
			m_path(L"")
		{
			auto& app = App::GetApp();
			m_media = app->GetDataDirWString();
		}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="folderName">�t�H���_�[��</param>
		LoadData(const wstring& folderName) 
		{
			auto& app = App::GetApp();
			m_media = app->GetDataDirWString();
			m_path = m_media + folderName;
		}

		/// <summary>
		/// �p�X�̐؂�ւ�
		/// </summary>
		/// <param name="folderName">�p�X�t�H���_�[��</param>
		void SwitchPath(const wstring& folderName)
		{
			m_path = m_media + folderName;
		}

		/// <summary>
		/// csv�t�@�C���̃��[�h
		/// </summary>
		/// <param name="csvFile">csv�t�@�C����</param>
		/// <param name="fileName">�t�@�C����</param>
		void LoadCSV(CsvFile& csvFile, const wstring& fileName)
		{
			csvFile.SetFileName(m_path + fileName);
		}

		/// <summary>
		/// csv�t�@�C���̃��[�h
		/// </summary>
		/// <param name="csvFile">csv�t�@�C����</param>
		/// <param name="folderName">�t�H���_�[��</param>
		/// <param name="fileName">�t�@�C����</param>
		static void LoadCSV(CsvFile& csvFile, const wstring& folderName, const wstring& fileName);

		/// <summary>
		/// �e�N�X�`���[�̃��[�h�֐�
		/// </summary>
		/// <param name="params">���[�h�p�����[�^�̔z��</param>
		void LoadTextures(const vector<LoadTxParam>& params);

		/// <summary>
		/// BGM,SE�̃��[�h
		/// </summary>
		/// <param name="folderName">�t�H���_�[��</param>
		/// <param name="params">���[�h�p�����[�^�z��</param>
		static void LoadSound(const wstring& folderName, const vector<LoadSoundParam>& params);

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

	};

}

//endbasecross