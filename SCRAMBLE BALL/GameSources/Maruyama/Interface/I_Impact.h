
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
	class NavGraphNode;

	namespace maru {
		class ImpactMap;

		//--------------------------------------------------------------------------------------
		///	影響範囲タイプ
		//--------------------------------------------------------------------------------------
		enum class ImpacterType {
			Circle,  //円範囲
			Eye,     //視界範囲
		};

		//--------------------------------------------------------------------------------------
		///	発見したもののタイプ
		//--------------------------------------------------------------------------------------
		enum class OccupancyType {
			Empty,		//空
			Enemy,		//敵
			HideItem,	//隠すアイテム
		};

		//--------------------------------------------------------------------------------------
		///	影響データ
		//--------------------------------------------------------------------------------------
		struct ImpactData {
			float dangerValue;				//危険度
			float occupancyValue;			//占有値(0.0f ～　1.0f : 0.5fが確認前、0.0fが確認済み、1.0fは発見直後)
			OccupancyType occupancyType;	//占有タイプ
			float occupacyRecoverySpeed;	//占有値の回復時間
			int areaIndex;					//エリアインデックス

			ImpactData();
			virtual ~ImpactData();
		};

		//--------------------------------------------------------------------------------------
		///	影響を与える者のデータ
		//--------------------------------------------------------------------------------------
		struct ImpacterData 
		{
			float value;							//影響度
			float circleRange;						//円の影響範囲
			ImpacterType type;						//影響範囲タイプ(視界が存在するなら、その分加算する。)
			std::weak_ptr<NavGraphNode> selfNode;	//自分が所属するノード

			ImpacterData();
		};

		//--------------------------------------------------------------------------------------
		///	影響を与える者のインターフェース
		//--------------------------------------------------------------------------------------
		class I_Impacter
		{
			ImpacterData m_impacterData; //影響を与える者のデータ

		public:
			virtual ~I_Impacter() = default;

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
			/// <returns>影響を与える者のデータ</returns>
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

			/// <summary>
			/// 影響マップの取得
			/// </summary>
			/// <returns>影響マップ</returns>
			virtual std::shared_ptr<ImpactMap> GetImpactMap() const = 0;

			/// <summary>
			/// 所属するノードの設定
			/// </summary>
			/// <param name="node">影響データ</param>
			void SetSelfNode(const std::shared_ptr<NavGraphNode>& node);

			/// <summary>
			/// 自分が所属するノードの取得
			/// </summary>
			/// <returns>自分が所属するノード</returns>
			std::shared_ptr<NavGraphNode> GetSelfNode() const;

		};

	}
}