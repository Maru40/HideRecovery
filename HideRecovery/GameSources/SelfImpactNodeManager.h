
/*!
@file SelfImpactNodeManager.h
@brief SelfImpactNodeManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------

	namespace maru {

		class I_Impacter;

		struct SelfImpactNodeManager_Parametor {
			float range;	//この距離分離れたら、自身のノードを再検索する。

		};

		//--------------------------------------------------------------------------------------
		/// 影響マップの中で自分自身のノードを管理するコンポーネント
		//--------------------------------------------------------------------------------------
		class SelfImpactNodeManager : public Component
		{
		public:
			using Parametor = SelfImpactNodeManager_Parametor;

		private:
			Parametor m_param;

			std::weak_ptr<I_Impacter> m_impacter;	//影響を与える者

		public:
			SelfImpactNodeManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<I_Impacter>& impacter);

			void OnUpdate() override;

		private:
			/// <summary>
			/// 自分自身のノード更新
			/// </summary>
			void SelfNodeUpdate();

			/// <summary>
			/// 自分自身のノードを更新する。
			/// </summary>
			void SettingSelfNode();

		};

	}
}