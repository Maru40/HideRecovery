
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

		//--------------------------------------------------------------------------------------
		/// �O���[�v�^�C�v
		//--------------------------------------------------------------------------------------
		enum class FactionType
		{

		};

		//--------------------------------------------------------------------------------------
		/// �O���[�v�����o�[�̃C���^�[�t�F�[�X
		//--------------------------------------------------------------------------------------
		class I_FactionMember {
		public:

		};

		//--------------------------------------------------------------------------------------
		/// �O���[�v������
		//--------------------------------------------------------------------------------------
		class FactionCoordinator : public CoordinatorBase<EnemyBase>
		{
		public:

		private:
			std::weak_ptr<AIDirector> m_director;	//AI�f�B���N�^�[

				//�p�g���[���O���[�v
			std::vector<std::shared_ptr<CombatCoordinator>> m_combats;	//�U���p�O���[�v
			std::vector<std::shared_ptr<CoordinatorBase>> m_allCoordinators;	//�S�ẴO���[�v

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="director">AI�f�B���N�^�[</param>
			FactionCoordinator(const std::shared_ptr<AIDirector>& director);

			~FactionCoordinator() = default;

			void OnStart() override;
			void OnUpdate() override;
			void OnExit() override;

		private:
			

		public:
			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �f�B���N�^�[�̎擾
			/// </summary>
			/// <returns>�f�B���N�^�[</returns>
			std::shared_ptr<AIDirector> GetDirector() const noexcept { return m_director.lock(); }

		};

	}
}