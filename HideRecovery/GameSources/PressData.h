/*!
@file PressData.h
@brief PressData�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "SlowTimerManager.h"

#include "ReactiveProperty.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class Pusher;
	struct SlowTimeParametor;
	struct CollisionPair;

	//--------------------------------------------------------------------------------------
	/// �ׂ��^�C�v
	//--------------------------------------------------------------------------------------
	enum class PressType
	{
		Perfect,  //�p�[�t�F�N�g����
		Good,     //�O�b�h����
		Miss,     //�~�X����
	};

	//--------------------------------------------------------------------------------------
	/// �ׂ��f�[�^
	//--------------------------------------------------------------------------------------
	struct PressData
	{
		/// <summary>
		/// �ׂ����s�f�[�^
		/// </summary>
		struct MissData
		{
			Vec3 pusherVec = Vec3(0.0f);      //�ׂ�����
			Vec3 refrectionVec = Vec3(0.0f);  //���˕���
		};

		ReactiveProperty<GameObject> pusher;  //�ׂ��Ă�������
		ex_weak_ptr<GameObject> pressed;      //�ׂ���鎩�����g
		ex_weak_ptr<GameObject> obstacle;     //�ׂ��ǂȂǂ̏�Q��
		SlowTimeParametor slowParam;          //�X���[�^�C�}�[�p�����[�^
		MissData missData;                    //MissData
		PressType pressType;                  //�v���X�^�C�v
		CollisionPair collisionPair;          //�R���W�����f�[�^
		Vec3 pressedVec;                      //�ׂ�����
		Vec3 lostPusherPosition;              //�ׂ����̃I�u�W�F�N�g�����X�g�����ꏊ

		/// <summary>
		///	�R���X�g���N�^
		/// </summary>
		PressData();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="pusher">�ׂ��Ă�������</param>
		/// <param name="pressed">�ׂ���鎩�����g</param>
		/// <param name="obstacle">��Q��</param>
		PressData(const std::shared_ptr<GameObject>& pusher, 
			const std::shared_ptr<GameObject>& pressed, 
			const std::shared_ptr<GameObject>& obstacle);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="pusher">�ׂ��Ă�������</param>
		/// <param name="pressed">�ׂ���鎩�����g</param>
		/// <param name="obstacle">��Q��</param>
		/// <param name="slowParam">�X���[�^�C���p�����[�^</param>
		/// <param name="missData">�~�X�f�[�^</param>
		/// <param name="pressType">�ׂ��^�C�v</param>
		/// <param name="collisionPair">�Փ˃f�[�^</param>
		PressData(const std::shared_ptr<GameObject>& pusher,
			const std::shared_ptr<GameObject>& pressed,
			const std::shared_ptr<GameObject>& obstacle,
			const SlowTimeParametor& slowParam,
			const MissData& missData,
			const PressType pressType,
			const CollisionPair& collisionPair);

		/// <summary>
		/// �ׂ��Ă���������擾
		/// </summary>
		/// <returns>�ׂ��Ă�������</returns>
		std::shared_ptr<Pusher> GetPusher() const;

		/// <summary>
		/// �ׂ��͂̎擾
		/// </summary>
		/// <returns>�ׂ���</returns>
		float CalculatePressPower() const;

		/// <summary>
		/// �ׂ���鑤����ׂ����ւ̃x�N�g��
		/// </summary>
		/// <returns>�ׂ���鑤����ׂ����ւ̃x�N�g��</returns>
		Vec3 GetPressedToPuserVec() const;

		/// <summary>
		/// �ׂ�������ׂ���鑤�ւ̃x�N�g��
		/// </summary>
		/// <returns>�ׂ�������ׂ���鑤�ւ̃x�N�g��</returns>
		Vec3 GetPusherToPressedVec() const;

		/// <summary>
		/// �q�b�g�|�C���g����ׂ���鑤�ւ̃x�N�g��
		/// </summary>
		/// <returns>�q�b�g�|�C���g����ׂ���鑤�ւ̃x�N�g��</returns>
		Vec3 GetHitPointToPressedVec() const;

		/// <summary>
		/// Pusher�̃|�W�V�������擾
		/// </summary>
		/// <returns>Pusher�̃|�W�V����</returns>
		Vec3 GetPusherPosition() const;

		/// <summary>
		/// PusherObject�̃R���|�[�l���g���擾����B
		/// </summary>
		/// <returns>PusherObject�̃R���|�\�l���g</returns>
		template<class T>
		std::shared_ptr<T> GetPusherObjectComponent() const {
			return pusher ? pusher->GetComponent<T>(false) : nullptr;
		}
	};

}

//endbasecross