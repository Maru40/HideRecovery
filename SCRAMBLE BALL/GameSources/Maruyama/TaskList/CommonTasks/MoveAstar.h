/*!
@file MoveAstar.h
@brief MoveAstar�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/TaskList/TaskList.h"

//#include "Maruyama/Enemy/Astar/GraphAstar.h"
//#include "Maruyama/Enemy/Astar/AstarEdge.h"

namespace basecross {

	class TargetManager;
	class VelocityManager;
	class SelfAstarNodeController;
	class NavGraphNode;
	class OpenDataHandler;
	class AstarGraph;

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
		///	thread���Ǘ�����f�[�^
		//--------------------------------------------------------------------------------------
		struct MoveAstar_ThreadData
		{
			bool m_isRunning;
			std::thread m_thread;
			std::function<void()> startEvent;
			std::function<void()> exitEvent;

			MoveAstar_ThreadData(std::thread& newThread);

		private:
			void StartEvent() { if (startEvent) { startEvent(); } }

		public:
			/// <summary>
			/// �I���C�x���g(�����ŌĂԂƔr�����䂪�܂܂Ȃ�Ȃ��Ȃ邽�߂������Ă���B)
			/// </summary>
			void ExitEvent() { if (exitEvent) { exitEvent(); } }
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
			using ThreadData = MoveAstar_ThreadData;

		private:
			const Parametor* m_param;						//�p�����[�^

			std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//�^�X�N���X�g

			std::queue<int> m_areaRoute;					//�m�F����G���A���[�g

			std::weak_ptr<Transform> m_transform;			
			std::weak_ptr<TargetManager> m_targetManager;	
			std::weak_ptr<VelocityManager> m_velocityManager;
			std::weak_ptr<SelfAstarNodeController> m_selfAstarNodeController;

			std::shared_ptr<OpenDataHandler> m_areaOpenDataHandler;
			std::shared_ptr<OpenDataHandler> m_openDataHandler;

			bool m_isInitializeSearch;	//����T�[�`����
			bool m_isSearchRoute;		//���[�g�����������ǂ���
			//static std::mutex m_mtx;	//�~���[�e�b�N�X
			std::mutex m_mtx;

			std::thread::id m_currentThreadID;	//���ݓo�^���̃}���`�^�X�N

			std::shared_ptr<ThreadData> m_threadData;

		public:
			MoveAstar(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr);

			virtual ~MoveAstar() = default;

			void OnStart() override;

			bool OnUpdate() override;

			void OnExit() override;

		private:

			void DefineTask();

			void SelectTask();

			//���̏ꏊ�̌������J�n
			void StartThread_NextRoute();

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

			/// <summary>
			/// �p�j��̖ړI�m�[�h���擾
			/// </summary>
			std::shared_ptr<NavGraphNode> CalculateMoveTargetNode();

			//�G���AIndex�̎擾(�{����Astar�n�Ƃ��ĕʂŎ���)
			std::vector<int> SearchAreaIndices(const Vec3& startPosition, const Vec3& targetPosition);

			//���[�g�̎擾(�{����Astar�n�Ƃ��ĕʂŎ���)
			std::vector<Vec3> CalculateRoutePositions(
				const std::shared_ptr<NavGraphNode>& selfNode,
				const std::shared_ptr<NavGraphNode>& targetNode,
				const int areaIndex,
				const int targetAreaIndex
			);

			//Astar�����̃X�^�[�g(�{����Astar�n�Ƃ��ĕʂŎ���)
			std::stack<std::weak_ptr<NavGraphNode>> SearchAstarStart(
				const std::shared_ptr<NavGraphNode>& selfNode,
				const std::shared_ptr<NavGraphNode>& targetNode,
				const std::shared_ptr<AstarGraph>& graph,
				const int areaIndex = -1
			);

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