
/*!
@file ImpactMap.cpp
@brief ImpactMap�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Astar/NavGraphNode.h"
#include "Maruyama/Enemy/Astar/AstarNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"
#include "Maruyama/StateMachine/NodeBase.h"
#include "Maruyama/StateMachine/EdgeBase.h"
#include "Maruyama/Patch/Graphs/SparseGraph_Ex.h"

#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"
#include "Factory_WayPointMap_FloodFill.h"

#include "Maruyama/Utility/UtilityObstacle.h"

namespace basecross {

	namespace maru {

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

		bool Factory_WayPointMap_FloodFill::IsCreate(
			const Vec3& startPosition,
			const Vec3& targetPosition,
			const Parametor& parametor
		) {
			//��Q���ɓ������Ă�����
			auto obstacleObjects = GetStage()->GetGameObjectVec();	//��Q���z��
			if (maru::UtilityObstacle::IsRayObstacle(startPosition, targetPosition, obstacleObjects)) {
				return false;	//�����ł��Ȃ�
			}

			return true;	//�S�Ă̏����𓾂Ă��邽�߁A�����ł���B
		}

		std::vector<Vec3> Factory_WayPointMap_FloodFill::CalculationTargetPositions(
			const Vec3& startPosition, 
			const Parametor& parametor
		) {
			std::vector<Vec3> result;

			for (const auto& direction : DIRECTIONS) {
				//�����������ꏊ��ݒ�
				auto targetPosition = startPosition + (direction * parametor.intervalRange);

				//���̏ꏊ�ɐ����ł��邩���f
				if (IsCreate(startPosition, targetPosition, parametor)) {
					result.push_back(targetPosition);
				}
			}

			return result;
		}

		void Factory_WayPointMap_FloodFill::CreateWayPoints(
			const Vec3& startPosition,
			const std::shared_ptr<GraphType>& graph,
			const Parametor& parametor
		) {
			auto targetPositions = CalculationTargetPositions(startPosition, parametor);

			for (const auto& targetPosition : targetPositions) {
				//graph->AddNode(targetPosition);	//�E�F�C�|�C���g�̐���
				//graph->AddNode();
				//graph->AddNode(0, std::make_shared<NodeBase>());
				graph->AddNode<AstarNode>(targetPosition);
				//graph->AddEdge<AstarEdge>();
				
				//graph->AddEdge();
			}
		}

		void Factory_WayPointMap_FloodFill::AddWayPointMap(
			const Vec3& baseStartPosition,
			const std::shared_ptr<GraphType>& graph,
			const Parametor& parametor
		) {
			std::queue<Vec3> startPositionQueue;
			startPositionQueue.push(baseStartPosition);

			while (!startPositionQueue.empty()) {	//�L���[����ɂȂ�܂�
				auto startPosition = startPositionQueue.front();
				startPositionQueue.pop();
				CreateWayPoints(startPosition, graph ,parametor);
			}
		}

	}
}