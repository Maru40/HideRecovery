
/*!
@file SlimeCore.cpp
@brief SlimeCoreのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "SlimeCore.h"

#include "SeekTarget.h"
#include "VelocityManager.h"

#include "ScaleParent.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 目のオブジェクト
		//--------------------------------------------------------------------------------------

		EyeObject::EyeObject(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		void EyeObject::OnCreate() {
			auto draw = AddComponent<BcPNTStaticDraw>();
			draw->SetMeshResource(L"DEFAULT_SPHERE");
		}

		void EyeObject::SetDiffuse(const Col4& color) {
			auto draw = GetComponent<BcPNTStaticDraw>();
			draw->SetDiffuse(color);
		}

		//--------------------------------------------------------------------------------------
		/// スライムのコア本体
		//--------------------------------------------------------------------------------------

		SlimeCore::SlimeCore(const std::shared_ptr<GameObject>& objPtr)
			:Component(objPtr)
		{}

		void SlimeCore::OnCreate() {
			CreateCore();
		}

		void SlimeCore::OnUpdate() {
			if (m_parentObject) {
				m_parentObject->GetComponent<Transform>()->SetForward(transform->GetForward());
			}
		}

		void SlimeCore::OnDestroy() {
			GetStage()->RemoveGameObject<GameObject>(m_parentObject.GetShard());
			GetStage()->RemoveGameObject<GameObject>(m_inObject.GetShard());
			GetStage()->RemoveGameObject<GameObject>(m_outObject.GetShard());
		}

		void SlimeCore::CreateCore() {
			auto seekParam = SeekTarget::Parametor();
			seekParam.range = 0.0f;
			seekParam.offset = Vec3(0.0f, 0.0f, 0.0f);
			seekParam.seekType = SeekTarget::SeekType::Lerp;
			seekParam.isScaleParent = true;
			m_parentObject = GetStage()->Instantiate<GameObject>(transform->GetPosition(), transform->GetQuaternion());
			m_parentObject->AddComponent<SeekTarget>(GetGameObject(), seekParam);
			m_parentObject->AddComponent<VelocityManager>();
			m_parentObject->AddComponent<ScaleParent>(GetGameObject());

			CreateInObject();
			CreateOutObject();
		}

		void SlimeCore::CreateInObject() {
			auto object = GetStage()->AddGameObject<EyeObject>();
			m_inObject = object;

			//スケール
			const Vec3 Scale = Vec3(0.35f);
			//色
			const Col4 color(0.1f, 0.8f, 0.1f, 1.0f);
			object->SetDiffuse(color);
			//追従
			auto seekParam = SeekTarget::Parametor();
			seekParam.range = 0.0f;
			seekParam.offset = Vec3(0.0f, 0.0f, 0.45f);
			seekParam.seekType = SeekTarget::SeekType::Normal;
			seekParam.isScaleParent = true;
			object->SetParent(m_parentObject.GetShard());

			object->GetComponent<Transform>()->SetScale(Scale);
			object->GetComponent<Transform>()->SetPosition(seekParam.offset);
		}

		void SlimeCore::CreateOutObject() {
			auto object = GetStage()->AddGameObject<EyeObject>();
			m_outObject = object;

			//スケール
			const Vec3 Scale = Vec3(1.0f);
			object->GetComponent<Transform>()->SetScale(Scale);
			//追従
			auto seekParam = SeekTarget::Parametor();
			seekParam.range = 0.0f;
			seekParam.offset = Vec3(0.0f, 0.0f, 0.0f);
			seekParam.seekType = SeekTarget::SeekType::Normal;

			object->SetParent(m_parentObject.GetShard());
			object->GetComponent<Transform>()->SetScale(Scale);
			object->GetComponent<Transform>()->SetPosition(seekParam.offset);
		}

		void SlimeCore::OnDraw() {
			Component::OnDraw();
		}

		void SlimeCore::OnDrawFalse() {
			m_parentObject->SetDrawActive(false);
			m_inObject->SetDrawActive(false);
			m_outObject->SetDrawActive(false);
		}

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		void SlimeCore::SetEyeDiffuse(const Col4& color) {
			m_inObject->SetDiffuse(color);
		}

		void SlimeCore::SetEyeBallDiffuse(const Col4& color) {
			m_outObject->SetDiffuse(color);
		}

		std::shared_ptr<SeekTarget> SlimeCore::GetSeekTargetPtr() const {
			if (m_parentObject) {
				return m_parentObject->GetComponent<SeekTarget>(false);
			}
			
			return nullptr;
		}

		void SlimeCore::SetSeekParametor(const SeekTarget::Parametor& seekParam) {
			auto seek = m_parentObject->GetComponent<SeekTarget>(false);
			if (seek) {
				seek->SetParametor(seekParam);
			}
		}

		SeekTarget_Parametor SlimeCore::GetSeekParametor() const {
			return m_parentObject->GetComponent<SeekTarget>(false)->GetParametor();
		}

	}
}