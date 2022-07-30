
/*!
@file MetaballRenderBase.cpp
@brief MetaballRenderBaseのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "MetaballRenderBase.h"

#include "MetaballChildrenObject.h"
#include "MetaballChildrenRender.h"

#include "RayMarchingFilter.h"
#include "BcPNTSlimeDraw.h"

#include "SeekTarget.h"

#include "MaruUtility.h"
#include "MaruAction.h"

namespace basecross {

	namespace Metaball {

		RenderBase::RenderBase(const std::shared_ptr<GameObject>& objPtr)
			:RenderBase(objPtr, 0)
		{}

		RenderBase::RenderBase(const std::shared_ptr<GameObject>& objPtr, const int& numCreate)
			: Component(objPtr), m_numCreate(numCreate)
		{}

		void RenderBase::OnCreate() {
			auto stage = GetStage();
			m_rayMarchingFilter = stage->AddGameObject<RayMarchingFilterObject>();
			m_draw = m_rayMarchingFilter->GetComponent<BcPNTSlimeDraw>();

			CreateChildren();
			m_draw->SetRenders(m_children);
		}

		void RenderBase::OnDestroy() {
			GetStage()->RemoveGameObject<RayMarchingFilterObject>(m_rayMarchingFilter.GetShard());  //Rayマーチングの削除

			//子供の全て削除
			for (const auto& child : m_children) {
				if (child) {
					GetStage()->RemoveGameObject<ChildrenObject>(child->GetGameObject());
				}
			}
		}

		void RenderBase::CreateChildren() {
			auto stage = GetStage();
			for (int i = 0; i < m_numCreate; i++) {
				auto slime = stage->AddGameObject<ChildrenObject>();         //スライムの生成

				m_children.push_back(slime->GetComponent<ChildrenRender>()); //チルドレンのレンダー用
			}
		}

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		void RenderBase::SetDiffuse(const Col4& color) {
			for (auto& child : m_children) {
				child->SetColor(color);
			}
		}

		void RenderBase::SetChildrenRadius(const float& radius) {
			for (auto& child : m_children) {
				child->SetRadius(radius);
			}
		}

		void RenderBase::SetChildrenParent(const std::shared_ptr<GameObject>& parent) {
			for (auto& child : m_children) {
				child->SetParent(parent);
			}
		}

		vector<ex_weak_ptr<ChildrenRender>> RenderBase::GetChildren() const {
			return m_children;
		}

		bool RenderBase::IsChildrenParent() const {
			if (m_children.empty()) {
				return false;
			}

			if (m_children[0]) {
				return m_children[0]->GetParent() ? true : false;
			}

			return false;
		}

		std::shared_ptr<GameObject> RenderBase::GetChildrenParent() const {
			if (m_children.empty()) {
				return nullptr;
			}

			return m_children[0] ? m_children[0]->GetParent() : nullptr;
		}

		void RenderBase::AddChild(const std::shared_ptr<ChildrenRender>& child) {
			m_children.push_back(child);
			m_draw->AddRender(child);
		}

		void RenderBase::AddChildren(const std::vector<std::shared_ptr<ChildrenRender>>& children) {
			for (const auto& child : children) {
				AddChild(child);
			}
		}

		void RenderBase::RemoveChild(const ex_weak_ptr<ChildrenRender>& child) {
			maru::Utility::RemoveVec(m_children, child);
			m_draw->RemoveRender(child);
		}

		void RenderBase::AllReleaseChildren() {
			maru::Action<void()> action;
			for (auto& child : m_children) {
				action += [&, child]() { RemoveChild(child); };
			}

			action.Invoke();
		}

		vector<Vec3> RenderBase::CalculateChildrenSeekOffsets() {
			vector<Vec3> offsets;
			auto seekTargets = GetChildrenComponents<SeekTarget>();
			for (const auto& seek : seekTargets) {
				if (seek) {
					offsets.push_back(seek->GetOffset());
				}
			}

			return offsets;
		}

		void RenderBase::OnDrawActive() {
			m_draw ? m_draw->SetDrawActive(GetDrawActive()) : nullptr;
		}

		void RenderBase::OnDrawFalse() {
			m_draw ? m_draw->SetDrawActive(GetDrawActive()) : nullptr;
		}
	}
}