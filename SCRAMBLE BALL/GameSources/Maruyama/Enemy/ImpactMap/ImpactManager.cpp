
/*!
@file ImpactManager.cpp
@brief ImpactManager�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "ImpactManager.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Enemy/ImpactMap/FieldImpactMap.h"
#include "ImpactMap.h"

#include "Maruyama/Enemy/Astar/GraphAstar.h"
#include "Maruyama/Enemy/Astar/SparseGraph.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Enemy/Astar/UtilityAstar.h"
#include "Maruyama/Utility/UtilityObstacle.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"

namespace basecross {

	namespace maru {

		//�m�[�h�̃A�N�e�B�u��ԍX�V
		void ImpactManager::NodeActiveUpdate(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter) {
			
		}
		
		//�G�b�W�̃A�N�e�B�u��ԍX�V
		void ImpactManager::EdgeActiveUpdate(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter) {
			//��Q��������ꍇ��Active��false�ɕύX
			const auto& astar = impactMap->GetGraphAstar();
			const auto& graph = astar->GetGraph();
			const auto numNode = graph->GetNumNodes();

			for (int i = 0; i < numNode; i++) {
				for (const auto& edge : graph->GetEdges(i)) {
					auto fromNode = graph->GetNode(edge->GetFrom());
					auto toNode = graph->GetNode(edge->GetTo());
					
					//��Q���ɑj�܂�Ă������A�N�e�B�u��Ԃɂ���B
					if (maru::UtilityObstacle::IsRayObstacle(fromNode->GetPosition(), toNode->GetPosition(), impactMap->GetStage()->GetGameObjectVec())) {
						edge->SetIsActive(false);
					}
				}
			}
		}

		//�����̎��͂ɉe���x���X�V����B
		void ImpactManager::CalculateAroundImpact(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter) {
			auto data = impacter->GetImpacterData();

			//�����̏�������m�[�h�̎擾
			auto selfNode = UtilityAstar::SearchNearNode(*impactMap->GetGraphAstar().get(), impacter->GetImpacterObject());

			//�����̎��͂���߂������قǉe���x���A�b�v������B
			
		}

		//���E�͈͂̉e���x���X�V����
		void ImpactManager::CalculateEyeImpact(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter) {
			//���E�͈͂̎擾
			auto impacterObject = impacter->GetImpacterObject();
			auto impacterTrans = impacterObject->GetComponent<Transform>();
			auto eye = impacter->GetEyeSearchRange();
			EyeSearchRangeParametor eyeParam = eye->GetParametor();

			//�����̏�������m�[�h�̎擾
			auto selfNode = UtilityAstar::SearchNearNode(*impactMap->GetGraphAstar().get(), impacter->GetImpacterObject());

			//���E���̃m�[�h���擾
			auto nodes = impactMap->GetEyeRangeNodes(impacterTrans->GetPosition(), impacter);

			//���E���̉e���x���X�V
			for (auto& node : nodes) {
				//��L�l�X�V
				auto impactData = node->GetImpactData();
				impactData.occupancyValue = 0.0f;
				node->SetImpactData(impactData);

				if (auto fieldImpact = maru::FieldImpactMap::GetInstance()) {
					fieldImpact->AddOccupancyUpdateData(node);
				}
			}
		}

		//�e���x�̌v�Z
		void ImpactManager::CalculateAllImpact(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter) {
			//�m�[�h�̃A�N�e�B�u��Ԃ̍X�V
			NodeActiveUpdate(impactMap, impacter);

			//�G�b�W�̃A�N�e�B�u��Ԃ̍X�V
			EdgeActiveUpdate(impactMap, impacter);

			auto impactType = impacter->GetImpacterData().type;
			switch (impactType)
			{
			case ImpacterType::Circle:
				CalculateAroundImpact(impactMap, impacter);
				break;

			case ImpacterType::Eye:
				CalculateAroundImpact(impactMap, impacter);
				CalculateEyeImpact(impactMap, impacter);
				break;
			}
		}

	}
}