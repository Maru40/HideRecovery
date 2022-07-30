
/*!
@file MetaballChildrenSeekManager.cpp
@brief MetaballChildrenSeekManager�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "GameTimer.h"
#include "TaskList.h"
#include "SeekTargetOffsetsManager.h"
#include "SeekTarget.h"
#include "MetaballChildrenSeekManager.h"

#include "MetaballRenderBase.h"
#include "MetaballChildrenRender.h"

#include "VelocityManager.h"
#include "MyRandom.h"

#include "DebugObject.h"

namespace basecross {

	namespace Metaball {

		//--------------------------------------------------------------------------------------
		///	�p�����[�^
		//--------------------------------------------------------------------------------------

		ChildrenSeekManager_Parametor::ChildrenSeekManager_Parametor()
			:ChildrenSeekManager_Parametor(
				vector<SeekTarget::Parametor>()
			)
		{}

		ChildrenSeekManager_Parametor::ChildrenSeekManager_Parametor(const vector<SeekTarget::Parametor>& seekParams)
			: seekParams(seekParams)
		{}

		//--------------------------------------------------------------------------------------
		///	���^�{�[���̒Ǐ]�{��
		//--------------------------------------------------------------------------------------

		ChildrenSeekManager::ChildrenSeekManager(const std::shared_ptr<GameObject>& objPtr)
			: ChildrenSeekManager(objPtr, Parametor())
		{}

		ChildrenSeekManager::ChildrenSeekManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor)
			: Component(objPtr), m_param(Parametor(parametor)), m_seekOffsetsManager(nullptr)
		{}

		void ChildrenSeekManager::OnCreate() {
			SettingSeek();
			//Children��SeekOffset�p�̃f�[�^�𐶐�
			auto render = GetGameObject()->GetComponent<RenderBase>(false);
			if (render) {
				m_seekOffsetsManager = std::make_unique<SeekTargetOffsetsManager>(GetGameObject(), render->GetChildrenComponents<SeekTarget>());
			}
		}

		void ChildrenSeekManager::OnUpdate() {
			if (m_seekOffsetsManager) {
				m_seekOffsetsManager->OnUpdate();
			}
		}

		void ChildrenSeekManager::SettingSeek() {
			auto render = GetGameObject()->GetComponent<RenderBase>(false);
			if (!render) {
				MessageBox(0, L"Metaball::RenderBase�R���|�[�l���g�����݂��܂���B", L"Metaball::ChildrenSeekManager::SettingSeek()", 0);
			}

			auto children = render->GetChildren();
			const auto& seekParams = m_param.seekParams;

			int index = 0;
			for (const auto& child : children) {
				auto childObject = child->GetGameObject();
				auto seek = childObject->GetComponent<SeekTarget>(false);
				//�Ǐ]�R���|�[�l���g�����邩�ǂ���
				if (seek) {
					seek->SetTarget(GetGameObject(), true);
				}
				else {
					seek = childObject->AddComponent<SeekTarget>(GetGameObject());
				}

				//SeekParametor���Z�b�g�B
				SetSeekParametor(seek, index++);
			}
		}

		void ChildrenSeekManager::SetSeekParametor(const std::shared_ptr<SeekTarget>& seek, const int& index) {
			const auto& seekParams = m_param.seekParams;

			//SeekParametor�����݂���Ȃ�p�����[�^���Z�b�g�B
			if (index < seekParams.size()) {
				seek->SetParametor(seekParams[index]);
			}
			else {
				DebugObject::AddString(L"ChildrenSeekManager::SettingSeek()�ASeekParametor������܂���");
			}
		}

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		void ChildrenSeekManager::StartMove(const SeekTargetOffsetsManager::TaskEnum& task, const SeekTargetOffsetsManager::Parametor& parametor){
			m_seekOffsetsManager->StartMove(task, parametor);
		}

		//Seek��Ԃ��������Ēe��������B
		void ChildrenSeekManager::Crash(const Vec3& velocity) {
			auto render = GetGameObject()->GetComponent<Metaball::RenderBase>(false);
			if (!render) {
				return;
			}

			auto seekTargets = render->GetChildrenComponents<SeekTarget>();
			auto power = velocity.length();
			for (const auto& seek : seekTargets) {
				seek->SetUpdateActive(false); 
				auto velocityManager = seek->GetGameObject()->GetComponent<VelocityManager>(false);
				if (velocityManager) {
					auto randomVec = maru::MyRandom::RandomVector(Vec3(power));
					
					auto direct = velocity + randomVec;
					velocityManager->SetVelocity(direct.GetNormalized() * power);
					velocityManager->StartDeseleration();
				}
			}
		}

		bool ChildrenSeekManager::IsEndTask() {
			return m_seekOffsetsManager->IsEndTask();
		}

		//--------------------------------------------------------------------------------------
		///	Utility
		//--------------------------------------------------------------------------------------

		namespace Utility {

			vector<SeekTarget::Parametor> ChildrenSeekSetting::CreateSeekTargetParametors(
				const float& speed,
				const float& range,
				const std::vector<Vec3> offsets,
				const SeekTarget::SeekType& seekType)
			{
				vector<SeekTarget::Parametor> parametors;
				for (const auto& offset : offsets) {
					parametors.push_back(SeekTarget::Parametor(speed, range, offset, seekType));
				}

				return parametors;
			}

		}

	}
}

//endbasecross