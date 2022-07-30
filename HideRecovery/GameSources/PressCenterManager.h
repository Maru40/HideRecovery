/*!
@file PressCenterManager.h
@brief PressCenterManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "PressData.h"
#include "HitStopManager.h"

#include "ReactiveBool.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	struct CollisionPair;
	struct PressData;
	struct PressData::MissData;
	class Pusher;

	namespace maru {
		struct HitData;
		enum class DeltaType;
		class ReactiveBool;
	}

	//--------------------------------------------------------------------------------------
	/// �ׂ����S�Ǘ�
	//--------------------------------------------------------------------------------------
	class PressCenterManager : public Component
	{
	public:

		//--------------------------------------------------------------------------------------
		/// �p�����[�^
		//--------------------------------------------------------------------------------------
		struct Parametor
		{
			float searchRange;			      //�ǂƂ̊��m�͈�
			float pressedRad;			      //Excerent����p�x
			float pressedGoodRad;		      //GoodRad�̔���p�x
			SlowTimeParametor slowParam;      //�X���[�^�C�}�[�p�����[�^
			HitStopManager::Data hitStopData; //�q�b�g�X�g�b�v�f�[�^
			maru::ReactiveBool isPressed;     //�v���C���[�Ȃǂɉ�����Ă�����
			PressData successPressData;       //�ׂ��������f�[�^

			Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="searchRange">�ǂƂ̊��m�͈�</param>
			/// <param name="pressedDegree">Excerent����p�x(�f�O���[�p)</param>
			/// <param name="pressedGoodDegree">Good����p�x(�f�O���[�p)</param>
			/// <param name="slowParam">�X���[�^�C�}�[�p�����[�^</param>
			/// <param name="hitStopData">�q�b�g�X�g�b�v�f�[�^</param>
			/// <param name="isPressed">�ׂ����������Ă��邩�ǂ���</param>
			Parametor(const float& searchRange, 
				const float& pressedDegree, const float& pressedGoodDegree,
				const SlowTimeParametor& slowParam,
				const HitStopManager::Data& hitStopData,
				const bool isPressed = false);
		};

	private:
		Parametor m_param;             //�p�����[�^

		ex_weak_ptr<Pusher> m_pusher;  //�ׂ����̃|�C���^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		PressCenterManager(const std::shared_ptr<GameObject>& objPtr);

	private:
		/// <summary>
		/// �v���X�f�[�^�̐���
		/// </summary>
		/// <param name="pair">�Փ˃f�[�^</param>
		/// <param name="missData">���s�f�[�^</param>
		/// <param name="pressType">�ׂ��^�C�v</param>
		/// <returns>���������v���X�f�[�^</returns>
		PressData CreatePressData(const CollisionPair& pair, const PressData::MissData& missData, const PressType pressType);

		/// <summary>
		/// �~�X�f�[�^�̐���
		/// </summary>
		/// <param name="pair">�Փ˃f�[�^</param>
		/// <returns>���������~�X�f�[�^</returns>
		PressData::MissData CreateMissData(const CollisionPair& pair);

		/// <summary>
		/// �߂��̐����ȏ�Q�����擾����B
		/// </summary>
		/// <returns>�߂��ɐ������肪�����Q����z��ŕԂ��B</returns>
		vector<maru::HitData> FindNearObstacleHitDatas();

		/// <summary>
		/// ��Q�����q�b�g���ɓ��邩�ǂ���
		/// </summary>
		/// <param name="data">�q�b�g�f�[�^</param>
		/// <returns>��Q���̌��Ȃ�true</returns>
		bool IsHitCandidate(const maru::HitData& data);

		/// <summary>
		/// �ׂ����Q�����ǂ���
		/// </summary>
		/// <param name="object">�Ώۂ̃I�u�W�F�N�g</param>
		/// <returns>��Q���Ȃ�true</returns>
		bool IsHitObject(const std::shared_ptr<GameObject>& object);

		/// <summary>
		/// �ׂ���}���p���ǂ���
		/// </summary>
		/// <param name="type">�ׂ��^�C�v</param>
		/// <returns>�ׂ���}���p�Ȃ�true</returns>
		bool IsPusherAngle(const PressType type);

		/// <summary>
		/// �J�����V�F�C�N�X�^�[�g
		/// </summary>
		/// <param name="time">�V�F�C�N����</param>
		/// <param name="deltaType">�o�ߎ��ԃ^�C�v</param>
		void StartCameraShake(const float& time, const maru::DeltaType& deltaType);

		/// <summary>
		/// �����Ԃ���������
		/// </summary>
		/// <param name="pair">�Փ˃f�[�^</param>
		/// <param name="pressType">�ׂ��^�C�v</param>
		void SuccessPressProcess(const CollisionPair& pair, const PressType pressType);

		/// <summary>
		/// �����Ԃ����s����
		/// </summary>
		/// <param name="pair">�Փ˃f�[�^</param>
		/// <param name="pressType">�ׂ��^�C�v</param>
		void MissPressProcess(const CollisionPair& pair, const PressType pressType);

		/// <summary>
		/// �X���C���ɓ���������
		/// </summary>
		/// <param name="pair">�Փ˃f�[�^</param>
		void SlimeHit(const CollisionPair& pair);

		/// <summary>
		/// ��Q���ɓ����������ɔ��������āA����������ׂ��B
		/// </summary>
		/// <param name="pair">�Փ˃f�[�^</param>
		void PressObstacleHit(const CollisionPair& pair);

		/// <summary>
		/// �q�b�g����
		/// </summary>
		/// <param name="pair">�Փ˃f�[�^</param>
		void OnCollisionEnter(const CollisionPair& pair) override;

		/// <summary>
		/// �q�b�g����
		/// </summary>
		/// <param name="pair">�Փ˃f�[�^</param>
		void OnCollisionExcute(const CollisionPair& pair) override;

	public:

		/// <summary>
		/// �ׂ��^�C�v���v�Z����
		/// </summary>
		/// <param name="pair">�Փ˃f�[�^</param>
		/// <returns>�ׂ��^�C�v</returns>
		PressType CalculatePressType(const CollisionPair& pair) const;

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �����ɓ�����I�u�W�F�N�g�̌��ꗗ���擾����
		/// </summary>
		/// <returns>�����ɓ�����I�u�W�F�N�g�̌��ꗗ</returns>
		vector<maru::HitData> FindHitCandidateHitDatas();

		/// <summary>
		/// �ׂ���Ă����Ԃ̐ݒ�
		/// </summary>
		/// <param name="isPressed">�ׂ���Ă����Ԃ��ǂ���</param>
		void SetIsPressed(const bool isPressed) noexcept;

		/// <summary>
		/// �ׂ���Ă����Ԃ��ǂ������擾
		/// </summary>
		/// <returns>�ׂ���Ă����Ԃ��ǂ���</returns>
		bool IsPressed() const;

		/// <summary>
		/// �ׂ���Ă����Ԃ��؂�ւ�����Ƃ��ɌĂяo���C�x���g�̒ǉ�
		/// </summary>
		/// <param name="b">�ǂ����̏�Ԃɐ؂�ւ�����Ƃ��ɌĂяo����</param>
		/// <param name="function">�؂�ւ�����Ƃ��ɌĂяo���C�x���g</param>
		void AddIsPressedReactiveFunction(const bool b, const std::function<void()>& function);

		/// <summary>
		/// �ׂ����̐ݒ�
		/// </summary>
		/// <param name="pusher">�ׂ���</param>
		void SetPusher(const std::shared_ptr<Pusher>& pusher) noexcept {
			m_pusher = pusher;
		}

		/// <summary>
		/// �ׂ����̎擾
		/// </summary>
		/// <returns>�ׂ���</returns>
		std::shared_ptr<Pusher> GetPusher() const {
			return m_pusher.GetShard();
		}

		/// <summary>
		/// �ׂ������f�[�^�̐ݒ�
		/// </summary>
		/// <param name="data">�ׂ������f�[�^</param>
		void SetSuccessPressData(const PressData& data) noexcept;

		/// <summary>
		/// �ׂ������f�[�^�̎擾
		/// </summary>
		/// <returns>�ׂ������f�[�^</returns>
		PressData GetSuccessPressData() const noexcept;
	};

}

//endbasecross