/*!
@file CollisionObject.h
@brief CollisionObject
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	struct PositionScaleRotation;

	//--------------------------------------------------------------------------------------
	///	�Փ˃I�u�W�F�N�g�Ǘ��p�����[�^
	//--------------------------------------------------------------------------------------
	struct CollisionObjectManager_Parametor
	{
		wstring objName;    //�I�u�W�F�N�g�l�[��
		wstring folderName; //�t�H���_�[�l�[��
		wstring fileName;   //�t�@�C���l�[��
		Vec3 baseScale;     //��{�X�P�[��
	};

	//--------------------------------------------------------------------------------------
	///	�Փ˃I�u�W�F�N�g�Ǘ�
	//--------------------------------------------------------------------------------------
	class CollisionObjectManager : public Component
	{
	public:
		using Parametor = CollisionObjectManager_Parametor;

	private:
		Parametor m_param;								//�p�����[�^

		std::vector<ex_weak_ptr<GameObject>> m_objects; //���������I�u�W�F�N�g�̔z��

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="parametor">�p�����[�^</param>
		CollisionObjectManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// �Փ˃I�u�W�F�N�g�̐���
		/// </summary>
		/// <param name="baseScale">��{�̑傫��</param>
		/// <param name="offsetData">�I�t�Z�b�g�f�[�^</param>
		/// <returns>���������Փ˃I�u�W�F�N�g</returns>
		std::shared_ptr<GameObject> CreateCollisionObject(const Vec3& baseScale, const PositionScaleRotation& offsetData);

		/// <summary>
		/// �Փ˃I�u�W�F�N�g�̍폜
		/// </summary>
		void RemoveCollisionObject();

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �R���W�����̃f�o�b�O�\����On,Off
		/// </summary>
		/// <param name="isDrawActive">�\�����邩�ǂ���</param>
		void SetCollisionDrawActive(const bool isDrawActive);

		/// <summary>
		/// �R���W������Update�̐؂�ւ�
		/// </summary>
		/// <param name="isUpdateActive">�X�V���邩�ǂ���</param>
		void SetCollisionUpdateActive(const bool isUpdateActive);

		/// <summary>
		/// �S�ẴR���W�����I�u�W�F�N�g���擾
		/// </summary>
		/// <returns>�S�ẴR���W�����I�u�W�F�N�g</returns>
		std::vector<ex_weak_ptr<GameObject>> GetCollisionObjects() const;

		/// <summary>
		/// �R���W�����I�u�W�F�N�g�̃Z�b�e�B���O
		/// </summary>
		/// <param name="parametor">�Z�b�e�B���O�p�p�����[�^</param>
		void SettingCollisionObject(const Parametor& parametor);
	};

}