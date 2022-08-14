
/*!
@file FieldImpactMap.cpp
@brief FieldImpactMap�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "SingletonComponent.h"
#include "FieldImpactMap.h"

#include "StageMapCSV.h"
#include "ImpactMap.h"

#include "PlayerInputer.h"

#include "TimeHelper.h"
#include "GameTimer.h"

#include "NavGraphNode.h"

#include "MaruAction.h"
#include "MaruUtility.h"

namespace basecross {

	namespace maru {

		std::weak_ptr<FieldImpactMap> SingletonComponent<FieldImpactMap>::sm_instance;

		//--------------------------------------------------------------------------------------
		///	�t�B�[���h�e���}�b�v�̃p�����[�^
		//--------------------------------------------------------------------------------------

		FieldImpactMap_Parametor::FieldImpactMap_Parametor():
			intervalRange(7.0f),
			createHeightOffset(1.0f)
		{}

		//--------------------------------------------------------------------------------------
		///	��L�l�X�V�f�[�^
		//--------------------------------------------------------------------------------------

		OccupancyUpdateData::OccupancyUpdateData():
			OccupancyUpdateData(0.0f, nullptr)
		{}

		OccupancyUpdateData::OccupancyUpdateData(const float time, const std::shared_ptr<NavGraphNode>& node):
			OccupancyUpdateData(GameTimer(time), node)
		{}

		OccupancyUpdateData::OccupancyUpdateData(const GameTimer& timer, const std::shared_ptr<NavGraphNode>& node):
			timer(new GameTimer(timer)),
			node(node)
		{}

		bool OccupancyUpdateData::operator== (const OccupancyUpdateData& data) {
			return this == &data;
		}

		//--------------------------------------------------------------------------------------
		///	�t�B�[���h�e���}�b�v�{��
		//--------------------------------------------------------------------------------------

		FieldImpactMap::FieldImpactMap(
			const std::shared_ptr<GameObject>& objPtr, 
			const std::vector<std::shared_ptr<GameObject>>& floors
		):
			FieldImpactMap(objPtr, floors, Parametor())
		{}

		FieldImpactMap::FieldImpactMap(
			const std::shared_ptr<GameObject>& objPtr, 
			const std::vector<std::shared_ptr<GameObject>>& floors, 
			const Parametor& parametor
		):
			SingletonComponent(objPtr), m_floors(maru::Utility::ConvertArraySharedToWeak(floors)), m_param(parametor)
		{}

		void FieldImpactMap::OnLateStart() {
			m_impactMap = std::make_shared<ImpactMap>(GetStage()); //ImpactMap�̐���

			m_impactMap->ClearNodes();  //�m�[�h�̃N���A
			m_impactMap->ClearEdges();  //�G�b�W�̃N���A
			const float IntervalRange = m_param.intervalRange; //�e���}�b�v�̊e�m�[�h�̋���
			SettingImpactMap(m_floors, IntervalRange);

			m_impactMap->CreateDebugDraw(false); //�f�o�b�O�f�[�^�̐���
		}

		void FieldImpactMap::OnUpdate() {
			OccupancyUpdate();

			//�f�o�b�O
			if (m_impactMap) {
				m_impactMap->DebugInput([]() { return PlayerInputer::GetInstance()->IsYDown(); });
			}
		}

		void FieldImpactMap::OccupancyUpdate() {
			std::vector<OccupancyUpdateData*> removeDatas;
			for (auto& data : m_occupancyUpdateDatas) {
				constexpr float MaxOccupancyValue = 0.5f;
				data.timer->UpdateTimer();
				auto node = data.node.lock();
				if (!node) {
					removeDatas.push_back(&data);
					continue;
				}

				auto impactData = node->GetImpactData();
				auto timeRate = data.timer->GetTimeRate();
				impactData.occupancyValue = MaxOccupancyValue * timeRate;
				node->SetImpactData(impactData);

				if (data.timer->IsTimeUp()) {
					impactData.occupancyValue = MaxOccupancyValue;
					node->SetImpactData(impactData);
					removeDatas.push_back(&data);
				}

				//�f�o�b�O
				auto colorValue = 1.0f - impactData.occupancyValue;
				GetImpactMap()->ChangeTargetNodeColor(node->GetIndex(), Col4(1.0f, colorValue, colorValue, 1.0f));
			}
			
			//����Ȃ��f�[�^���폜
			for (int i = 0; i < removeDatas.size(); i++) {
				auto iter = m_occupancyUpdateDatas.begin();
				while (iter != m_occupancyUpdateDatas.end()) {
					if (&(*iter) == removeDatas[i]) {
						iter = m_occupancyUpdateDatas.erase(iter);
						break;
					}
					iter++;
				}
			}
		}

		std::vector<Vec3> FieldImpactMap::GetRoutePositons(const Vec3& selfPosition, const Vec3& targetPosition) {
			return m_impactMap->GetRoutePositons(selfPosition, targetPosition);
		}

		std::shared_ptr<ImpactMap> FieldImpactMap::GetImpactMap() const {
			return m_impactMap;
		}

		void FieldImpactMap::AddOccupancyUpdateData(const std::shared_ptr<NavGraphNode>& node) {
			//�����f�[�^�����݂���Ȃ�A���Ԃ̍X�V�̂�
			for (auto& data : m_occupancyUpdateDatas) {
				if (data.node.lock() == node) {
					auto impactData = node->GetImpactData();
					data.timer->ResetTimer(impactData.occupacyRecoverySpeed);
					return;
				}
			}

			auto impactData = node->GetImpactData();
			m_occupancyUpdateDatas.push_back(OccupancyUpdateData(GameTimer(impactData.occupacyRecoverySpeed), node));
		}

	}
}