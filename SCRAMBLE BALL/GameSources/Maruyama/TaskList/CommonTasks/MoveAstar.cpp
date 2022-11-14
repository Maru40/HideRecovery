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

#include "Maruyama/Utility/Component/TargetManager.h"
#include "VelocityManager.h"

#include "Maruyama/Item/HideItem.h"
#include "Maruyama/StageObject/HidePlace.h"

#include "Maruyama/Enemy/Component/SelfAstarNodeController.h"
#include "Maruyama/Utility/Component/Targeted.h"

#include "Maruyama/Enemy/ImpactMap/SelfImpactNodeManager.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	namespace Task {

		//std::mutex MoveAstar::m_mtx;

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
			m_isInitializeSearch(true),
			m_isSearchRoute(false)
		{
			DefineTask();

			m_transform = GetOwner()->GetGameObject()->GetComponent<Transform>();
			m_targetManager = GetOwner()->GetGameObject()->GetComponent<TargetManager>();
			m_velocityManager = GetOwner()->GetGameObject()->GetComponent<VelocityManager>();
			m_selfAstarNodeController = GetOwner()->GetGameObject()->GetComponent<SelfAstarNodeController>(false);
		}

		void MoveAstar::OnStart() {
			CalculateMoveAreaRouteQueue();	//�p�j�G���A���[�g�̎擾

			if (m_isInitializeSearch) {
				//���񌟍��̂݃o�O�邽�߁A�������܂ł̉�����
				m_isInitializeSearch = false;
				NextRoute();
			}
			else {
				SetIsSearchRoute(true);
				//NextRoute();
				//�X���b�h����
				std::thread nextRoute([&]() { NextRoute(); });
				nextRoute.detach();
			}
		}

		bool MoveAstar::OnUpdate() {
			//���[�g�������͑��̏������~�߂�B
			if (IsSearchRoute()) {
				return false;
			}

			m_taskList->UpdateTask();

			if (m_taskList->IsEnd()) {
				SetIsSearchRoute(true);
				//NextRoute();
				//�X���b�h����
				std::thread nextRoute([&]() { NextRoute(); });
				nextRoute.detach();
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
			TaskEnum tasks[] = {
				TaskEnum::Move,
			};

			for (const auto& task : tasks) {
				m_taskList->AddTask(task);
			}
		}

		void MoveAstar::NextRoute() {
			//SetIsSearchRoute(true);		//�����J�n
			//std::lock_guard<std::mutex> lock(m_mtx);

			if (m_areaRoute.empty()) {
				SetIsSearchRoute(false);//�����I��
				return;
			}

			//���������J�n
			//m_velocityManager.lock()->StartDeseleration();

			auto positions = CalculateMovePositions();	//�V�����|�W�V�����ɕύX

			SelectTask();	//�^�X�N�̍Ďn��

			//���������I��
			//m_velocityManager.lock()->SetIsDeseleration(false);
			SetIsSearchRoute(false);	//�����I��
		}

		std::queue<int> MoveAstar::CalculateMoveAreaRouteQueue() {
			maru::Utility::QueueClear(m_areaRoute);	//�L���[�̃N���A

			auto fieldImpactMap = maru::FieldImpactMap::GetInstance();	//�t�B�[���h�e���}�b�v�̎擾

			//�J�n�ʒu�ƖڕW�ʒu�̎擾
			auto startPosition = m_transform.lock()->GetPosition();
			auto targetPosition = CalculateMoveTargetPosition();

			//�G���A�̃��[�g���擾
			auto areaRouteIndices = fieldImpactMap->SearchAreaRouteIndices(startPosition, targetPosition);

			//Astar�������ŏ��̎������g�̃m�[�h���Ȃ����߁A�ŏ��͌��ݏ������Ă���G���A���猟������B
			m_areaRoute.push(fieldImpactMap->SearchNearAreaIndex(startPosition));
			for (const auto& areaRouteIndex : areaRouteIndices) {
				m_areaRoute.push(areaRouteIndex);
			}

			return m_areaRoute;
		}

		std::vector<Vec3> MoveAstar::CalculateMovePositions() {
			if (m_areaRoute.empty()) {
				return std::vector<Vec3>();
			}

			auto startPosition = m_transform.lock()->GetPosition();
			auto endPosition = CalculateMoveTargetPosition();

			int areaIndex = m_areaRoute.front();	//�������g������G���A�C���f�b�N�X
			m_areaRoute.pop();
			int targetAreaIndex = !m_areaRoute.empty() ? m_areaRoute.front() : areaIndex;
			auto startNode = m_selfAstarNodeController.lock()->CalculateNode();
			//auto targetNode = 
			auto positions = maru::FieldImpactMap::GetInstance()->GetRoutePositions(startNode, CalculateMoveTargetNode(), areaIndex, targetAreaIndex);

			m_param->movePositionsParam->positions = positions;
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

			auto selfAstarNodeController = targetManager->GetTarget()->GetComponent<SelfAstarNodeController>(false);
			if (!selfAstarNodeController) {
				return nullptr;
			}

			auto node = selfAstarNodeController->CalculateNode();
			if (!node) {
				Debug::GetInstance()->Log(L"TaregetNode��nullptr�ł��B");
			}
			
			return node;
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