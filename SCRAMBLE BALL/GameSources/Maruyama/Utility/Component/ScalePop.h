/*!
@file ScalePop.h
@brief ScalePop�w�b�_�@
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	namespace maru {
		enum class DeltaType;
	}

	//--------------------------------------------------------------------------------------
	///	�X�P�[���|�b�v�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct ScalePop_Parametor {
		Vec3 maxScale;              //�ő�̑傫��
		Vec3 returnScale;           //��x�傫���Ȃ��Ă���߂�傫��
		float speed;                //�傫���ύX���x
		float startScale;           //�����̑傫��
		maru::DeltaType deltaType;  //���Ԍv���^�C�v

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		ScalePop_Parametor();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="maxScale">�ő�̑傫��</param>
		/// <param name="returnScale">��x�傫���Ȃ��Ă���߂�傫��</param>
		/// <param name="speed">�傫���ύX���x</param>
		ScalePop_Parametor(const Vec3& maxScale, const Vec3& returnScale, const float& speed);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="maxScale">�ő�̑傫��</param>
		/// <param name="returnScale">��x�傫���Ȃ��Ă���߂�傫��</param>
		/// <param name="speed">�傫���X�V���x</param>
		/// <param name="startScale">�����̑傫��</param>
		ScalePop_Parametor(const Vec3& maxScale, const Vec3& returnScale, const float& speed, const float& startScale);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="maxScale">�ő�̑傫��</param>
		/// <param name="returnScale">��x�傫���Ȃ��Ă���߂�傫��</param>
		/// <param name="speed">�傫���ύX���x</param>
		/// <param name="startScale">�����̑傫��</param>
		/// <param name="deltaType">���Ԍv���^�C�v</param>
		ScalePop_Parametor(const Vec3& maxScale, const Vec3& returnScale, const float& speed, const float& startScale, maru::DeltaType deltaType);
	};

	//--------------------------------------------------------------------------------------
	///	�X�P�[���|�b�v
	//--------------------------------------------------------------------------------------
	class ScalePop : public Component
	{
	public:
		using Parametor = ScalePop_Parametor;

	private:
		Vec3 m_maxScale;                //�ő�X�P�[��
		Vec3 m_returnScale;				//��x�傫���Ȃ��Ă���߂�X�P�[��
		float m_speed;					//�X�P�[���ύX�X�s�[�h
		maru::DeltaType m_deltaType;	//���Ԍv���^�C�v
										
		float m_elapsed = 0.0f;         //���݂̌v������

		/// <summary>
		/// �X�P�[���̕ϓ����I���������ǂ����𔻒f
		/// </summary>
		/// <param name="scale">���݂̃X�P�[��</param>
		/// <param name="targetScale">�ڕW�̃X�P�[��</param>
		/// <returns>�I������Ȃ�true</returns>
		bool IsStopScale(const Vec3& scale, const Vec3& targetScale);

		/// <summary>
		/// �傫���X�V
		/// </summary>
		void ScaleUpdate();

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		ScalePop(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="parametor">�p�����[�^</param>
		ScalePop(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnUpdate() override;

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �ő�̑傫���̐ݒ�
		/// </summary>
		/// <param name="scale">�ő�̑傫��</param>
		void SetMaxScale(const Vec3& scale) noexcept { m_maxScale = scale; }

		/// <summary>
		/// �ő�̑傫���̎擾
		/// </summary>
		/// <returns>�ő�̑傫��</returns>
		Vec3 GetMaxScale() const noexcept { return m_maxScale; }

		/// <summary>
		/// ��x�傫���Ȃ��Ă���߂�傫���̐ݒ�
		/// </summary>
		/// <param name="scale">��x�傫���Ȃ��Ă���߂�傫��</param>
		void SetReturnScale(const Vec3& scale) noexcept { m_returnScale = scale; }

		/// <summary>
		/// ��x�傫���Ȃ��Ă���߂�傫���̎擾
		/// </summary>
		/// <returns>��x�傫���Ȃ��Ă���߂�傫��</returns>
		Vec3 GetReturnScale() const noexcept { return m_returnScale; }

		/// <summary>
		/// �傫���ύX���x�̐ݒ�
		/// </summary>
		/// <param name="speed">�傫���ύX���x</param>
		void SetSpeed(const float speed) noexcept { m_speed = speed; }

		/// <summary>
		/// �傫���ύX���x�̎擾
		/// </summary>
		/// <returns>�傫���ύX���x</returns>
		float GetSpeed() const noexcept { return m_speed; }

		/// <summary>
		/// �傫���ύX�J�n
		/// </summary>
		/// <param name="speed">�傫���ύX���x</param>
		/// <param name="startScaleSize">�J�n���̑傫��</param>
		void StartPop(const float& speed, const float& startScaleSize = 0.0f) {
			m_speed = speed;
			m_elapsed = startScaleSize;
			ScaleUpdate();
		}

		/// <summary>
		/// �I�����f
		/// </summary>
		/// <returns>�傫���ύX������������true</returns>
		bool IsPopEnd() const {
			return m_speed == 0.0f;
		}
	};

}

//endbasecross