/*!
@file WaterDraw.h
@brief WaterDraw�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "BulletBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class LaserBaseDraw;
	struct ParticleColliderManager_Parametor;
	
	namespace maru {
		class ParticleManager;
	}

	//--------------------------------------------------------------------------------------
	/// �����W�F�b�g�`��p�����[�^
	//--------------------------------------------------------------------------------------
	struct WaterDraw_Parametor
	{
		std::shared_ptr<ParticleColliderManager_Parametor> particleCollisionManagerParam; //�p�[�e�B�N���}�l�[�W���̃p�����[�^

		WaterDraw_Parametor();
	};

	//--------------------------------------------------------------------------------------
	/// �����W�F�b�g�`��
	//--------------------------------------------------------------------------------------
	class WaterDraw : public BulletBase
	{
	public:
		using Parametor = WaterDraw_Parametor;

	private:
		Parametor m_param;                                    //�p�����[�^

		ex_weak_ptr<GameObject> m_body;                       //�{��
		ex_weak_ptr<GameObject> m_boardAura;                  //�I�[��
		ex_weak_ptr<maru::ParticleManager> m_particleManager; //�p�[�e�B�N���}�l�[�W��

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		WaterDraw(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;

	private:
		/// <summary>
		/// �p�[�e�B�N���}�l�[�W���\�̐���
		/// </summary>
		void CreatePartilceManager();

		/// <summary>
		/// ��΂�����
		/// </summary>
		/// <param name="direct">��΂�����</param>
		void Shot(const Vec3& direct) override;

		/// <summary>
		/// ��΂������̏I�����ɌĂԏ���
		/// </summary>
		void ShotEnd() override;

		/// <summary>
		/// Body�̈ʒu��ݒ肷��B
		/// </summary>
		void SetBodyPosition();

		/// <summary>
		/// �p�[�e�B�N���R���W�����̒�����ݒ肷��
		/// </summary>
		void SetParticleCollisionLength();

	};

}