
/*!
@file OccupancyManager.cpp
@brief OccupancyManager�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "OccupancyManager.h"

#include "Maruyama/Enemy/Astar/NavGraphNode.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"

namespace basecross {

	namespace maru {
		constexpr float UPDATE_DESIRED_VALUE = 0.5f;	//�X�V���̖ڕW�l

		//--------------------------------------------------------------------------------------
		///	��L�l�X�V�f�[�^
		//--------------------------------------------------------------------------------------

		OccupancyUpdateData::OccupancyUpdateData() :
			OccupancyUpdateData(0.0f, nullptr)
		{}

		OccupancyUpdateData::OccupancyUpdateData(const float intervalTime, const std::shared_ptr<NavGraphNode>& node) :
			OccupancyUpdateData(GameTimer(intervalTime), node)
		{}

		OccupancyUpdateData::OccupancyUpdateData(const GameTimer& timer, const std::shared_ptr<NavGraphNode>& node) :
			startValue(node->GetImpactData().occupancyValue),
			timer(new GameTimer(timer)),
			node(node)
		{}

		bool OccupancyUpdateData::operator== (const OccupancyUpdateData& data) {
			return this == &data;
		}

		//--------------------------------------------------------------------------------------
		///	�e���f�[�^�̐�L�l�Ǘ��̖{��
		//--------------------------------------------------------------------------------------

		OccupancyManager::OccupancyManager(const std::shared_ptr<maru::ImpactMap>& impactMap):
			m_impactMap(impactMap)
		{}

		void OccupancyManager::OnUpdate() {
			OccupancyValueUpdate();
		}

		void OccupancyManager::OccupancyValueUpdate() {
			std::vector<UpdateData*> removeDatas;	//�폜����f�[�^���܂Ƃ߂�z��B

			for (auto& data : m_updateDatas) {
				//�m�[�h��nullptr�Ȃ�A�폜�Ώۂɂ��Ĕ�΂��B
				auto node = data.node.lock();
				if (!node) {
					removeDatas.push_back(&data);
					continue;
				}

				data.timer->UpdateTimer();	//�^�C�}�[�X�V

				//�f�[�^�̗p��
				auto impactData = node->GetImpactData();
				auto timeRate = data.timer->GetTimeRate();

				//��L�l�̌v�Z
				auto endTime = data.timer->GetIntervalTime();
				auto nowTime = data.timer->GetElapsedTime();
				auto value = Lerp::CalculateLerp(data.startValue, UPDATE_DESIRED_VALUE, 0.0f, endTime, nowTime, Lerp::rate::Linear);

				//�f�[�^�̍X�V
				impactData.occupancyValue = value;
				node->SetImpactData(impactData);

				//���Ԃ��o�߂�����
				if (data.timer->IsTimeUp()) {
					impactData.occupancyValue = UPDATE_DESIRED_VALUE;	//�ڕW�l�ɐݒ�
					node->SetImpactData(impactData);
					removeDatas.push_back(&data);	//�z�񂩂�폜������ɒǉ�
				}

				//�f�o�b�O
				auto colorValue = 1.0f - impactData.occupancyValue;
				GetImpactMap()->ChangeTargetNodeColor(node->GetIndex(), Col4(1.0f, colorValue, colorValue, 1.0f));
			}

			//�f�[�^�̍폜
			RemoveUpdateDatas(removeDatas);
		}

		float OccupancyManager::CalculateOccupancyIntervalTime(const std::shared_ptr<NavGraphNode>& node) {
			auto impactData = node->GetImpactData();
			float subDesired = std::abs(UPDATE_DESIRED_VALUE - impactData.occupancyValue);	//�ڕW�l�Ƃ̍����l
			float rate = subDesired / UPDATE_DESIRED_VALUE;

			auto intervalTime = impactData.occupacyRecoverySpeed * rate;

			return intervalTime;
		}

		void OccupancyManager::RemoveUpdateDatas(const std::vector<UpdateData*>& removeDatas) {
			//����Ȃ��f�[�^���폜
			for (int i = 0; i < removeDatas.size(); i++) {
				auto iter = m_updateDatas.begin();

				while (iter != m_updateDatas.end()) {
					if (&(*iter) == removeDatas[i]) {
						iter = m_updateDatas.erase(iter);
						break;
					}
					iter++;
				}
			}
		}

		void OccupancyManager::AddOccupancyUpdateData(const std::shared_ptr<NavGraphNode>& node) {
			//�����f�[�^�����݂���Ȃ�A���Ԃ̍X�V�̂�
			for (auto& data : m_updateDatas) {
				if (data.node.lock() == node) {
					auto impactData = node->GetImpactData();
					data.timer->ResetTimer(CalculateOccupancyIntervalTime(node));
					return;
				}
			}

			auto impactData = node->GetImpactData();
			m_updateDatas.push_back(OccupancyUpdateData(GameTimer(CalculateOccupancyIntervalTime(node)), node));
		}

	}
}