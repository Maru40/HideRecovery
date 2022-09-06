
/*!
@file ImpactManager.h
@brief ImpactManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Interface/I_Impact.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	前方宣言
		//--------------------------------------------------------------------------------------
		class ImpactMap;

		//--------------------------------------------------------------------------------------
		///	影響マップの影響度管理
		//--------------------------------------------------------------------------------------
		class ImpactManager
		{
		private:
			/// <summary>
			/// ノードのアクティブ状態を更新
			/// </summary>
			/// <param name="impactMap">影響マップ</param>
			/// <param name="impacter">影響を与える者</param>
			void NodeActiveUpdate(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter);

			/// <summary>
			/// エッジのアクティブ状態を更新
			/// </summary>
			/// <param name="impactMap">影響マップ</param>
			/// <param name="impacter">影響を与える者</param>
			void EdgeActiveUpdate(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter);

		public:

			/// <summary>
			/// 自分の周囲に影響度を更新する。
			/// </summary>
			/// <param name="impactMap">影響マップ</param>
			/// <param name="impacter">影響を与える者</param>
			void CalculateAroundImpact(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter);

			/// <summary>
			/// 視界範囲の影響度を更新する
			/// </summary>
			/// <param name="impactMap">影響マップ</param>
			/// <param name="impacter">影響を与える者</param>
			void CalculateEyeImpact(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter);

		public:

			/// <summary>
			/// 影響度を計算する。
			/// </summary>
			/// <param name = "impactMap">影響マップ</param>
			/// <param name = "impacter">影響を与える者</param>
			void CalculateAllImpact(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter);

		};

	}
}

//basecross