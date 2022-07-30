#pragma once
/*!
@file MetaballRenderBase.h
@brief MetaballRenderBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class BcPNTSlimeDraw;
	class RayMarchingFilterObject;

	namespace Metaball {

		class ChildrenRender;
		struct ChildrenRender_Parametor;

		//--------------------------------------------------------------------------------------
		///	メタボール描画の基底クラス
		//--------------------------------------------------------------------------------------
		class RenderBase : public Component
		{
		private:
			int m_numCreate;											//生成する数

			vector<ex_weak_ptr<ChildrenRender>> m_children;				//管理するメタボールの配列

			ex_weak_ptr<RayMarchingFilterObject> m_rayMarchingFilter;	//レイマーチング用のフィルターを生成
			ex_weak_ptr<BcPNTSlimeDraw> m_draw;							//ドロ―コンポーネント

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			RenderBase(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="numCreate">生成する数</param>
			RenderBase(const std::shared_ptr<GameObject>& objPtr, const int& numCreate);

			void OnCreate() override;
			void OnDestroy() override;

		private:
			/// <summary>
			/// メタボールを生成
			/// </summary>
			void CreateChildren();

		public:
			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// ディフューズカラーの設定
			/// </summary>
			/// <param name="color">ディフューズカラー</param>
			void SetDiffuse(const Col4& color);

			/// <summary>
			/// メタボールの半径の設定
			/// </summary>
			/// <param name="radius">半径</param>
			void SetChildrenRadius(const float& radius);

			/// <summary>
			/// メタボールの親の設定
			/// </summary>
			/// <param name="parent">親</param>
			void SetChildrenParent(const std::shared_ptr<GameObject>& parent);

			/// <summary>
			/// メタボールの配列を返す
			/// </summary>
			/// <returns>メタボールの配列</returns>
			vector<ex_weak_ptr<ChildrenRender>> GetChildren() const;

			/// <summary>
			/// メタボールのチルドレンに親設定があるかどうか
			/// </summary>
			/// <returns>親が存在するならtrue</returns>
			bool IsChildrenParent() const;

			/// <summary>
			/// メタボールのチルドレンに親設定を取得
			/// </summary>
			/// <returns>メタボールの親</returns>
			std::shared_ptr<GameObject> GetChildrenParent() const;

			/// <summary>
			/// 子オブジェクトの追加
			/// </summary>
			/// <param name="child">子オブジェクト</param>
			void AddChild(const std::shared_ptr<ChildrenRender>& child);

			/// <summary>
			/// 子オブジェクトの複数追加
			/// </summary>
			/// <param name="children">子オブジェクトの配列</param>
			void AddChildren(const std::vector<std::shared_ptr<ChildrenRender>>& children);

			/// <summary>
			/// 子オブジェクトの削除
			/// </summary>
			/// <param name="child">削除する子オブジェクト</param>
			void RemoveChild(const ex_weak_ptr<ChildrenRender>& child);

			/// <summary>
			/// 全ての子オブジェクトの解放
			/// </summary>
			void AllReleaseChildren();

			/// <summary>
			/// 指定したコンポーネントを配列にして返す。
			/// </summary>
			/// <returns>コンポーネントの配列</returns>
			template<class T>
			vector<ex_weak_ptr<T>> GetChildrenComponents() {
				vector<ex_weak_ptr<T>> resultVec;
				for (const auto& child : m_children) {
					if (!child) {
						continue;
					}

					resultVec.push_back(child->GetGameObject()->GetComponent<T>(false));
				}

				return resultVec;
			}

			/// <summary>
			/// 全てのメタボールの追従オフセットを配列で取得
			/// </summary>
			/// <returns>全てのメタボールのオフセット</returns>
			vector<Vec3> CalculateChildrenSeekOffsets();

			void OnDrawActive() override;
			void OnDrawFalse() override;

		};

	}
}

//endbasecross