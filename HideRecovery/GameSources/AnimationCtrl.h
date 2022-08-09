
/*!
@file AnimationCtrl.h
@brief AnimationCtrl.hクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "StateNodeBase.h"

namespace basecross {

	class I_AnimationStateNode {
	public:
		virtual ~I_AnimationStateNode() = default;

		virtual void OnStart() = 0;
		virtual bool OnUpdate(const float speed = 1.0f) = 0;
		virtual void OnExit() = 0;
	};

	class AnimationStateNodeBase : public I_AnimationStateNode
	{
		std::weak_ptr<GameObject> m_owner;
		std::weak_ptr<SmBaseDraw> m_model;

	public:
		AnimationStateNodeBase(const std::shared_ptr<GameObject>& owner) :
			m_owner(owner)
		{
			m_model = owner->GetComponent<SmBaseDraw>(false);
		}

		virtual ~AnimationStateNodeBase() = default;

		virtual bool OnUpdate(const float speed = 1.0f) override {
			if (!m_model.lock()) {
				return true;
			}

			auto delta = App::GetApp()->GetElapsedTime();
			auto model = m_model.lock();

			model->UpdateAnimation(delta * speed);

			return false;
		}

		/// <summary>
		/// 所有者の取得
		/// </summary>
		/// <returns>所有者</returns>
		std::shared_ptr<GameObject> GetOwner() const { return m_owner.lock(); }

		/// <summary>
		/// モデルの取得
		/// </summary>
		/// <returns>モデル</returns>
		std::shared_ptr<SmBaseDraw> GetModel() const { return m_model.lock(); }
	};

	class AnimationCtrl : public Component
	{
	public:
		AnimationCtrl(const std::shared_ptr<GameObject>& objPtr) 
			:Component(objPtr)
		{}

		virtual ~AnimationCtrl() = default;

		virtual void OnCreate() override {};
		virtual void OnUpdate() override {};
	};

}

//endbasecross