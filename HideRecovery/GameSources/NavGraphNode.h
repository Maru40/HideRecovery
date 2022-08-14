
/*!
@file NavGraphNode.h
@brief NavGraphNodeなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "GraphNodeBase.h"

#include "I_Impact.h"
#include "ReactiveProperty.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	template<class T>
	class ReactiveProperty;

	//--------------------------------------------------------------------------------------
	///	ナビグラフ用のノード
	//--------------------------------------------------------------------------------------
	class NavGraphNode : public GraphNodeBase
	{
	public:
		using ImpactData = maru::ImpactData;

	private:
		Vec3 m_position;											//ノードの場所
		std::unique_ptr<ReactiveProperty<ImpactData>> m_impactData;	//影響マップデータ

		std::weak_ptr<GameObject> m_parent;							//親オブジェクト

	public:

		NavGraphNode();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="index">このクラスのインデックス</param>
		NavGraphNode(const int& index);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="index">このクラスのインデックス</param>
		/// <param name="position">座標</param>
		NavGraphNode(const int& index, const Vec3& position);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="index">このクラスのインデックス</param>
		/// <param name="position">座標</param>
		/// <param name="impactData">影響マップデータ</param>
		NavGraphNode(const int& index, const Vec3& position, const maru::ImpactData& impactData);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="index">このクラスのインデックス</param>
		/// <param name="position">座標</param>
		/// <param name="impactData">影響マップデータ</param>
		/// <param name="parent">親オブジェクト</param>
		NavGraphNode(const int& index, const Vec3& position, const maru::ImpactData& impactData, const std::shared_ptr<GameObject>& parent);

		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="node">コピーするノード</param>
		NavGraphNode(const NavGraphNode& node);

		virtual ~NavGraphNode();

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 座標のセット
		/// </summary>
		/// <param name="position">座標</param>
		void SetPosition(const Vec3& position) noexcept { m_position = position; }

		/// <summary>
		/// 座標の取得
		/// </summary>
		/// <returns>座標</returns>
		Vec3 GetPosition() const;

		/// <summary>
		/// ローカル座標の取得
		/// </summary>
		/// <returns>ローカル座標</returns>
		Vec3 GetLocalPosition() const noexcept { return m_position; }

		/// <summary>
		/// 影響データのセット
		/// </summary>
		/// <param name="data">影響データ</param>
		void SetImpactData(const ImpactData& data) noexcept;

		/// <summary>
		/// 影響データの取得
		/// </summary>
		/// <returns>影響データ</returns>
		ImpactData GetImpactData() const noexcept;

		/// <summary>
		/// 親オブジェクトが存在するかどうか判断
		/// </summary>
		/// <returns>親が存在するならtrue</returns>
		bool IsParent() const noexcept { return !m_parent.expired(); }

		/// <summary>
		/// 親オブジェクトのセット
		/// </summary>
		/// <param name="parent">親オブジェクト</param>
		void SetParent(const std::shared_ptr<GameObject>& parent) noexcept { m_parent = parent; }

		/// <summary>
		/// 親オブジェクトの取得
		/// </summary>
		/// <returns>親オブジェクト</returns>
		std::shared_ptr<GameObject> GetParent() const { return m_parent.lock(); }

		//--------------------------------------------------------------------------------------
		///	オペレータ
		//--------------------------------------------------------------------------------------

		NavGraphNode& operator= (const NavGraphNode& other);
	};

}

//endbasecross