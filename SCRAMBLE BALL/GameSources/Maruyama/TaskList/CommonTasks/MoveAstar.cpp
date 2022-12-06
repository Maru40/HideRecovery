/*!
@file MoveAstar.cpp
@brief MoveAstar�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "MoveAstar.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/TaskList/CommonTasks/Task_MovePositions.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Enemy/ImpactMap/FieldImpactMap.h"
#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"
#include "Maruyama/Enemy/Astar/GraphAstar.h"
#include "Maruyama/Enemy/Astar/NavGraphNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"

#include "Maruyama/Utility/Component/TargetManager.h"
#include "VelocityManager.h"

#include "Maruyama/Item/HideItem.h"
#include "Maruyama/StageObject/HidePlace.h"

#include "Maruyama/Enemy/Component/SelfAstarNodeController.h"
#include "Maruyama/Utility/Component/Targeted.h"

#include "Maruyama/Enemy/ImpactMap/SelfImpactNodeManager.h"
#include "Maruyama/Enemy/Astar/OpenDataHandler.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	namespace Task {

		//--------------------------------------------------------------------------------------
		///	thread���Ǘ�����f�[�^
		//--------------------------------------------------------------------------------------

		MoveAstar_ThreadData::MoveAstar_ThreadData(std::thread& newThread) :
			m_isRunning(true)
		{
			newThread.swap(m_thread);

			std::thread observeRunning([&]() {
				m_thread.join();
				m_isRunning = false;
				Debug::GetInstance()->Log(L"�����I��");
			});

			observeRunning.detach();
		}

		//--------------------------------------------------------------------------------------
		///	�^�[�Q�b�g�̋߂��܂�Astar�𗘗p���Ĉړ�����^�X�N�p�����[�^
		//--------------------------------------------------------------------------------------

		MoveAstar_Parametor::MoveAstar_Parametor() :
			movePositionsParam(new Task_MovePositions_Parametor())
		{}

		//--------------------------------------------------------------------------------------
		///	�^�[�Q�b�g�̋߂��܂�Astar�𗘗p���Ĉړ�����^�X�N
		//--------------------------------------------------------------------------------------


		MoveAstar::MoveAstar(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr) :
			TaskNodeBase(owner),
			m_param(paramPtr),
			m_taskList(new TaskList<TaskEnum>()),
			m_areaOpenDataHandler(new OpenDataHandler()),
			m_openDataHandler(new OpenDataHandler()),
			m_isInitializeSearch(true),
			m_isSearchRoute(false),
			m_threadData(nullptr)
		{
			DefineTask();

			m_transform = GetOwner()->GetGameObject()->GetComponent<Transform>();
			m_targetManager = GetOwner()->GetGameObject()->GetComponent<TargetManager>();
			m_velocityManager = GetOwner()->GetGameObject()->GetComponent<VelocityManager>();
			m_selfAstarNodeController = GetOwner()->GetGameObject()->GetComponent<SelfAstarNodeController>(false);
		}

		void MoveAstar::OnStart() {
			//Debug::GetInstance()->ClearLog();
			CalculateMoveAreaRouteQueue();	//�p�j�G���A���[�g�̎擾

			if (m_isInitializeSearch) {
				//���񌟍��̂݃o�O�邽�߁A�������܂ł̉�����
				m_isInitializeSearch = false;
				NextRoute();
			}
			else {
				//NextRoute();
				m_taskList->ForceStop();	//�^�X�N�̋����I��
				StartThread_NextRoute();	//�����X���b�h�̊J�n
			}
		}

		bool MoveAstar::OnUpdate() {
			//���[�g�������͑��̏������~�߂�B
			//if (IsSearchRoute()) {
			//	return false;
			//}

			//�X���b�h���������Ȃ珈�������Ȃ��B
			if (m_threadData && m_threadData->m_isRunning) {
				return false;
			}

			m_taskList->UpdateTask();

			if (m_taskList->IsEnd()) {
				StartThread_NextRoute();	//�����X���b�h�̊J�n
				//NextRoute();
			}

			return IsEnd();
		}

		void MoveAstar::OnExit() {
			m_taskList->ForceStop();
		}

		void MoveAstar::DefineTask() {
			auto ownerObject = GetOwner()->GetGameObject();

			m_taskList->DefineTask(TaskEnum::Move, std::make_shared<Task_MovePositions>(ownerObject, m_param->movePositionsParam));
		}

		void MoveAstar::SelectTask() {
			//id���Ⴄ�Ȃ�return
			if (m_currentThreadID != std::this_thread::get_id()) {
				return;
			}

			std::lock_guard<mutex> lock(m_mtx);	//���b�N

			TaskEnum tasks[] = {
				TaskEnum::Move,
			};

			for (const auto& task : tasks) {
				m_taskList->AddTask(task);
			}
		}

		void MoveAstar::StartThread_NextRoute() {
			SetIsSearchRoute(true);
			//�X���b�h����
			//m_threadData = ThreadData(nullptr);
			//m_threadData = std::make_shared<ThreadData>([&]() { NextRoute(); });
			std::thread nextRoute(&MoveAstar::NextRoute, this);
			m_currentThreadID = nextRoute.get_id();	//ID�̎擾

			m_threadData = std::make_shared<ThreadData>(nextRoute);
			//nextRoute.detach();
		}

		void MoveAstar::NextRoute() {
			if (m_areaRoute.empty()) {
				std::lock_guard<mutex> lock(m_mtx);	//���b�N
				SetIsSearchRoute(false);//�����I��
				return;
			}

			auto positions = CalculateMovePositions();	//�V�����|�W�V�����ɕύX
	
			//id���Ⴄ�Ȃ�return
			if (m_currentThreadID != std::this_thread::get_id()) {
				return;
			}

			//�|�W�V�����̕ύX(�r���K�{)
			m_param->movePositionsParam->positions = positions;

			//�^�X�N�̍Ďn��(�r���K�{)
			SelectTask();	

			//id���Ⴄ�Ȃ�return
			if (m_currentThreadID != std::this_thread::get_id()) {
				return;
			}

			std::lock_guard<mutex> lock(m_mtx);	//���b�N

			//�����I��(�r���K�{)
			SetIsSearchRoute(false);	
		}

		std::queue<int> MoveAstar::CalculateMoveAreaRouteQueue() {
			maru::Utility::QueueClear(m_areaRoute);	//�L���[�̃N���A

			auto fieldImpactMap = maru::FieldImpactMap::GetInstance();	//�t�B�[���h�e���}�b�v�̎擾
			auto astar = fieldImpactMap->GetImpactMap()->GetGraphAstar();

			//�J�n�ʒu�ƖڕW�ʒu�̎擾
			auto startPosition = m_transform.lock()->GetPosition();
			auto targetPosition = CalculateMoveTargetPosition();

			//�G���A�̃��[�g���擾
			auto areaRouteIndices = SearchAreaIndices(startPosition, targetPosition);

			std::wstring debugStr = L"";	//�f�o�b�O
			//Astar�������ŏ��̎������g�̃m�[�h���Ȃ����߁A�ŏ��͌��ݏ������Ă���G���A���猟������B
			int startAreaIndex = astar->SearchNearAreaIndex(startPosition);
			debugStr += std::to_wstring(startAreaIndex);
			m_areaRoute.push(startAreaIndex);
			for (const auto& areaRouteIndex : areaRouteIndices) {
				debugStr += L"," + std::to_wstring(areaRouteIndex);
				m_areaRoute.push(areaRouteIndex);
			}

			//Debug::GetInstance()->Log(L"�G���A���[�g---------------------------");
			////std::reverse(debugStr.begin(), debugStr.end());
			//Debug::GetInstance()->Log(debugStr);

			return m_areaRoute;
		}

		std::vector<Vec3> MoveAstar::CalculateMovePositions() {
			std::lock_guard<mutex> lock(m_mtx);	//���b�N

			if (m_areaRoute.empty()) {
				return std::vector<Vec3>();
			}

			//�������g������G���A�C���f�b�N�X
			int areaIndex = m_areaRoute.front();	
			m_areaRoute.pop();	//(�r���K�{)
			int targetAreaIndex = !m_areaRoute.empty() ? m_areaRoute.front() : areaIndex;
			
			if (m_currentThreadID != std::this_thread::get_id()) {
				return {};
			}

			auto startNode = m_selfAstarNodeController.lock()->CalculateNode();
			
			auto positions = CalculateRoutePositions(startNode, CalculateMoveTargetNode(), areaIndex, targetAreaIndex);

			return positions;
		}

		Vec3 MoveAstar::CalculateMoveTargetPosition() {
			auto targetManager = m_targetManager.lock();
			//�K�v�R���|�[�l���g�����݂��Ȃ��Ȃ�
			if (!targetManager || !targetManager->HasTarget()) {
				//Debug::GetInstance()->Log(L"MoveAstar::CalculateMoveTargetPosition() �ŕK�v�R���|�[�l���g�����݂��܂���B");
				return Vec3(0.0f);
			}

			return targetManager->GetTargetPosition();	//��{�^�[�Q�b�g�Ǘ�����̎擾�Ō����ł���悤�ɂ���B
		}

		std::shared_ptr<NavGraphNode> MoveAstar::CalculateMoveTargetNode() {
			auto targetManager = m_targetManager.lock();
			//�K�v�R���|�[�l���g�����݂��Ȃ��Ȃ�
			if (!targetManager || !targetManager->HasTarget()) {
				return nullptr;
			}

			auto target = targetManager->GetTarget();
			auto selfAstarNodeController = targetManager->GetTarget()->GetComponent<SelfAstarNodeController>(false);
			if (!selfAstarNodeController) {
				return nullptr;
			}

			auto node = selfAstarNodeController->CalculateNode();
			if (!node) {
				Debug::GetInstance()->Log(target->GetComponent<Transform>(false)->GetPosition());
				Debug::GetInstance()->Log(L"TaregetNode��nullptr�ł��B");
			}

			//Debug::GetInstance()->Log(targetManager->GetTargetPosition());
			return node;
		}

		std::vector<int> MoveAstar::SearchAreaIndices(const Vec3& startPosition, const Vec3& targetPosition) {
			auto fieldImpactMap = maru::FieldImpactMap::GetInstance();
			auto impactMap = fieldImpactMap->GetImpactMap();
			auto astar = impactMap->GetGraphAstar();
			
			auto areaGraph = astar->GetAreaGraph();

			auto selfNode = astar->SearchNearAreaNode(startPosition);
			auto targetNode = astar->SearchNearAreaNode(targetPosition);

			auto route = SearchAstarStart(selfNode, targetNode, areaGraph);

			std::vector<int> resultIndices;
			while (!route.empty()) {
				auto top = route.top();
				if (top.expired()) {
					route.pop();
					continue;
				}

				resultIndices.push_back(top.lock()->GetAreaIndex());

				route.pop();
			}

			return resultIndices;
		}

		std::vector<Vec3> MoveAstar::CalculateRoutePositions(
			const std::shared_ptr<NavGraphNode>& selfNode,
			const std::shared_ptr<NavGraphNode>& targetNode,
			const int areaIndex,
			const int targetAreaIndex
		) {
			auto fieldImpactMap = maru::FieldImpactMap::GetInstance();
			auto impactMap = fieldImpactMap->GetImpactMap();
			auto astar = impactMap->GetGraphAstar();

			auto graph = astar->GetBaseGraph();

			auto route = SearchAstarStart(selfNode, targetNode, graph, targetAreaIndex);

			std::vector<Vec3> resultPositions;

			while (!route.empty()) {
				auto top = route.top();
				if (top.expired()) {
					route.pop();
					continue;
				}

				resultPositions.push_back(top.lock()->GetPosition());

				route.pop();
			}

			return resultPositions;
		}

		std::stack<std::weak_ptr<NavGraphNode>> MoveAstar::SearchAstarStart(
			const std::shared_ptr<NavGraphNode>& selfNode,
			const std::shared_ptr<NavGraphNode>& targetNode,
			const std::shared_ptr<AstarGraph>& graph,
			const int targetAreaIndex
		) {
			std::stack<std::weak_ptr<NavGraphNode>> result;

			if (selfNode == nullptr || targetNode == nullptr) {
				if (selfNode == nullptr) {
					Debug::GetInstance()->Log(L"MoveAstar::SearchAstarStart(), startNode��null�ł�");
				}

				if (targetNode == nullptr) {
					//Debug::GetInstance()->Log(L"MoveAstar::SearchAstarStart(), targetNode��null�ł�");
				}

				return result;
			}

			if (selfNode->GetPosition() == targetNode->GetPosition()) {	//�����m�[�h�Ȃ�true
				//Debug::GetInstance()->Log(L"MoveAstar::SearchAstarStart(), �������[�g�ł��B");
				result.push(selfNode);
				return result;
			}

			auto openDataHandler = OpenDataHandler();
			openDataHandler.StartSearchAstar(selfNode, targetNode, graph, targetAreaIndex);	//OpenData���g���čŒZ�o�H����������B

			return openDataHandler.GetRoute();		//���[�g�̎擾
		}

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		bool MoveAstar::IsEnd() const {
			auto targetManager = m_targetManager.lock();
			if (!targetManager || !targetManager->HasTarget()) {
				return true;
			}

			return m_areaRoute.empty() && m_taskList->IsEnd();
		}

	}
}