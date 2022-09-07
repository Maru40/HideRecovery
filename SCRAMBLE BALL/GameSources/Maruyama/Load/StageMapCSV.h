/*!
@file StageMapCSV.h
@brief StageMapCSV
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "LoadData.h"

#include "Maruyama/StageObject/StageObjectBase.h"

#include "Maruyama/StageObject/OriginalMeshStageObject.h"
#include "Maruyama/StageObject/OriginalMeshToonObject.h"
#include "Maruyama/Utility/Object/CollisionObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�ʒu�A�傫���A��]
	//--------------------------------------------------------------------------------------
	struct PositionScaleRotation {
		Vec3 position;	//�ʒu
		Vec3 scale;		//�傫��
		Vec3 rotation;	//��]

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		PositionScaleRotation();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="position">�ʒu</param>
		/// <param name="scale">�傫��</param>
		/// <param name="rotation">��]</param>
		PositionScaleRotation(const Vec3& position, const Vec3& scale, const Vec3& rotation);
	};

	//--------------------------------------------------------------------------------------
	///	�X�e�[�W�}�b�vCSV�t�@�C���̓ǂݍ���
	//--------------------------------------------------------------------------------------
	class StageMapCSV : public GameObject
	{
	public:
		static map<wstring, int> sm_admissionCount; //���ꂵ����

	private:

		//--------------------------------------------------------------------------------------
		///	CSV�t�@�C���̗�C���f�b�N�X
		//--------------------------------------------------------------------------------------
		enum CsvIndex
		{
			name,
			posX, posY, posZ,
			scaleX, scaleY, scaleZ,
			rotX, rotY, rotZ,
			texture,
		};

		wstring m_folderName;				//�t�H���_�[��
		wstring m_fileName;					//�t�@�C����
		map<wstring, CsvFile> m_csvFiles;	//CSV�t�@�C����

		map<wstring ,std::vector<ex_weak_ptr<StageObjectBase>>> m_stageObjs;	//���������X�e�[�W�I�u�W�F�N�g�z��

		/// <summary>
		/// �I�u�W�F�N�g�𐶐�����
		/// </summary>
		/// <param name="name">�I�u�W�F�N�g�̖��O</param>
		/// <param name="scale">�傫��</param>
		/// <param name="rotation">��]</param>
		/// <param name="position">�ʒu</param>
		/// <param name="texture">�摜</param>
		/// <returns>���������I�u�W�F�N�g</returns>
		template<class T>
		std::shared_ptr<T> Instantiate(const wstring& name, const Vec3& scale, const Vec3& rotation, const Vec3& position, const wstring& texture) {
			auto object = GetStage()->AddGameObject<T>(name, texture);
			auto objTrans = object->GetComponent<Transform>();
			auto newScale = scale;
			auto newPosition = position;

			objTrans->SetScale(scale);
			objTrans->SetRotation(rotation);
			objTrans->SetPosition(position);

			return object;
		}

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		/// <param name="folderName">�t�H���_�[��</param>
		/// <param name="fileName">�t�@�C����</param>
		StageMapCSV(const std::shared_ptr<Stage>& stage,
			const std::wstring& folderName,
			const std::wstring& fileName
		) :
			GameObject(stage),
			m_folderName(folderName),
			m_fileName(fileName)
		{}

		void OnCreate() override;
		void OnUpdate() override;
 
		/// <summary>
		/// CSV�t�@�C���̃��[�h
		/// </summary>
		void LoadCSV();

		/// <summary>
		/// CSV�t�@�C���̃��[�h
		/// </summary>
		/// <param name="folderName">�t�H���_�[��</param>
		/// <param name="fileName">�t�@�C����</param>
		/// <returns>�ǂݍ���CSV�t�@�C��</returns>
		static map<wstring, CsvFile> LoadCSVFileMap(const wstring& folderName, const wstring& fileName);

		/// <summary>
		/// �}�b�v�̃��Z�b�g
		/// </summary>
		void ResetMap();

		/// <summary>
		/// �}�b�v�̐؂�ւ�
		/// </summary>
		/// <param name="fileName">�؂�ւ������}�b�v�̏��̓�����csv�t�@�C����</param>
		/// <returns>�}�b�v��V�K�쐬����K�v������ꍇ��ture��Ԃ��B</returns>
		bool ChangeMap(const wstring& fileName);

		/// <summary>
		/// �X�e�[�W�̃A�N�e�B�u�̐؂�ւ�
		/// </summary>
		/// <param name="isActive">�A�N�e�B�u���ǂ���</param>
		void StageObjectActiveChange(const bool isActive);

		/// <summary>
		/// �X�e�[�W�I�u�W�F�N�g�̒ǉ�
		/// </summary>
		/// <param name="fileName">�t�@�C����</param>
		/// <param name="object">�ǉ�����I�u�W�F�N�g</param>
		void AddStageObject(const wstring& fileName,const std::shared_ptr<StageObjectBase>& object) {
			ex_weak_ptr<StageObjectBase> obj = object;
			m_stageObjs[fileName].push_back(obj);
		}

		/// <summary>
		/// �w�肵����̃f�[�^��wsring�^�Ŏ擾����
		/// </summary>
		/// <param name="objName">�I�u�W�F�N�g��</param>
		/// <param name="folderName">�t�H���_�[��</param>
		/// <param name="fileName">�t�@�C����</param>
		/// <param name="typeIndex">��C���f�b�N�X</param>
		/// <returns>�w�肵����f�[�^��wstring�^</returns>
		static wstring GetWstringData(const wstring& objName, const wstring& folderName, const wstring& fileName, const int& typeIndex);

		/// <summary>
		/// �w�肵���I�u�W�F�N�g���̈ʒu��z��Ŏ󂯎��
		/// </summary>
		/// <param name="objName">�I�u�W�F�N�g��</param>
		/// <param name="folderName">�t�H���_�[��</param>
		/// <param name="fileName">�t�@�C����</param>
		/// <returns>�w�肵���I�u�W�F�N�g���̈ʒu�z��</returns>
		static vector<Vec3> GetPositions(const wstring& objName, const wstring& folderName, const wstring& fileName);

		/// <summary>
		/// �w�肵���I�u�W�F�N�g���̉�]��z��Ŏ󂯎��
		/// </summary>
		/// <param name="objName">�I�u�W�F�N�g��</param>
		/// <param name="folderName">�t�H���_�[��</param>
		/// <param name="fileName">�t�@�C����</param>
		/// <returns>�w�肵���I�u�W�F�N�g���̉�]�z��</returns>
		static vector<Vec3> GetRotations(const wstring& objName, const wstring& folderName, const wstring& fileName);

		/// <summary>
		/// �w�肵���I�u�W�F�N�g���̑傫����z��Ŏ󂯎��
		/// </summary>
		/// <param name="objName">�I�u�W�F�N�g��</param>
		/// <param name="folderName">�t�H���_�[��</param>
		/// <param name="fileName">�t�@�C����</param>
		/// <returns>�w�肵���I�u�W�F�N�g���̑傫���z��</returns>
		static vector<Vec3> GetScales(const wstring& objName, const wstring& folderName, const wstring& fileName);

		/// <summary>
		/// �w�肵���I�u�W�F�N�g���ň�ԑ������������u�ʒu�A�傫���A��]�v���󂯎��
		/// </summary>
		/// <param name="objName">�I�u�W�F�N�g��</param>
		/// <param name="folderName">�t�H���_�[��</param>
		/// <param name="fileName">�t�@�C����</param>
		/// <returns>�w�肵���I�u�W�F�N�g���ň�ԑ������������u�ʒu�A�傫���A��]�v</returns>
		static PositionScaleRotation GetPositionScaleRotation(const wstring& objName, const wstring& folderName, const wstring& fileName);

		/// <summary>
		/// �w�肵���I�u�W�F�N�g���u�ʒu�A�傫���A��]�v��z��Ŏ󂯎��
		/// </summary>
		/// <param name="objName">�I�u�W�F�N�g��</param>
		/// <param name="folderName">�t�H���_�[��</param>
		/// <param name="fileName">�t�@�C����</param>
		/// <returns>�w�肵���I�u�W�F�N�g���u�ʒu�A�傫���A��]�v�̔z��</returns>
		static vector<PositionScaleRotation> GetPositionScaleRotations(const wstring& objName, const wstring& folderName, const wstring& fileName);

		/// <summary>
		/// �w�肵���I�u�W�F�N�g���̃e�N�X�`������z��Ŏ擾
		/// </summary>
		/// <param name="objName">�I�u�W�F�N�g��</param>
		/// <returns>�e�N�X�`�����̔z��</returns>
		vector<wstring> GetTextures(const wstring& objName);

		/// <summary>
		/// �}�b�v��ɃI�u�W�F�N�g�𐶐�
		/// </summary>
		/// <param name="objName">�I�u�W�F�N�g��</param>
		/// <param name="offset">��������I�t�Z�b�g�ʒu</param>
		/// <param name="isAddObjectList">���̃N���X�������ǂ���</param>
		/// <returns>���������I�u�W�F�N�g�̔z��</returns>
		template<class T>
		vector<std::shared_ptr<T>> CreateObject(const wstring& objName, const Vec3& offset = Vec3(0.0f), const bool isAddObjectList = true)
		{
			vector<wstring> lineVec;
			m_csvFiles[m_fileName].GetSelect(lineVec, 0, objName);
			vector<std::shared_ptr<T>> resultObjects;

			for (auto& line : lineVec)
			{
				vector<wstring> tokens;
				Util::WStrToTokenVector(tokens, line, L',');

				Vec3 scale(     //�X�P�[���̎擾
					static_cast<float>(_wtof(tokens[CsvIndex::scaleX].c_str())),
					static_cast<float>(_wtof(tokens[CsvIndex::scaleY].c_str())),
					static_cast<float>(_wtof(tokens[CsvIndex::scaleZ].c_str()))
				);

				Vec3 rotation(  //���[�e�[�V�����擾
					XMConvertToRadians(static_cast<float>(_wtof(tokens[CsvIndex::rotX].c_str()))),
					XMConvertToRadians(static_cast<float>(_wtof(tokens[CsvIndex::rotY].c_str()))),
					XMConvertToRadians(static_cast<float>(_wtof(tokens[CsvIndex::rotZ].c_str())))
				);

				Vec3 position(  //�|�W�V�����擾
					static_cast<float>(_wtof(tokens[CsvIndex::posX].c_str())),
					static_cast<float>(_wtof(tokens[CsvIndex::posY].c_str())),
					static_cast<float>(_wtof(tokens[CsvIndex::posZ].c_str()))
				);

				wstring texture = tokens[CsvIndex::texture].c_str();  //�e�N�X�`���̎擾
				
				auto stageObject = Instantiate<T>(objName, scale, rotation, position + offset, texture);
				resultObjects.push_back(stageObject);

				if (isAddObjectList) {
					ex_weak_ptr<StageObjectBase> stageObj = stageObject;
					m_stageObjs[m_fileName].push_back(stageObj);  //�I�u�W�F�N�g�������̃��X�g�ɒǉ�	
				}

				if (auto originalMesh = dynamic_pointer_cast<I_OriginalMeshObject>(stageObject)) {
					bool isCollision = texture == L"in";
					originalMesh->SettingCollisionObject(isCollision);
				}
			}

			return resultObjects;
		}

		/// <summary>
		/// �}�b�v��ɃI�u�W�F�N�g�̕�������
		/// </summary>
		/// <param name="names">�I�u�W�F�N�g���̔z��</param>
		/// <param name="offset">��������I�t�Z�b�g�ʒu</param>
		/// <param name="isAddObjectList">���̃N���X�������ǂ���</param>
		/// <returns>���������I�u�W�F�N�g�̔z��</returns>
		template<class T>
		std::vector<std::shared_ptr<T>> CreateObjects(const vector<wstring>& names, const Vec3& offset = Vec3(0.0f), const bool isAddObjectList = true) {
			std::vector<std::shared_ptr<T>> resultObjects;
			for (const auto& name : names) {
				auto objects = CreateObject<T>(name, offset, isAddObjectList);
				for (const auto& object : objects) {
					resultObjects.push_back(object);
				}
			}
			return resultObjects;
		}

		/// <summary>
		/// �ǉ��������̎擾
		/// </summary>
		/// <param name="fileName">�t�@�C����</param>
		/// <returns>�ǉ�������</returns>
		static int GetAdmissionCount(const wstring& fileName) {
			return sm_admissionCount.at(fileName);
		}

	};

}
