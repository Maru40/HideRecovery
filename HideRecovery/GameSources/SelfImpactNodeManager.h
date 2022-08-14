
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

		//--------------------------------------------------------------------------------------
		/// 影響マップの中で自分自身のノードを管理するコンポーネントのパラメータ
		//--------------------------------------------------------------------------------------
		struct SelfImpactNodeManager_Parametor {
			float range;		//この距離分離れたら、自身のノードを再検索する。
			float muchFarRange;	//現在のノードより離れすぎた場合に再検索するノード

			SelfImpactNodeManager_Parametor();
		};

		//--------------------------------------------------------------------------------------
		/// 影響マップの中で自分自身のノードを管理するコンポーネント
		//--------------------------------------------------------------------------------------
		class SelfImpactNodeManager : public Component
		{
		public:
			using Parametor = SelfImpactNodeManager_Parametor;

		private:
			Parametor m_param;						//パラメータ

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

		public:
			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 自分のノードを再検索する距離を設定
			/// </summary>
			/// <param name="range">自分のノードを再検索する距離</param>
			void SetRange(const float range) noexcept { m_param.range = range; }

			/// <summary>
			/// 自分のノードを再検索する距離を取得
			/// </summary>
			/// <returns>自分のノードを再検索する距離</returns>
			float GetRange() const noexcept { return m_param.range; }
		};

	}
}