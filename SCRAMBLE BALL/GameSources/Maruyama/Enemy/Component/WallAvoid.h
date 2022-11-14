
/*!
@file WallAvoid.h
@brief WallAvoid�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	
	//--------------------------------------------------------------------------------------
	///	�ǉ��p�̐G�o
	//--------------------------------------------------------------------------------------
	class WallAvoidTacticle {
		float m_range;	//����
		float m_degree;	//�p�x

	public:

		WallAvoidTacticle();

		WallAvoidTacticle(const float degree);

		WallAvoidTacticle(const float range, const float degree);

		float GetRange() const noexcept { return m_range; }

		float GetDegree() const noexcept { return m_degree; }

		float GetRad() const { return XMConvertToRadians(m_degree); }
	};

	//--------------------------------------------------------------------------------------
	///	�ǉ���p�����[�^
	//--------------------------------------------------------------------------------------
	struct WallAvoid_Parametor {
		float avoidPower;
		Vec3 offset;

		WallAvoid_Parametor();
	};

	//--------------------------------------------------------------------------------------
	///	�ǉ��
	//--------------------------------------------------------------------------------------
	class WallAvoid : public Component 
	{
	public:
		using Parametor = WallAvoid_Parametor;

	private:
		Parametor m_param;

		Vec3 m_avoidVec;	//�ǉ���̃x�N�g��

		std::weak_ptr<Transform> m_transform;

		std::vector<std::shared_ptr<WallAvoidTacticle>> m_tacticles;	//�G�o�z��
		std::vector<std::weak_ptr<GameObject>> m_obstacles;				//��Q���Q

	public:
		WallAvoid(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnStart() override;
		void OnLateStart() override;

		void OnUpdate() override;

	private:
		/// <summary>
		/// ��Q���悯
		/// </summary>
		void AvoidUpdate();

		/// <summary>
		/// ��Q���悯�x�N�g�����v�Z���Ď擾
		/// </summary>
		/// <returns></returns>
		Vec3 CalculateAvoidVec(const std::shared_ptr<WallAvoidTacticle>& tacticle);

		/// <summary>
		/// ���C�̕������v�Z���Ď擾����
		/// </summary>
		/// <param name="tacticle">���C����p�̐G�o</param>
		/// <returns></returns>
		Vec3 CalculateRayDirection(const std::shared_ptr<WallAvoidTacticle>& tacticle);

		/// <summary>
		/// �f�t�H���g�̏�Q���z��𐶐�
		/// </summary>
		void SettingDefaultObstacles();

		/// <summary>
		/// �f�t�H���g�G�o�𐶐�
		/// </summary>
		void SettingDefaultTacticles();

	public:

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		Vec3 TakeAvoidVec() noexcept {
			auto result = m_avoidVec;
			m_avoidVec = Vec3(0.0f);
			return result;
		}
	};

}