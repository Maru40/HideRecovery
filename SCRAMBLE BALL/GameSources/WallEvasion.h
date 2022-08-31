/*!
@file WallEvasion.h
@brief WallEvasion
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"
#include "TactileCtrl.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	��Q�����p�̃f�[�^
	//--------------------------------------------------------------------------------------
	struct ObstacleRangePair
	{
		ex_weak_ptr<GameObject> object;	//�I�u�W�F�N�g�f�[�^
		float range;					//�������

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		ObstacleRangePair()
			:ObstacleRangePair(nullptr, 0.0f)
		{}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">�I�u�W�F�N�g�f�[�^</param>
		/// <param name="range">�������</param>
		ObstacleRangePair(const std::shared_ptr<GameObject>& objPtr, const float& range)
			:object(objPtr), range(range)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	��Q�����
	//--------------------------------------------------------------------------------------
	class WallEvasion : public Component
	{
		std::vector<ex_weak_ptr<GameObject>> m_obstacleObjs;	//��Q���̑Ώ۔z��
		float m_maxSpeed = 30.0f;								//�ő��𑬓x

		std::vector<ex_weak_ptr<GameObject>> m_tactileObjects;	//�G�o���I�u�W�F�N�g�z��

		/// <summary>
		/// �ǂƂ̋����𑪒肵�āA���������ɕ��ׂĔz���Ԃ�
		/// </summary>
		/// <returns>�ǂƂ̋����𑪒肵���z��</returns>
		vector<ObstacleRangePair> CalucWallRangeSort();

		/// <summary>
		/// ������͂��v�Z����B
		/// </summary>
		/// <param name="tactile">�G�o�I�u�W�F�N�g</param>
		/// <returns>�v�Z������</returns>
		Vec3 CalucForce(const std::shared_ptr<GameObject>& tactile);

		/// <summary>
		/// �ǉ������
		/// </summary>
		/// <param name="tactile">�G�o�I�u�W�F�N�g</param>
		/// <param name="other">�ڐG�����I�u�W�F�N�g</param>
		void EvasionUpdate(const std::shared_ptr<TactileCtrl>& tactile, const std::shared_ptr<GameObject>& other);

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		WallEvasion(const std::shared_ptr<GameObject>& objPtr)
			:Component(objPtr)
		{}
		
		void OnDestroy() override;

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ��Q������̃I�u�W�F�N�g�𑝂₷�B
		/// </summary>
		/// <param name="object">��Q���̃I�u�W�F�N�g</param>
		void AddObstacleObjs(const std::shared_ptr<GameObject>& object) {
			m_obstacleObjs.push_back(object);
		}

		/// <summary>
		/// �G�o�̃Z�b�g
		/// </summary>
		/// <param name="tactile">�G�o�̃I�u�W�F�N�g</param>
		void SetTactile(const std::shared_ptr<TactileCtrl>& tactile);

		/// <summary>
		/// �ő呬�x�̐ݒ�
		/// </summary>
		/// <param name="speed">�ő呬�x</param>
		void SetMaxSpeed(const float& speed) noexcept { m_maxSpeed = speed; }

		/// <summary>
		/// �ő呬�x�̎擾
		/// </summary>
		/// <returns>�ő呬�x</returns>
		float GetMaxSpeed() const noexcept { return m_maxSpeed; }

	};

}

//endbasecross