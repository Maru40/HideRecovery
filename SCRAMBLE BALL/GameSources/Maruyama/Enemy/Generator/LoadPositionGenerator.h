
/*!
@file LoadPositionGenerator.h
@brief LoadPositionGenerator�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "GeneratorBase.h"
#include "Maruyama/Utility/Random.h"
#include "Maruyama/Load/StageMapCSV.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �����p�f�[�^�����[�h���邽�߂̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct LoadPositionGenerator_Parametor {
		wstring objectName;  //�I�u�W�F�N�g�̖��O
		wstring folderName;  //�t�H���_�l�[��
		wstring fileName;    //�t�@�C���l�[��

		LoadPositionGenerator_Parametor();
		LoadPositionGenerator_Parametor(const wstring& objectName, const wstring& folderName, const wstring& fileName);
	};

	//--------------------------------------------------------------------------------------
	/// �����ʒu�����[�h���Đ�������W�F�l���[�^
	//--------------------------------------------------------------------------------------
	template<class T>
	class LoadPositionGenerator : public GeneratorBase
	{
	public:
		using Parametor = LoadPositionGenerator_Parametor;

	private:
		Parametor m_param;  //�p�����[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="objectName">��������I�u�W�F�N�g��</param>
		/// <param name="folderName">�ǂݍ��ރt�H���_�[��</param>
		/// <param name="fileName">�ǂݍ��ރt�@�C����</param>
		LoadPositionGenerator(const std::shared_ptr<GameObject>& objPtr, const wstring& objectName, const wstring& folderName, const wstring& fileName) :
			LoadPositionGenerator(objPtr, Parametor(objectName, folderName, fileName))
		{}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="parametor">�����p�p�����[�^</param>
		LoadPositionGenerator(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor) :
			GeneratorBase(objPtr), m_param(parametor)
		{}

		void OnCreate() override {
			//�t�@�C�����琶���������ꏊ���擾
			auto datas = StageMapCSV::GetPositionScaleRotations(m_param.objectName, m_param.folderName, m_param.fileName);
			for (auto& data : datas) {
				auto object = GetStage()->Instantiate<T>(data.position, Quat::Identity());
				auto objTrans = object->GetComponent<Transform>();
				objTrans->Rotate(data.rotation);

				AddObject(object);
			}
		}

	};

}
