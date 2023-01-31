
/*!
@file ImpactMap.cpp
@brief ImpactMap�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Astar/AstarNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge_Ex.h"

#include "Maruyama/Enemy/Astar/NavGraphNode.h"
#include "Maruyama/StateMachine/NodeBase.h"
#include "Maruyama/StateMachine/EdgeBase.h"
#include "Maruyama/Patch/Graphs/SparseGraph_Ex.h"

#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"
#include "Factory_WayPointMap_FloodFill.h"

#include "Maruyama/Thread/ThreadPool.h"

#include "Maruyama/Utility/UtilityObstacle.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	namespace maru {

		using DirectionType = Factory_WayPointMap_FloodFill::DirectionType;
		using OpenData = Factory_WayPointMap_FloodFill::OpenData;

		/*
		//�����z��
		const Vec3 DIRECTIONS[] = {
			Vec3::Right(),										//�E
			(Vec3::Right() + Vec3::Forward()).GetNormalized(),	//�E��
			(Vec3::Right() - Vec3::Forward()).GetNormalized(),	//�E��O

			-Vec3::Right(),										//��
			(-Vec3::Right() + Vec3::Forward()).GetNormalized(),	//����
			(-Vec3::Right() - Vec3::Forward()).GetNormalized(),	//����O

			Vec3::Forward(),									//��
			-Vec3::Forward()									//��O
		};
		*/

		//�����}�b�v
		const std::unordered_map<DirectionType, Vec3> DIRECTION_MAP = {
			{ DirectionType::Right,			Vec3::Right() },										//�E
			//{ DirectionType::RightForward,	(Vec3::Right() + Vec3::Forward()).GetNormalized()},		//�E��
			//{ DirectionType::RightBack,		(Vec3::Right() - Vec3::Forward()).GetNormalized()},		//�E��O
			{ DirectionType::RightForward,	(Vec3::Right() + Vec3::Forward())},		//�E��
			{ DirectionType::RightBack,		(Vec3::Right() - Vec3::Forward())},		//�E��O

			{ DirectionType::Left,			-Vec3::Right()},										//��
			//{ DirectionType::LeftForward,	(-Vec3::Right() + Vec3::Forward()).GetNormalized() },	//����
			//{ DirectionType::LeftBack,		(-Vec3::Right() - Vec3::Forward()).GetNormalized() },	//����O
			{ DirectionType::LeftForward,	(-Vec3::Right() + Vec3::Forward()) },	//����
			{ DirectionType::LeftBack,		(-Vec3::Right() - Vec3::Forward()) },	//����O

			{ DirectionType::Foward,		Vec3::Forward()},										//����
			{ DirectionType::Back,			-Vec3::Forward()},										//����O
		};

		//--------------------------------------------------------------------------------------
		///	Factory_�e���}�b�v_�t���b�h�t�B���A���S���Y���̐����p���f�[�^
		//--------------------------------------------------------------------------------------

		Factory_WayPointMap_FloodFill::OpenData::OpenData(
			const std::shared_ptr<AstarNode>& parentNode,
			const std::shared_ptr<AstarNode>& selfNode
		):
			parentNode(parentNode),
			selfNode(selfNode),
			isActive(true)
		{}

		//--------------------------------------------------------------------------------------
		///	Factory_�e���}�b�v_�t���b�h�t�B���A���S���Y���{��
		//--------------------------------------------------------------------------------------

		Factory_WayPointMap_FloodFill::Factory_WayPointMap_FloodFill(const std::shared_ptr<Stage>& stage) :
			m_stage(stage)
		{
			/*
			auto startPosition = Vec3(0.0f);

			for (const auto& direction : DIRECTIONS) {
				auto targetPosition = startPosition + (direction * 2.0f);

				auto object = stage->Instantiate<GameObject>(targetPosition, Quat::Identity());
				object->AddComponent<PNTStaticDraw>()->SetMeshResource(L"DEFAULT_CUBE");
			}
			*/
		}

		std::unordered_map<DirectionType, int> Factory_WayPointMap_FloodFill::SettingIndexByDirection(const Parametor& parametor) {
			std::unordered_map<DirectionType, int> result;

			const maru::Rect& rect = parametor.rect;
			const float& intervalRange = parametor.intervalRange;

			//��ƂȂ鉡�̑傫���ƁA�c�̑傫��
			int widthCount = static_cast<int>(rect.width / intervalRange);
			int plusIndex = widthCount + 1;	//���̒����������ŉ��s�����̃C���f�b�N�X�ɂȂ�B

			result[DirectionType::Right] = +1;
			result[DirectionType::RightForward] = 1 + plusIndex;
			result[DirectionType::RightBack] = 1 - plusIndex;
			result[DirectionType::Left] = -1;
			result[DirectionType::LeftForward] = -1 + plusIndex;
			result[DirectionType::LeftBack] = -1 - plusIndex;
			result[DirectionType::Foward] = +plusIndex;
			result[DirectionType::Back] = -plusIndex;

			return result;
		}

		bool Factory_WayPointMap_FloodFill::IsNodeCreate(
			const std::shared_ptr<OpenData>& newOpenData,
			const std::shared_ptr<GraphType>& graph,
			const Parametor& parametor,
			bool& isRayHit
		) {
			std::lock_guard<std::mutex> lock(m_mutex);	//�~���[�e�b�N�X���b�N

			auto startPosition = newOpenData->parentNode.lock()->GetPosition();
			auto targetPosition = newOpenData->selfNode->GetPosition();

			//�^�[�Q�b�g���G���A���O���ɂ���Ȃ�
			int testIndex = newOpenData->selfNode->GetIndex();
			auto selfPosition = newOpenData->selfNode->GetPosition();
			if (!parametor.rect.IsInRect(newOpenData->selfNode->GetPosition())) {
				return false;
			}

			//��Q���ɓ������Ă�����(��ɏ�Q����������Ȃ��ƁA�G�b�W�Ƌ��L���Ă��邽�߃o�O��(�C��������))
			auto obstacleObjects = GetStage()->GetGameObjectVec();	//��Q���z��
			if (isRayHit = maru::UtilityObstacle::IsRayObstacle(startPosition, targetPosition, obstacleObjects)) {
				return false;	//�����ł��Ȃ�
			}

			//�����m�[�h�����݂���Ȃ�
			if (graph->IsSomeIndexNode(newOpenData->selfNode->GetIndex())) {
				return false;
			}

			return true;	//�ǂ̏����ɂ����Ă͂܂�Ȃ��Ȃ�true
		}


		bool Factory_WayPointMap_FloodFill::IsEdgeCreate(
			const std::shared_ptr<OpenData>& newOpenData,
			const std::shared_ptr<GraphType>& graph,
			const Parametor& parametor,
			const bool isRayHit
		) {
			std::lock_guard<std::mutex> lock(m_mutex);	//�~���[�e�b�N�X���b�N

			//��Q���ɓ������Ă���Ȃ�A�������Ȃ�
			if (isRayHit) {
				return false;
			}

			//�^�[�Q�b�g���G���A���O���ɂ���Ȃ�
			int testIndex = newOpenData->selfNode->GetIndex();
			auto selfPosition = newOpenData->selfNode->GetPosition();
			if (!parametor.rect.IsInRect(newOpenData->selfNode->GetPosition())) {
				return false;
			}

			//�����G�b�W�����݂���Ȃ�
			const auto parentNode = newOpenData->parentNode.lock();
			const auto& selfNode = newOpenData->selfNode;
			if (graph->IsSomeIndexEdge(parentNode->GetIndex(), selfNode->GetIndex())) {
				return false;	//�������Ȃ�
			}

			return true;	//�������ʂ������߁A��������B
		}

		void Factory_WayPointMap_FloodFill::CreateWayPoints(
			const std::shared_ptr<OpenData>& parentOpenData,
			const std::shared_ptr<GraphType>& graph,
			const Parametor& parametor
		) {
			auto openDatas = CreateChildrenOpenDatas(parentOpenData, parametor);	//�I�[�v���f�[�^�̐���

			//���[�v���āA�I�[�v���f�[�^�̒����琶���ł�����̂�ݒ�
			for (const auto& openData : openDatas) {
				auto parentNode = openData->parentNode.lock();
				auto selfNode = openData->selfNode;
				bool isRayHit = false;	//��Q���ɓ����������ǂ������L�^����B

				//�m�[�h�������ł���Ȃ�
				if (IsNodeCreate(openData, graph, parametor, isRayHit)) {
					std::lock_guard<std::mutex> lock(m_mutex);		//�~���[�e�b�N�X���b�N
					auto node = graph->AddNode(openData->selfNode);	//�O���t�Ƀm�[�h�ǉ�
					m_openDataQueue.push(openData);					//�����L���[��OpenData��ǉ�
				}

				//�G�b�W�̐���������������Ă���Ȃ�
				if (IsEdgeCreate(openData, graph, parametor, isRayHit)) {
					std::lock_guard<std::mutex> lock(m_mutex);	//�~���[�e�b�N�X���b�N
					auto edge = graph->AddEdge(parentNode->GetIndex(), selfNode->GetIndex());	//�O���t�ɃG�b�W�ǉ�
				}
			}
		}

		int Factory_WayPointMap_FloodFill::CalculateIndex(
			const std::shared_ptr<OpenData>& parentOpenData,
			const DirectionType directionType
		) const {
			const auto& parent = parentOpenData->selfNode;
			int index = parent->GetIndex() + m_plusIndexMapByDirection.at(directionType);	//�C���f�b�N�X�̌v�Z
			return index;
		}

		std::vector<std::shared_ptr<OpenData>> Factory_WayPointMap_FloodFill::CreateChildrenOpenDatas(
			const std::shared_ptr<OpenData>& parentOpenData,
			const Parametor& parametor
		) {
			std::vector<std::shared_ptr<OpenData>> result;
			const auto& parent = parentOpenData->selfNode;	//�e�m�[�h���擾

			for (const auto& pair : DIRECTION_MAP) {
				const DirectionType& directionType = pair.first;	//�����^�C�v
				const Vec3& direction = pair.second;				//�����x�N�g��

				int index = CalculateIndex(parentOpenData, directionType);			//�C���f�b�N�X�̌v�Z
				if (index < 0) {	//�C���f�b�N�X��0��菬�����Ȃ珈�����΂��B
					continue;
				}

				Vec3 startPosition = parent->GetPosition();			//�J�n�ʒu
				Vec3 targetPosition = startPosition + (direction * parametor.intervalRange);	//�����ʒu

				auto newNode = std::make_shared<AstarNode>(index, targetPosition);	//�V�K�m�[�h�̍쐬

				auto newOpenData = std::make_shared<OpenData>(parent, newNode);		//�V�K�f�[�^�쐬
				result.push_back(newOpenData);
			}

			return result;
		}

		void Factory_WayPointMap_FloodFill::AddWayPointMap(
			const std::shared_ptr<GraphType>& graph,
			const Parametor& parametor
		) {
			m_plusIndexMapByDirection = SettingIndexByDirection(parametor);	//�����ʂ̉��Z����C���f�b�N�X�����Z�b�e�B���O

			auto baseStartPosition = parametor.rect.CalculateStartPosition();
			baseStartPosition.y = parametor.createHeight;	//�����̐ݒ�

			maru::Utility::QueueClear(m_openDataQueue);
			auto newNode = std::make_shared<AstarNode>(0, baseStartPosition);
			//Debug::GetInstance()->Log(newNode->GetPosition());
			graph->AddNode(newNode);
			m_openDataQueue.push(std::make_shared<OpenData>(nullptr, newNode));

			while (!m_openDataQueue.empty()) {	//�L���[����ɂȂ�܂�
				auto parentData = m_openDataQueue.front();
				m_openDataQueue.pop();
				CreateWayPoints(parentData, graph, parametor);
			}

		}





		//�t�Ɏg��Ȃ��Ȃ�������g��Ȃ�
		void Factory_WayPointMap_FloodFill::MulchiThreadCretae() {
			//while (!m_openDataQueue.empty()) {	//�L���[����ɂȂ�܂�

			//	auto threadPool = new ThreadPool();	//�X���b�h�v�[���̐���

			//	while (!m_openDataQueue.empty()) {	//�L���[����ɂȂ�܂�
			//		auto parentData = m_openDataQueue.front();
			//		m_openDataQueue.pop();
			//		//CreateWayPoints(parentData, graph, parametor);

			//		threadPool->Submit(
			//			&Factory_WayPointMap_FloodFill::CreateWayPoints,
			//			this,
			//			parentData,
			//			graph,
			//			parametor
			//		);
			//	}

			//	delete(threadPool);
			//}
		}

	}
}