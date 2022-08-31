/*!
@file SeekTarget.h
@brief SeekTargetLook�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�Ǐ]�^�C�v
	//--------------------------------------------------------------------------------------
	enum class SeekTarget_Type {
		Normal,                     //�ʏ�
		Lerp,                       //Lerp
		VelocitySeek,               //�Ǐ]�s��
		VelcoityArrive,             //�����s��
		VelocityNearSeekFarArrive,	//�߂��ɂ���Ƃ��͒Ǐ]�A�����ɂ���Ƃ��͓����s��
	};

	//--------------------------------------------------------------------------------------
	///	�Ǐ]�p�����[�^
	//--------------------------------------------------------------------------------------
	struct SeekTarget_Parametor {
		using SeekType = SeekTarget_Type;

		float speed;				//�Ǐ]�X�s�[�h
		float range;				//�Ǐ]�͈�
		Vec3 offset;				//�Ǐ]�I�t�Z�b�g
		SeekType seekType;			//�Ǐ]�^�C�v
		bool isScaleParent;			//Scale��e�ɍ��킹�邩�ǂ����B

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		SeekTarget_Parametor();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="seekType">�Ǐ]�^�C�v</param>
		SeekTarget_Parametor(const SeekType& seekType);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="range">�Ǐ]�͈�</param>
		/// <param name="offset">�Ǐ]�I�t�Z�b�g</param>
		/// <param name="seekType">�Ǐ]�^�C�v</param>
		SeekTarget_Parametor(const float& range, const Vec3& offset, const SeekType& seekType);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="speed">�Ǐ]�X�s�[�h</param>
		/// <param name="range">�Ǐ]�͈�</param>
		/// <param name="offset">�Ǐ]�I�t�Z�b�g</param>
		/// <param name="seekType">�Ǐ]�^�C�v</param>
		/// <param name="isScaleParent">�Ǐ]�I�t�Z�b�g�ɐe�I�u�W�F�N�g�̃X�P�[����K�����邩�ǂ���</param>
		SeekTarget_Parametor(const float& speed, const float& range, const Vec3& offset, const SeekType& seekType, const bool isScaleParent = false);
	};

	//--------------------------------------------------------------------------------------
	///	�^�[�Q�b�g���^�[�Q�e�B���O�����Ƃ��̋������Ǘ�����
	//--------------------------------------------------------------------------------------
	class SeekTarget : public Component
	{
	public:
		using SeekType = SeekTarget_Type;
		using Parametor = SeekTarget_Parametor;

	private:
		Parametor m_param;                //�p�����[�^
	
		ex_weak_ptr<GameObject> m_target; //�^�[�Q�b�g�̃Q�[���I�u�W�F�N�g

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="target">�^�[�Q�b�g�̃Q�[���I�u�W�F�N�g</param>
		SeekTarget(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="target">�^�[�Q�b�g�̃Q�[���I�u�W�F�N�g</param>
		/// <param name="parametor">�p�����[�^</param>
		SeekTarget(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GameObject>& target, const Parametor& parametor);

		void OnCreate() override;
		void OnUpdate() override;

	private:

		/// <summary>
		/// �v�Z�����|�W�V������Ԃ��B
		/// </summary>
		/// <returns>�v�Z�����|�W�V����</returns>
		Vec3 CalculatePosition();

		/// <summary>
		/// �Ǐ]�A�b�v�f�[�g
		/// </summary>
		void ChaseUpdate();

		/// <summary>
		/// �ʏ�̒ǂ������鏈��
		/// </summary>
		/// <param name="basePosition">�ړI�n</param>
		void NormalChase(const Vec3& basePosition);

		/// <summary>
		/// �ړI�n�ɕ�Ԃ��|�����ړ�����
		/// </summary>
		/// <param name="basePosition">�ړI�n</param>
		void LerpChase(const Vec3& basePosition);

		/// <summary>
		/// ���x�Ǘ�������ړ�����
		/// </summary>
		/// <param name="basePosition">�ړI�n</param>
		void VelocityChase(const Vec3& basePosition);

		/// <summary>
		/// �Ǐ]��̃I�t�Z�b�g���v�Z����
		/// </summary>
		/// <returns>�Ǐ]��̃I�t�Z�b�g</returns>
		Vec3 CalculateOffset() const;

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �^�[�Q�b�g�̃Z�b�g
		/// </summary>
		/// <param name="target">�^�[�Q�b�g�̃Q�[���I�u�W�F�N�g</param>
		/// <param name="isSetPositioin">�����Ƀ^�[�Q�b�g�̏ꏊ�ɍ��킹�邩�ǂ���</param>
		void SetTarget(const std::shared_ptr<GameObject>& target, bool isSetPositioin = false) {
			m_target = target;
			//�����Ƀ^�[�Q�b�g�̃|�W�V�����ɍ��킹��ꍇ
			if (target && isSetPositioin) {
				transform->SetPosition(CalculatePosition());
			}
		}

		/// <summary>
		/// �^�[�Q�b�g�̎擾
		/// </summary>
		/// <returns>�^�[�Q�b�g�̃Q�[���I�u�W�F�N�g</returns>
		std::shared_ptr<GameObject> GetTarget() const noexcept{ return m_target.GetShard(); }

		/// <summary>
		/// �Ǐ]�^�C�v�̃Z�b�g
		/// </summary>
		/// <param name="type">�Ǐ]�^�C�v</param>
		void SetSeekType(const SeekType& type) noexcept{ m_param.seekType = type; }

		/// <summary>
		/// �Ǐ]�^�C�v�̎擾
		/// </summary>
		/// <returns>���݂̒Ǐ]�^�C�v</returns>
		SeekType GetSeekType() const noexcept{ return m_param.seekType; }

		/// <summary>
		/// �p�����[�^�̃Z�b�g
		/// </summary>
		/// <param name="parametor">�p�����[�^</param>
		void SetParametor(const Parametor& parametor) noexcept { m_param = parametor; }

		/// <summary>
		/// �p�����[�^�̎擾
		/// </summary>
		/// <returns>�p�����[�^</returns>
		Parametor GetParametor() const noexcept { return m_param; }

		/// <summary>
		/// �I�t�Z�b�g�̃Z�b�g
		/// </summary>
		/// <param name="offset">�I�t�Z�b�g�l</param>
		void SetOffset(const Vec3& offset) noexcept{ m_param.offset = offset; }

		/// <summary>
		/// �I�t�Z�b�g�̎擾
		/// </summary>
		/// <returns>�I�t�Z�b�g�l</returns>
		Vec3 GetOffset() const { return CalculateOffset(); }

		/// <summary>
		/// ���[�J���ȃI�t�Z�b�g�l���擾
		/// </summary>
		/// <returns>���[�J���ȃI�t�Z�b�g�l</returns>
		Vec3 GetLocalOffset() const noexcept { return m_param.offset; }
		
		/// <summary>
		/// �Ǐ]�͈͂̃Z�b�g
		/// </summary>
		/// <returns>�Ǐ]�͈�</returns>
		void SetRange(const float& range) noexcept { m_param.range = range; }

		/// <summary>
		/// �Ǐ]�͈͂̎擾
		/// </summary>
		/// <returns>�Ǐ]�͈�</returns>
		float GetRange() const noexcept{ return m_param.range; }

		/// <summary>
		/// �ړ��X�s�[�h�̃Z�b�g
		/// </summary>
		/// <returns>�ړ��X�s�[�h</returns>
		void SetSpeed(const float& speed) noexcept { m_param.speed = speed; }

		/// <summary>
		/// �ړ��X�s�[�h�̎擾
		/// </summary>
		/// <returns>�ړ��X�s�[�h</returns>
		float GetSpeed() const noexcept { return m_param.speed; }

		/// <summary>
		/// ���[���h���W���擾
		/// </summary>
		/// <returns>���[���h���W</returns>
		Vec3 GetWorldPosition() const {
			auto targetTrans = m_target->GetComponent<Transform>();

			Mat4x4 newMat;
			newMat.affineTransformation(
				transform->GetScale(),
				transform->GetPivot(),
				Quat::Identity(),
				m_param.offset
			);

			newMat *= targetTrans->GetWorldMatrix();
			auto position = newMat.transInMatrix();
			return position;
		}
	};

}

//endbasecross