/*!
@file OriginalMeshToonObject.h
@brief OriginalMeshToonObject�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "StageObjectBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�I���W�i�����b�V���g�D�[���I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class OriginalMeshToonObject : public StageObjectBase
	{
		bool m_isStart = false;    //�J�n�������������ǂ���
		bool m_isCollision = true; //�Փ˔���𐶐����邩�ǂ���
		Vec3 m_scaleOffset;        //�傫���I�t�Z�b�g

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		/// <param name="name">�I�u�W�F�N�g��</param>
		/// <param name="textrue">�e�N�X�`����</param>
		OriginalMeshToonObject(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& textrue);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// �Փ˔���I�u�W�F�N�g�𐶐�
		/// </summary>
		/// <param name="scale">�傫��</param>
		void CreateCollisionObject(const Vec3& scale);

	public:
		/// <summary>
		/// �Փ˔�����Ƃ邩�ǂ����ݒ肷��
		/// </summary>
		/// <param name="isCollision">�Փ˔�����Ƃ邩�ǂ���</param>
		void SetIsCollision(const bool isCollision) {
			m_isCollision = isCollision;
		}

	};

}

//endbasecross