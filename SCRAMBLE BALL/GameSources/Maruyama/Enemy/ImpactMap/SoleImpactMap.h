
/*!
@file SoleImpactMap.h
@brief SoleImpactMapなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	前方宣言
		//--------------------------------------------------------------------------------------
		class ImpactMap;
		class I_Impacter;

		//--------------------------------------------------------------------------------------
		///	パラメータ
		//--------------------------------------------------------------------------------------
		struct SoleImpactMap_Parametor 
		{
			Factory_ImpactMap_Parametor factoryParam;  //影響マップ生成用パラメータ
		};

		//--------------------------------------------------------------------------------------
		///	自分を中心に影響マップを展開するコンポーネント
		//--------------------------------------------------------------------------------------
		class SoleImpactMap : public Component
		{
		public:
			using Parametor = SoleImpactMap_Parametor;

		private:
			Parametor m_param;						//パラメータ

			std::shared_ptr<ImpactMap> m_impactMap;	//影響マップ

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			SoleImpactMap(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="parametor">パラメータ</param>
			SoleImpactMap(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

			void OnCreate() override;
			void OnUpdate() override;

		private:
			/// <summary>
			/// 影響マップの生成
			/// </summary>
			void CreateImpactMap();

		public:
			/// <summary>
			/// 影響度を計算して返す。
			/// </summary>
			/// <param name = "impacter">影響を与える者</param>
			void CalculateImpact(const std::shared_ptr<I_Impacter>& impacter);

		};

	}
}