
/*!
@file I_Impact.h
@brief I_Impactなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class EyeSearchRange;

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	影響範囲タイプ
		//--------------------------------------------------------------------------------------
		enum class ImpactType {
			Circle,  //円範囲
			Eye,     //視界範囲
		};

		//--------------------------------------------------------------------------------------
		///	影響データ
		//--------------------------------------------------------------------------------------
		struct ImpactData 
		{
			float value = 0.0f;               //影響度
			float circleRange = 0.0f;         //円の影響範囲
			ImpactType type = ImpactType(0);  //影響範囲タイプ(視界が存在するなら、その分加算する。)
		};

		//--------------------------------------------------------------------------------------
		///	影響インターフェース
		//--------------------------------------------------------------------------------------
		class I_Impacter
		{
			ImpactData m_impactData; //影響データ

		public:

			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 影響データのセット
			/// </summary>
			/// <param name="data">影響データ</param>
			ImpactData SetImpactData(const ImpactData& data) noexcept { m_impactData = data; }

			/// <summary>
			/// 影響データの取得
			/// </summary>
			/// <returns>影響データ</returns>
			ImpactData GetImpactData() const noexcept { return m_impactData; }

			/// <summary>
			/// 影響データの参照を取得
			/// </summary>
			/// <returns></returns>
			ImpactData& GetRefImpactData() noexcept { return m_impactData; }

			/// <summary>
			/// このクラスを所有するゲームオブジェクトを取得する
			/// </summary>
			/// <returns>このクラスを所有するゲームオブジェクト</returns>
			virtual std::shared_ptr<GameObject> GetImpactObject() const noexcept = 0;

			/// <summary>
			/// 視界範囲管理クラスを取得
			/// </summary>
			/// <returns>視界範囲管理クラス</returns>
			virtual std::shared_ptr<EyeSearchRange> GetEyeSearchRange() const = 0;

		};

	}
}