/*!
@file MetaballChildrenRender.h
@brief MetaballChildrenRenderなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Metaball {

		//--------------------------------------------------------------------------------------
		///	前方宣言
		//--------------------------------------------------------------------------------------
		class ChildrenRender;

		//--------------------------------------------------------------------------------------
		///	メタボールの子供の描画パラメータ
		//--------------------------------------------------------------------------------------
		struct ChildrenRender_Parametor
		{
			float radius;                   //半径
			Col4 color;                     //色
			ex_weak_ptr<GameObject> parent; //親オブジェクト
			bool isDrawActive;              //表示するかどうか

			/// <summary>
			/// コンストラクタ
			/// </summary>
			ChildrenRender_Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="radius">半径</param>
			/// <param name="color">色</param>
			ChildrenRender_Parametor(const float& radius, const Col4& color);
		};

		//--------------------------------------------------------------------------------------
		///	メタボールの子供の生成クラス
		//--------------------------------------------------------------------------------------
		class Factory_Children 
		{
		private:
			std::weak_ptr<Stage> m_stage;	//自分が所属するステージ

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="stage">このクラスが所属するステージ</param>
			Factory_Children(std::shared_ptr<Stage>& stage);

			/// <summary>
			/// 生成
			/// </summary>
			/// <param name="parametor">生成用パラメータ</param>
			/// <returns>生成したオブジェクト</returns>
			std::shared_ptr<ChildrenRender> Create(const ChildrenRender_Parametor& parametor);

			/// <summary>
			/// 複数生成
			/// </summary>
			/// <param name="params">生成用パラメータ配列</param>
			/// <returns>生成したオブジェクトの配列</returns>
			std::vector<std::shared_ptr<ChildrenRender>> Creates(const std::vector<ChildrenRender_Parametor>& params);

			/// <summary>
			/// ステージの取得
			/// </summary>
			/// <returns>ステージ</returns>
			std::shared_ptr<Stage> GetStage() const;
		};

		//--------------------------------------------------------------------------------------
		///	メタボールの子供の描画
		//--------------------------------------------------------------------------------------
		class ChildrenRender : public Component
		{
		public:
			using Parametor = ChildrenRender_Parametor;

		private:
			Parametor m_param;							//パラメータ

			std::function<void()> m_scaleFadeFunction;	//大きさフェードイベント

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			ChildrenRender(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="parametor">パラメータ</param>
			ChildrenRender(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

			void OnUpdate() override;

		private:
			/// <summary>
			/// フェード更新(小さくなって消える)
			/// </summary>
			/// <param name="speed">フェードスピード</param>
			void ScaleFadeUpdate(const float& speed);

		public:
			/// <summary>
			/// フェードスタート(小さくなって消える)
			/// </summary>
			/// <param name="speed">フェードスピード</param>
			void StartScaleFade(const float speed);

			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// パラメータの設定
			/// </summary>
			/// <param name="parametor">パラメータ</param>
			void SetParametor(const Parametor& parametor) noexcept {
				m_param = parametor;
			}
			
			/// <summary>
			/// パラメータの取得
			/// </summary>
			/// <returns>パラメータ</returns>
			Parametor GetParametor() const noexcept {
				return m_param;
			}

			/// <summary>
			/// 半径の設定
			/// </summary>
			/// <param name="radius">半径</param>
			void SetRadius(const float& radius) noexcept {
				m_param.radius = radius;
			}

			/// <summary>
			/// 半径の取得
			/// </summary>
			/// <returns>半径</returns>
			float GetRadius() const {
				if (m_param.parent) {
					auto scale = m_param.parent->GetComponent<Transform>()->GetScale();
					return m_param.radius * scale.x;	
				}

				return m_param.radius;
			}

			/// <summary>
			/// ローカル半径の取得
			/// </summary>
			/// <returns>ローカル半径</returns>
			float GetLocalRadius() const noexcept {
				return m_param.radius;
			}

			/// <summary>
			/// 色の設定
			/// </summary>
			/// <param name="color">色</param>
			void SetColor(const Col4& color) noexcept {
				m_param.color = color;
			}

			/// <summary>
			/// 色の取得
			/// </summary>
			/// <returns>色</returns>
			Col4 GetColor() const noexcept {
				return m_param.color;
			}

			/// <summary>
			/// 位置の取得
			/// </summary>
			/// <returns>位置</returns>
			Vec3 GetPosition() const {
				return transform->GetPosition();
			}

			/// <summary>
			/// スフィアデータの取得
			/// </summary>
			/// <returns>スフィアデータ</returns>
			Vec4 GetSphereData() const {
				auto center = GetPosition();
				return Vec4(center.x, center.y, center.z, GetRadius());
			}

			/// <summary>
			/// 親の設定
			/// </summary>
			/// <param name="parent">親</param>
			void SetParent(const std::shared_ptr<GameObject>& parent) noexcept {
				m_param.parent = parent;
			}

			/// <summary>
			/// 親の取得
			/// </summary>
			/// <returns>親</returns>
			std::shared_ptr<GameObject> GetParent() const {
				return m_param.parent.GetShard();
			}

			/// <summary>
			/// 描画しているかどうか
			/// </summary>
			/// <returns>描画しているならtrue</returns>
			bool IsDrawActive() const noexcept {
				return m_param.isDrawActive;
			}

			/// <summary>
			/// 描画するかどうか
			/// </summary>
			/// <param name="isDraw">描画するかどうか</param>
			void SetDrawActive(const bool isDraw) noexcept {
				m_param.isDrawActive = isDraw;
			}

		};

	}
}