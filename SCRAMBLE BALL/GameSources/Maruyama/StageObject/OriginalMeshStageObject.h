/*!
@file OriginalMeshStageObejct.h
@brief OriginalMeshStageObejct�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "StageObjectBase.h"
#include "Maruyama/Utility/Object/CollisionObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�I���W�i�����b�V���I�u�W�F�N�g�̃C���^�[�t�F�[�X
	//--------------------------------------------------------------------------------------
	class I_OriginalMeshObject {
	public:
		/// <summary>
		/// �Փ˔���̐ݒ�
		/// </summary>
		/// <param name="isCollision">�����蔻��𐶐����邩�ǂ���</param>
		virtual void SettingCollisionObject(const bool isCollision) = 0;
	};

	//--------------------------------------------------------------------------------------
	///	�I���W�i�����b�V���I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	template<class DrawType>
	class OriginalMeshStageObject : public StageObjectBase, public I_OriginalMeshObject
	{
		Vec3 m_scaleOffset; //�傫���̃I�t�Z�b�g

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		/// <param name="name">�I�u�W�F�N�g��</param>
		/// <param name="texture">�e�N�X�`����</param>
		OriginalMeshStageObject(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& texture)
			: StageObjectBase(stage, name, texture)
		{}

		void OnCreate() override {
			StageObjectBase::OnCreate();

			m_scaleOffset = Vec3(0.25f);
			//���b�V���̒����pMat
			Mat4x4 spanMat;
			spanMat.affineTransformation(
				Vec3(m_scaleOffset),
				Vec3(0.0f),
				Vec3(0.0f, XM_PI, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f)
			);

			auto draw = AddComponent<DrawType>();
			auto name = GetName();
			draw->SetMeshResource(GetName());
			draw->SetMeshToTransformMatrix(spanMat);

			//�e�t��
			auto shadow = AddComponent<Shadowmap>();
			shadow->SetMeshResource(GetName());
			shadow->SetMeshToTransformMatrix(spanMat);

			auto offset = Vec3(1.0f / m_scaleOffset.x, 1.0f / m_scaleOffset.y, 1.0f / m_scaleOffset.z);
			transform->SetScale(Vec3(offset));

			SetAlphaActive(true);
			AddTag(L"T_Obstacle");
		}

	protected:
		/// <summary>
		/// �Փ˔��萶���p�����[�^�̎擾
		/// </summary>
		/// <param name="scale">�傫��</param>
		/// <returns>�Փ˔��萶���p�����[�^</returns>
		CollisionObjectManager_Parametor GetCollisionObjectParametor(const Vec3& scale) {
			auto param = CollisionObjectManager::Parametor();
			param.objName = m_name;
			param.folderName = L"MapCollisionOffsetData/";
			param.fileName = L"MapCollisionOffsetData.csv";
			param.baseScale = scale;

			return param;
		}

	public:
		/// <summary>
		/// �Փ˔�������邩�ǂ����ݒ�
		/// </summary>
		/// <param name="isCollision">�Փ˔�������邩�ǂ���</param>
		void SetIsCollision(const bool isCollision) {
			m_isCollision = isCollision;
		}

		/// <summary>
		/// �Փ˔���I�u�W�F�N�g�̐ݒ�
		/// </summary>
		/// <param name="isCollision"></param>
		void SettingCollisionObject(const bool isCollision) override {
			auto offset = Vec3(1.0f / m_scaleOffset.x, 1.0f / m_scaleOffset.y, 1.0f / m_scaleOffset.z);
			auto scale = Vec3(transform->GetScale() * offset);
			transform->SetScale(scale);

			//�R���W�����𐶐�����Ȃ�B
			if (isCollision) {
				auto param = GetCollisionObjectParametor(scale);
				auto collisionObjectManager = AddComponent<CollisionObjectManager>(param);
				collisionObjectManager->SettingCollisionObject(param);

				//collisionObjectManager->SetCollisionDrawActive(true);
			}
		}
	};

}

//endbasecross