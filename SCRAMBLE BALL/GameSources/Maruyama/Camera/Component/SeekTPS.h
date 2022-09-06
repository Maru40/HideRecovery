/*!
@file SeekTPS.h
@brief SeekTPS�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	TPS�Ǐ]�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class SeekTPS : public Component
	{
	public:
		//--------------------------------------------------------------------------------------
		///	�Ǐ]�^�C�v
		//--------------------------------------------------------------------------------------
		enum class SeekType
		{
			Normal,				//�ʏ�
			Lerp,				//�⊮
		};

		//--------------------------------------------------------------------------------------
		///	�p�����[�^
		//--------------------------------------------------------------------------------------
		struct Parametor
		{
			float armRange;		//�A�[���̒���
			Vec3  armVec;		//�A�[���̕���

			float radY;			//y���̊p�x
			float radXZ;		//xz���̊p�x

			float speedY;		//y���̑��x
			float speedXZ;		//xz���̑��x

			float maxY;			//y���̍ő�
			float minY;			//y���̍ŏ�

			float lerpSpeed;	//��Ԉړ����x
			SeekType seekType;  //�Ǐ]�^�C�v

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="armRange">�A�[���̒���</param>
			/// <param name="armVec">�A�[���̕���</param>
			/// <param name="radY">y���̊p�x</param>
			/// <param name="radXZ">zy���̊p�x</param>
			/// <param name="speedY">y���̑��x</param>
			/// <param name="speedXZ">xz���̑��x</param>
			/// <param name="maxY">y���̍ő�p�x</param>
			/// <param name="minY">z���̍ő�p�x</param>
			/// <param name="lerpSpeed">�⊮�ړ����x</param>
			/// <param name="seekType">�Ǐ]�^�C�v</param>
			Parametor(const float& armRange,
				const Vec3& armVec,
				const float& radY,
				const float& radXZ,
				const float& speedY,
				const float& speedXZ,
				const float& maxY,
				const float& minY,
				const float& lerpSpeed,
				const SeekType& seekType);
		};

	private:

		ex_weak_ptr<GameObject> m_target;   //�^�[�Q�b�g
		Parametor m_param;					//�p�����[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="target">�^�[�Q�b�g</param>
		SeekTPS(const std::shared_ptr<GameObject>& objPtr,
			const std::shared_ptr<GameObject>& target
		):
			SeekTPS(
				objPtr,
				target,
				Parametor())
		{}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="target">�^�[�Q�b�g</param>
		/// <param name="parametor">�p�����[�^</param>
		SeekTPS(const std::shared_ptr<GameObject>& objPtr,
			const std::shared_ptr<GameObject>& target,
			const Parametor& parametor
		) :
			Component(objPtr),
			m_target(target),
			m_param(parametor)
		{}

		void OnCreate() override;
		void OnUpdate2() override;

	private:
		/// <summary>
		/// �ړI�n���v�Z����B
		/// </summary>
		/// <returns>�v�Z�����ړI�n</returns>
		Vec3 CalcuMovePosition();

		/// <summary>
		/// Y�����̓���
		/// </summary>
		void InputYVec();

		/// <summary>
		/// �������̓���
		/// </summary>
		void InputRXVec();

		/// <summary>
		/// Y�����̓��͍ő�l�`�F�b�N
		/// </summary>
		void CheckRimitY();

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �A�[���̒����̎擾
		/// </summary>
		/// <returns>�A�[������</returns>
		float GetArmRange() const noexcept {
			return m_param.armRange;
		}

		/// <summary>
		/// �A�[���̒����̐ݒ�
		/// </summary>
		/// <param name="range">�A�[���̒���</param>
		void SetArmRange(const float range) noexcept {
			m_param.armRange = range;
		}

		/// <summary>
		/// �^�[�Q�b�g�̐ݒ�
		/// </summary>
		/// <param name="target">�^�[�Q�b�g</param>
		void SetTarget(const std::shared_ptr<GameObject>& target) noexcept {
			m_target = target;
		}

		/// <summary>
		/// �A�[���̕����̐ݒ�
		/// </summary>
		/// <param name="armVec">�A�[���̕���</param>
		void SetArmVec(const Vec3& armVec) noexcept {
			m_param.armVec = armVec;
		}

		/// <summary>
		/// �A�[���̃I�t�Z�b�g�̎擾
		/// </summary>
		/// <returns></returns>
		Vec3 GetArmOffset() const {
			return m_param.armVec * m_param.armRange;
		}

		/// <summary>
		/// �Ǐ]�^�C�v�̐ݒ�
		/// </summary>
		/// <param name="type">�Ǐ]�^�C�v</param>
		void SetSeekType(const SeekType type) noexcept {
			m_param.seekType = type;
		}

		/// <summary>
		/// �Ǐ]�^�C�v�̎擾
		/// </summary>
		/// <returns>�Ǐ]�^�C�v</returns>
		SeekType GetSeekType() const noexcept {
			return m_param.seekType;
		}

		/// <summary>
		/// �p�����[�^�̐ݒ�
		/// </summary>
		/// <param name="parametor">�p�����[�^</param>
		void SetParametor(const Parametor& parametor) noexcept {
			m_param = parametor;
		}

		/// <summary>
		/// �p�����[�^�̎擾
		/// </summary>
		/// <returns>�p�����[�^</returns>
		Parametor GetParametor() const noexcept {
			return m_param;
		}
	};

}