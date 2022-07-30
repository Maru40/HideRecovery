/*!
@file Pusher.h
@brief Pusher�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "ReactiveBool.h"

#include "PressData.h"
#include "MaruAction.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	struct CollisionPair;
	struct PressData;

	//--------------------------------------------------------------------------------------
	/// �ׂ��^�[�Q�b�g�f�[�^
	//--------------------------------------------------------------------------------------
	struct PushTargetData {
		ex_weak_ptr<GameObject> target; //�^�[�Q�b�g
		CollisionPair collisionPair;    //�Փ˃f�[�^
		
		PushTargetData();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="target">�^�[�Q�b�g</param>
		/// <param name="collisionPair">�Փ˃f�[�^</param>
		PushTargetData(const std::shared_ptr<GameObject>& target, const CollisionPair& collisionPair);
	};

	//--------------------------------------------------------------------------------------
	/// �ׂ���
	//--------------------------------------------------------------------------------------
	class Pusher : public Component
	{
	public:
		/// <summary>
		/// �p�����[�^
		/// </summary>
		struct Parametor
		{
			float slowMoveSpeed = 0.0f;				   //�X���[�ړ��X�s�[�h
			Vec3 slowMoveDirect = Vec3(0.0f);		   //�X���[�ړ�����
			float missPressSpeed = 0.0f;			   //���s���ׂ̒��X�s�[�h
			float missDeselerationTime = 0.5f;		   //���s���̌�������
			float missDeselerationPower = 20.0f;	   //���s���̌�����
			float energyDebuffRate = 0.5f;			   //�G�i�W�[�f�o�t
			float energyDebuffTime = 1.0f;			   //�f�o�t����
			float pushPower;						   //�ׂ���
			maru::ReactiveBool isPush = false;         //Push���Ă��邩�ǂ����B
			maru::ReactiveBool isPushing = false;      //�v�b�V���O�����ǂ����B
			maru::ReactiveBool isMissPressing = false; //MissPressing��ԁB
			std::shared_ptr<PressData> data;           //�ׂ��f�[�^
			maru::Action<void(Pusher&)> destroyAction; //���S���ɌĂяo���C�x���g

			Parametor();
		};

	private:
		Parametor m_param;                         //�p�����[�^
		std::vector<PushTargetData> m_targetDatas; //�^�[�Q�b�g�f�[�^�z��

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		Pusher(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;

	private:
		/// <summary>
		/// �^�b�N���X�e�[�g�̃`�F�b�N
		/// </summary>
		void CheckTackleState();

		/// <summary>
		/// �ׂ����s���̍X�V����
		/// </summary>
		void MissPressUpdate();

		/// <summary>
		/// �ړ��X�V
		/// </summary>
		/// <param name="speed">�ړ��X�s�[�h</param>
		/// <param name="direct">�ړ�����</param>
		void MoveUpdate(const float& speed, const Vec3& direct);

		/// <summary>
		/// �ׂ��Ă��鎞�̍X�V����
		/// </summary>
		void PressingUpdate();

		/// <summary>
		/// �ׂ��^�[�Q�b�g�ɒׂ��Ă��邱�Ƃ�`����B
		/// </summary>
		void PushTargetUpdate();

		/// <summary>
		/// �ׂ����I��������ɌĂяo������
		/// </summary>
		/// <param name="target">�ׂ��I������^�[�Q�b�g</param>
		void EndPush(const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// �������ׂ����Ԃ����f
		/// </summary>
		/// <returns>�ׂ����ԂȂ�true</returns>
		bool IsPush() const;

		/// <summary>
		/// �^�[�Q�b�g��ǉ��ł��邩�ǂ���
		/// </summary>
		/// <returns>�ǉ��ł���Ȃ�true</returns>
		bool IsAddTarget(const std::shared_ptr<GameObject>& target) const;

		/// <summary>
		/// Pusher�ɓ����������̏���
		/// </summary>
		/// <param name="pair">������������̃f�[�^</param>
		void HitPusher(const CollisionPair& pair);

		/// <summary>
		/// �����J�n
		/// </summary>
		void StartDeseleration();

		/// <summary>
		/// Pushing���ɏ�Q���Ƀq�b�g������
		/// </summary>
		void HitPushingObstacle(const CollisionPair& pair);

		/// <summary>
		/// ���������Ƃ��̏���
		/// </summary>
		/// <param name="pair">������������</param>
		void OnCollisionEnter(const CollisionPair& pair) override;

		/// <summary>
		/// �����葱���Ă���ԌĂԏ���
		/// </summary>
		/// <param name="pair">������������</param>
		void OnCollisionExcute(const CollisionPair& pair) override;

		/// <summary>
		/// �^�b�N���X�s�[�h�̎擾
		/// </summary>
		/// <returns>�^�b�N���X�s�[�h</returns>
		float GetTackleSpeed() const;

	public:
		/// <summary>
		/// �^�[�Q�b�g�̃v�b�V�����n�߂�B
		/// </summary>
		/// <param name="pair">������������</param>
		void StartPush(const CollisionPair& pair);

		/// <summary>
		/// �����Ԃ������̔���
		/// </summary>
		/// <param name="backVec">�����Ԃ������̔����x�N�g��</param>
		void PushBack(const Vec3& backVec);

		/// <summary>
		/// �����Ԃ����J�n(StartPush��Push���ɓ��ꂽ���̑S�ē����)
		/// </summary>
		/// <param name="data">�ׂ��f�[�^</param>
		void SuccessPressing(const PressData& data);

		/// <summary>
		/// �����Ԃ��̎��s�X�^�[�g
		/// </summary>
		/// <param name="pair">������������</param>
		void MissPressing(const CollisionPair& pair);

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �^�[�Q�b�g�f�[�^�̒ǉ�
		/// </summary>
		/// <param name="data">�^�[�Q�b�g�f�[�^</param>
		void AddPushTargetData(const PushTargetData& data) {
			if (IsAddTarget(data.target.GetShard())) {
				m_targetDatas.push_back(data);
			}
		}

		/// <summary>
		///	�ׂ��Ă����Ԃ̐ݒ�
		/// </summary>
		/// <param name="isPush">�ׂ��Ă�����</param>
		void SetIsPush(const bool isPush) {
			m_param.isPush = isPush;
		}

		/// <summary>
		/// �ׂ��Ă����Ԃ��؂�ւ�����Ƃ��ɌĂяo�������C�x���g�̒ǉ�
		/// </summary>
		/// <param name="b">�ǂ����̏�Ԃɐ؂�ւ�����Ƃ��ɌĂяo��������</param>
		/// <param name="function">�Ăяo�������C�x���g</param>
		void AddReactiveIsPusherFunction(const bool b, const std::function<void()>& function);

		/// <summary>
		/// �ׂ����s��Ԃ��ǂ������擾
		/// </summary>
		/// <returns>�ׂ����s��ԂȂ�true</returns>
		bool IsMissPressing() const {
			return m_param.isMissPressing;
		}

		/// <summary>
		/// �ׂ������f�[�^�̐ݒ�
		/// </summary>
		/// <param name="data">�ׂ������f�[�^</param>
		void SetSuccessPressData(const PressData& data);

		/// <summary>
		/// �ׂ������f�[�^�̎擾
		/// </summary>
		/// <returns>�ׂ������f�[�^</returns>
		PressData GetSuccessPressData() const;

		/// <summary>
		/// ���S�C�x���g�̒ǉ�
		/// </summary>
		/// <param name="action">���S�C�x���g</param>
		void AddDestroyAction(const std::function<void(Pusher&)>& action);

	};

}

//endbasecross