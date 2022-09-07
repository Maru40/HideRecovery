/*!
@file HeightDestroy.h
@brief HeightDestroyなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	namespace maru {
		template<class T>
		class Action;
	}

	//--------------------------------------------------------------------------------------
	/// 一定の高さで死亡判定を出すコンポーネントのパラメータ
	//--------------------------------------------------------------------------------------
	struct HeightDestroy_Parametor
	{
		float height;

		HeightDestroy_Parametor();
	};

	//--------------------------------------------------------------------------------------
	/// 一定の高さで死亡判定を出すコンポーネント
	//--------------------------------------------------------------------------------------
	class HeightDestroy : public Component
	{
	public:
		using Parametor = HeightDestroy_Parametor;

	private:
		Parametor m_param;

		std::unique_ptr<maru::Action<void()>> m_action;

	public:
		HeightDestroy(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

	private:
		void ObserveHeight();

		void Invoke();

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		void AddDestroyAction(const std::function<void()>& func);

		bool IsDestroyHeight() const;

		void SetHeight(const float height) noexcept { m_param.height = height; }

		float GetHeight() const noexcept { return m_param.height; }
	};

}