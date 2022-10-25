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

#include "Maruyama/Item/HideItem.h"
#include "Maruyama/StageObject/HidePlace.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	namespace Task {

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
			m_taskList(new TaskList<TaskEnum>())
		{
			DefineTask();

			m_transform = GetOwner()->GetGameObject()->GetComponent<Transform>();
			m_targetManager = GetOwner()->GetGameObject()->GetComponent<TargetManager>();
		}

		void MoveAstar::OnStart() {
			SelectTask();	//�^�X�N�̑I��

			CalculateMoveAreaRouteQueue();	//�p�j�G���A���[�g�̎擾
			m_param->movePositionsParam->positions = CalculateMovePositions();	//�p�j�ړ����ݒ�
		}

		bool MoveAstar::OnUpdate() {
			m_taskList->UpdateTask();

			if (m_taskList->IsEnd()) {
				NextRoute();
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
			if (m_areaRoute.empty()) {
				return;
			}

			m_param->movePositionsParam->positions = CalculateMovePositions();	//�V�����|�W�V�����ɕύX

			SelectTask();				//�^�X�N�̍Ďn��
		}

		std::queue<int> MoveAstar::CalculateMoveAreaRouteQueue() {
			maru::Utility::QueueClear(m_areaRoute);	//�L���[�̃N���A

			auto fieldImpactMap = maru::FieldImpactMap::GetInstance();	//�t�B�[���h�e���}�b�v�̎擾

			//�J�n�ʒu�ƖڕW�ʒu�̎擾
			auto startPosition = m_transform.lock()->GetPosition();
			auto targetPosition = CalculateMoveTargetPosition();

			//�G���A�̃��[�g���擾
			auto areaRouteIndices = fieldImpactMap->SearchAreaRouteIndices(startPosition, targetPosition);

			Debug::GetInstance()->Log(L"AreaRoute-------------------");

			Debug::GetInstance()->Log(fieldImpactMap->SearchNearAreaIndex(startPosition));

			//Astar�������ŏ��̎������g�̃m�[�h���Ȃ����߁A�ŏ��͌��ݏ������Ă���G���A���猟������B
			m_areaRoute.push(fieldImpactMap->SearchNearAreaIndex(startPosition));
			for (const auto& areaRouteIndex : areaRouteIndices) {
				m_areaRoute.push(areaRouteIndex);
				Debug::GetInstance()->Log(areaRouteIndex);
			}
			Debug::GetInstance()->Log(L"AreaRoute-------------------");

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
			int targetAreaIndex = !m_areaRoute.empty() ? m_areaRoute.front() : -1;	//�ڕW�G���A���ʂł���Ȃ炻���ݒ�(-1�͖ڕW�m�[�h�������G���A)
			return maru::FieldImpactMap::GetInstance()->GetRoutePositions(startPosition, endPosition, areaIndex, targetAreaIndex);
		}

		Vec3 MoveAstar::CalculateMoveTargetPosition() {
			auto targetManager = m_targetManager.lock();
			//�K�v�R���|�[�l���g�����݂��Ȃ��Ȃ�
			if (!targetManager || !targetManager->HasTarget()) {
				Debug::GetInstance()->Log(L"MoveAstar::CalculateMoveTargetPosition() �ŕK�v�R���|�[�l���g�����݂��܂���B");

				//�f�o�b�O��Hide��Ώۂɂ���B
				auto hidePlace = maru::Utility::FindComponent<HidePlace>();
				auto position = hidePlace->GetGameObject()->GetComponent<Transform>()->GetPosition();
				return position;
				return Vec3(0.0f);
			}

			return targetManager->GetTargetPosition();	//��{�^�[�Q�b�g�Ǘ�����̎擾�Ō����ł���悤�ɂ���B
		}

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		bool MoveAstar::IsEnd() const {
			return m_areaRoute.empty() && m_taskList->IsEnd();
		}

	}
}