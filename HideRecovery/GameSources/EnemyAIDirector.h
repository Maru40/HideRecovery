
/*!
@file EnemyAIDirector.h
@brief EnemyAIDirector�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		class EnemyBase;
		class FactionCoordinator;

		//�t�B�[���h�S�̂̐���
		class AIDirector : public maru::SingletonComponent<AIDirector>
		{
			vector<ex_weak_ptr<EnemyBase>> m_enemys;

			//FactionCoordinator�̊Ǘ�
			std::vector<std::shared_ptr<FactionCoordinator>> m_factionCoordinators;

		public :
			AIDirector(const std::shared_ptr<GameObject>& objPtr);

			void OnLateStart() override;

		private:
			void SetEnemys();

			//Faction�̒ǉ��A�폜
			std::shared_ptr<FactionCoordinator> AddFaction();
			bool RemoveFaction(const std::shared_ptr<FactionCoordinator>& removeCoordinator);

		public:
			//�A�N�Z�b�T-------------------------------------------------------------------------------------



		};

	}
}