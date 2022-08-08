/*!
@file ImpactDataDraw.h
@brief ImpactDataDrawクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class NavGraphNode;
	class NumbersObject;

	//--------------------------------------------------------------------------------------
	///	影響データ表示用クラス
	//--------------------------------------------------------------------------------------
	class ImpactDataDraw : public Component
	{
		//--------------------------------------------------------------------------------------
		///	表示データ
		//--------------------------------------------------------------------------------------
		struct DrawData {
			ex_weak_ptr<NumbersObject> dangerNumber;	//危険度表示オブジェクト

			DrawData();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="numbersObject">危険度表示オブジェクト</param>
			DrawData(const std::shared_ptr<NumbersObject>& dangerNumbers);
		};

		ex_weak_ptr<NavGraphNode> m_node;	//ノードのポインタ
		DrawData m_drawData;				//表示データ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="node">ノードのポインタ</param>
		ImpactDataDraw(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<NavGraphNode>& node);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// 危険度表示オブジェクトの生成
		/// </summary>
		/// <returns>危険度表示オブジェクト</returns>
		std::shared_ptr<NumbersObject> CreateDangerNumbersObject(const Vec3& offset = Vec3(0.0f));
	};

	//--------------------------------------------------------------------------------------
	///	影響データ表示用オブジェクト
	//--------------------------------------------------------------------------------------
	class ImpactDataDrawObject : public GameObject
	{
		ex_weak_ptr<NavGraphNode> m_node;	//ノードのポインタ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		/// <param name="node">ノードのポインタ</param>
		ImpactDataDrawObject(const std::shared_ptr<Stage>& stage, const std::shared_ptr<NavGraphNode>& node);

		void OnCreate() override;
	};
}