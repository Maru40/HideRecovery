/*!
@file MetaballChildrenRender.cpp
@brief MetaballChildrenRenderのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "MetaballChildrenRender.h"

#include "MetaballChildrenObject.h"

namespace basecross {

	namespace Metaball {

		//--------------------------------------------------------------------------------------
		///	パラメータ
		//--------------------------------------------------------------------------------------

		ChildrenRender_Parametor::ChildrenRender_Parametor()
			: ChildrenRender_Parametor(1.0f, Col4(0, 1, 0, 0.5f))
		{}

		ChildrenRender_Parametor::ChildrenRender_Parametor(const float& radius, const Col4& color)
			: radius(radius), color(color), parent(nullptr), isDrawActive(true)
		{}

		//--------------------------------------------------------------------------------------
		///	Factory
		//--------------------------------------------------------------------------------------

		Factory_Children::Factory_Children(std::shared_ptr<Stage>& stage) :
			m_stage(stage)
		{}

		std::shared_ptr<ChildrenRender> Factory_Children::Create(const ChildrenRender_Parametor& parametor) {
			auto child = GetStage()->AddGameObject<ChildrenObject>();
			if (auto render = child->GetComponent<ChildrenRender>(false)) {
				render->SetParametor(parametor);
				return render;
			}

			return nullptr;  //存在しなかったらnullptrを返す。
		}

		std::vector<std::shared_ptr<ChildrenRender>> Factory_Children::Creates(const std::vector<ChildrenRender_Parametor>& params) {
			std::vector<std::shared_ptr<ChildrenRender>> renders;

			for (const auto& param : params) {
				renders.push_back(Create(param));
			}

			return renders;
		}

		std::shared_ptr<Stage> Factory_Children::GetStage() const {
			return m_stage.lock(); 
		}

		//--------------------------------------------------------------------------------------
		///	ChildrenRender
		//--------------------------------------------------------------------------------------

		ChildrenRender::ChildrenRender(const std::shared_ptr<GameObject>& objPtr)
			: ChildrenRender(objPtr, Parametor())
		{}

		ChildrenRender::ChildrenRender(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor)
			: Component(objPtr), m_param(parametor), m_scaleFadeFunction(nullptr)
		{}

		void ChildrenRender::OnUpdate() {
			if (m_scaleFadeFunction) {
				m_scaleFadeFunction();
			}
		}

		void ChildrenRender::ScaleFadeUpdate(const float& speed) {
			auto delta = App::GetApp()->GetElapsedTime();
			m_param.radius += -speed * delta;

			if (m_param.radius < 0.0f) {
				m_param.radius = 0.0f;
				GetStage()->RemoveGameObject<GameObject>(GetGameObject());
			}
		}

		void ChildrenRender::StartScaleFade(const float speed) {
			m_scaleFadeFunction = [&, speed]() { ScaleFadeUpdate(speed); };
		}

	}
}