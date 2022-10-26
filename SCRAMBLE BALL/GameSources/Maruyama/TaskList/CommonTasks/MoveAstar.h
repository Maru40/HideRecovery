/*!
@file MoveAstar.h
@brief MoveAstar�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/TaskList/TaskList.h"

namespace basecross {

	class TargetManager;
	class VelocityManager;
	class SelfAstarNodeController;

	template<class T>
	class TaskList;

	struct Task_MovePositions_Parametor;

	namespace Enemy {
		class EnemyBase;
	}

	namespace Task {

		enum class TaskEnum {
			Move,
		};

		//--------------------------------------------------------------------------------------
		///	�^�[�Q�b�g�̋߂��܂�Astar�𗘗p���Ĉړ�����^�X�N�p�����[�^
		//--------------------------------------------------------------------------------------
		struct MoveAstar_Parametor {
			std::shared_ptr<Task_MovePositions_Parametor> movePositionsParam;

			MoveAstar_Parametor();
		};

		//--------------------------------------------------------------------------------------
		///	�^�[�Q�b�g�̋߂��܂�Astar�𗘗p���Ĉړ�����^�X�N
		//--------------------------------------------------------------------------------------
		class MoveAstar : public basecross::TaskNodeBase<Enemy::EnemyBase>
		{
		public:
			using Parametor = MoveAstar_Parametor;

		private:
			const Parametor* m_param;						//�p�����[�^

			std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//�^�X�N���X�g

			std::queue<int> m_areaRoute;					//�m�F����G���A���[�g

			std::weak_ptr<Transform> m_transform;			
			std::weak_ptr<TargetManager> m_targetManager;	
			std::weak_ptr<VelocityManager> m_velocityManager;
			std::weak_ptr<SelfAstarNodeController> m_selfAstarNodeController;

			bool m_isInitializeSearch;	//����T�[�`����
			bool m_isSearchRoute;		//���[�g�����������ǂ���
			//static std::mutex m_mtx;	//�~���[�e�b�N�X

		public:
			MoveAstar(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr);

			virtual ~MoveAstar() = default;

			void OnStart() override;

			bool OnUpdate() override;

			void OnExit() override;

		private:

			void DefineTask();

			void SelectTask();

			void NextRoute();

			/// <summary>
			/// �G���A�Ԃ̃��[�g���v�Z����B
			/// </summary>
			/// <returns>�G���A�Ԃ̃��[�g</returns>
			std::queue<int> CalculateMoveAreaRouteQueue();

			/// <summary>
			/// �p�j����ꏊ�̔z����擾
			/// </summary>
			/// <returns>�p�j����ꏊ�̔z��</returns>
			std::vector<Vec3> CalculateMovePositions();

			/// <summary>
			/// �p�j�ړ���̖ړI�n���擾
			/// </summary>
			/// <returns>�p�j�ړ�����擾</returns>
			Vec3 CalculateMoveTargetPosition();

			void SetIsSearchRoute(const bool isSearchRoute) noexcept { m_isSearchRoute = isSearchRoute; }

			bool IsSearchRoute() const noexcept { return m_isSearchRoute; }

		public:

			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			bool IsEnd() const;
		};


	}
}