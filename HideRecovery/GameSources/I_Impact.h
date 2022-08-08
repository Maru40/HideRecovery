
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
		enum class ImpacterType {
			Circle,  //円範囲
			Eye,     //視界範囲
		};

		//--------------------------------------------------------------------------------------
		///	影響データ
		//--------------------------------------------------------------------------------------
		struct ImpactData {
			float dangerValue;				//危険度
			float confirmationIntervalTime;	//再確認までのインターバル
			bool isConfirmation;			//確認済みかどうか

			ImpactData();
		};

		//--------------------------------------------------------------------------------------
		///	影響を与える者のデータ
		//--------------------------------------------------------------------------------------
		struct ImpacterData 
		{
			float value = 0.0f;					  //影響度
			float circleRange = 0.0f;			  //円の影響範囲
			ImpacterType type = ImpacterType(0);  //影響範囲タイプ(視界が存在するなら、その分加算する。)
		};

		//--------------------------------------------------------------------------------------
		///	影響を与える者のインターフェース
		//--------------------------------------------------------------------------------------
		class I_Impacter
		{
			ImpacterData m_impacterData; //影響を与える者のデータ

		public:

			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 影響を与える者のデータを設定
			/// </summary>
			/// <param name="data">影響データ</param>
			ImpacterData SetImpacterData(const ImpacterData& data) noexcept { m_impacterData = data; }

			/// <summary>
			/// 影響を与える者のデータを取得
			/// </summary>
			/// <returns>影響データ</returns>
			ImpacterData GetImpacterData() const noexcept { return m_impacterData; }

			/// <summary>
			/// 影響を与える者のデータの参照を取得
			/// </summary>
			/// <returns></returns>
			ImpacterData& GetRefImpactData() noexcept { return m_impacterData; }

			/// <summary>
			/// このクラスを所有するゲームオブジェクトを取得する
			/// </summary>
			/// <returns>このクラスを所有するゲームオブジェクト</returns>
			virtual std::shared_ptr<GameObject> GetImpacterObject() const noexcept = 0;

			/// <summary>
			/// 視界範囲管理クラスを取得
			/// </summary>
			/// <returns>視界範囲管理クラス</returns>
			virtual std::shared_ptr<EyeSearchRange> GetEyeSearchRange() const = 0;

		};

	}
}