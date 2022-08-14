
/*!
@file FactionCoordinator.h
@brief FactionCoordinator�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "CoordinatorBase.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class AIDirector;
		class EnemyBase;
		class CombatCoordinator;
		class PatrolCoordinator;
		class FactionCoordinator;
		class I_FactionMember;

		//--------------------------------------------------------------------------------------
		/// �O���[�v�^�C�v
		//--------------------------------------------------------------------------------------
		enum class FactionType
		{
			None,	//�������Ă��Ȃ�
			Patrol,	//�p�g���[��
			Combat,	//�퓬
		};

		//--------------------------------------------------------------------------------------
		/// �O���[�v������
		//--------------------------------------------------------------------------------------
		class FactionCoordinator : public CoordinatorBase<EnemyBase>
		{
		public:

		private:
			std::weak_ptr<AIDirector> m_director;	//AI�f�B���N�^�[

			std::vector<std::shared_ptr<PatrolCoordinator>> m_patrols;			//�p�g���[���O���[�v
			std::vector<std::shared_ptr<CombatCoordinator>> m_combats;			//�U���p�O���[�v
			std::vector<std::shared_ptr<CoordinatorBase>> m_allCoordinators;	//�S�ẴO���[�v

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="director">AI�f�B���N�^�[</param>
			FactionCoordinator(const std::shared_ptr<AIDirector>& director);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="director">AI�f�B���N�^�[</param>
			/// <param name="members">�����o�[�z��</param>
			FactionCoordinator(const std::shared_ptr<AIDirector>& director, const std::vector<std::weak_ptr<EnemyBase>>& members);

			virtual ~FactionCoordinator() = default;

			void OnStart() override;
			void OnUpdate() override;
			void OnExit() override;

		private:
			/// <summary>
			/// �O���[�v�̐���
			/// </summary>
			template<class T, class... Ts>
			std::shared_ptr<T> CreateFaction(std::vector<std::shared_ptr<T>>& addVec, Ts&&... params) {
				auto faction = std::make_shared<T>(params...);
				faction->OnStart();
				addVec.push_back(faction);
				m_allCoordinators.push_back(faction);

				return faction;
			}

		public:
			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �f�B���N�^�[�̎擾
			/// </summary>
			/// <returns>�f�B���N�^�[</returns>
			std::shared_ptr<AIDirector> GetDirector() const noexcept { return m_director.lock(); }





			//--------------------------------------------------------------------------------------
			/// �f�o�b�O
			//--------------------------------------------------------------------------------------

			//�^�[�Q�b�g���������ƒm�点��f�o�b�O����
			void DebugWriteTarget(const std::shared_ptr<I_FactionMember>& member, const std::shared_ptr<GameObject>& target);
		};

	}
}