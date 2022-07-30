/*!
@file LaserBase.h
@brief LaserBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	enum class ScaleChangeType {
		Normal,
		Reverse,
	};

	//--------------------------------------------------------------------------------------
	/// レーザーの描画パラメータ
	//--------------------------------------------------------------------------------------
	struct LaserBaseDraw_Parametor
	{
		float width = 0.25f;		 //横幅
		float maxWidth = 3.0f;		 //最大の横幅
		float length = 1.0f;		 //長さ
		float maxLength = 5.0f;		 //最大の長さ
									 
		float longerSpeed = 1.0f;	 //伸びるスピード
		float smallerSpeed = 1.0f;	 //縮まるスピード
									 
		Vec3 forward;				 //正面方向
		Col4 color;					 //色
		wstring texture;			 //テクスチャ名
		ScaleChangeType longerType;  //伸びるタイプ
		ScaleChangeType smallerType; //縮むタイプ
		bool isSmaller = false;	     //縮まっている状態かどうか

		/// <summary>
		///	コンストラクタ
		/// </summary>
		LaserBaseDraw_Parametor();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="width">横幅</param>
		/// <param name="maxWidth">最大の横幅</param>
		/// <param name="length">長さ</param>
		/// <param name="maxLength">最大の長さ</param>
		/// <param name="longerSpeed">伸びるスピード</param>
		/// <param name="smallerSpeed">縮まるスピード</param>
		/// <param name="forward">正面方向</param>
		/// <param name="color">色</param>
		/// <param name="texture">テクスチャ名</param>
		LaserBaseDraw_Parametor(
			const float& width, const float& maxWidth, 
			const float& length, const float& maxLength,
			const float& longerSpeed, const float& smallerSpeed,
			const Vec3& forward, const Col4& color,
			const wstring& texture);
	};

	class LaserBaseDraw : public Component
	{
	public:
		using Parametor = LaserBaseDraw_Parametor;
		using DrawComp = BcPCTStaticDraw;

	private:
		Parametor m_param;                                  //パラメータ

		std::vector<VertexPositionColorTexture> m_vertices; //頂点データ配列

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスが所有するゲームオブジェクト</param>
		LaserBaseDraw(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスが所有するゲームオブジェクト</param>
		/// <param name="param">パラメータ</param>
		LaserBaseDraw(const std::shared_ptr<GameObject>& objPtr, const Parametor& param);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// 伸びる更新処理
		/// </summary>
		void LongerUpdate();

		/// <summary>
		/// 縮む更新処理
		/// </summary>
		void SmallerUpdate();

		/// <summary>
		/// 頂点の更新
		/// </summary>
		void VerticesUpdate();

		/// <summary>
		/// 更新処理終了時に呼ぶ処理
		/// </summary>
		void EndProcess();

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 縮んでいる状態かどうか
		/// </summary>
		/// <returns>縮んでいるならtrue</returns>
		bool IsSmaller() const {
			return m_param.isSmaller;
		}

		/// <summary>
		/// 縮んでいる状態の設定
		/// </summary>
		/// <param name="isSmaller">縮んでいる状態かどうか</param>
		void SetIsSmaller(const bool isSmaller) {
			m_param.isSmaller = isSmaller;
			if (isSmaller) {
				transform->SetPosition(transform->GetPosition() + (GetLongerForward() * GetLength()));
			}
		}

		/// <summary>
		/// 正面方向の取得
		/// </summary>
		/// <returns>正面方向</returns>
		Vec3 GetForward() const {
			return m_param.forward;
		}

		/// <summary>
		/// 正面方向の設定
		/// </summary>
		/// <param name="forward">正面方向</param>
		void SetForward(const Vec3& forward) noexcept {
			m_param.forward = forward;
		}

		/// <summary>
		/// 伸びる正面方向
		/// </summary>
		/// <returns>伸びる正面方向</returns>
		Vec3 GetLongerForward() const {
			ScaleChangeType type = m_param.isSmaller ? m_param.smallerType : m_param.longerType;

			switch (type)
			{
			case ScaleChangeType::Normal:
				return m_param.forward;
				break;

			case ScaleChangeType::Reverse:
				return -m_param.forward;
				break;
			}

			return m_param.forward;
		}

		/// <summary>
		/// 長さの取得
		/// </summary>
		/// <returns>長さ</returns>
		float GetLength() const {
			return m_param.length;
		}

		/// <summary>
		/// 横幅の取得
		/// </summary>
		/// <returns>横幅</returns>
		float GetWidth() const {
			return m_param.width;
		}

		/// <summary>
		/// 色の取得
		/// </summary>
		/// <returns>色</returns>
		Col4 GetColor() const {
		 	return m_param.color;
		}

		/// <summary>
		/// 色の設定
		/// </summary>
		/// <param name="color">色</param>
		void SetColor(const Col4& color) {
			m_param.color = color;
		}

		/// <summary>
		/// 伸びるタイプの取得
		/// </summary>
		/// <returns>伸びるタイプ</returns>
		ScaleChangeType GetLongerType() const {
			return m_param.longerType;
		}

		/// <summary>
		/// 伸びるタイプの設定
		/// </summary>
		/// <param name="type">伸びるタイプ</param>
		void SetLongerType(const ScaleChangeType& type) {
			m_param.longerType = type;
		}

		/// <summary>
		/// テクスチャの設定
		/// </summary>
		/// <param name="texture">テクスチャ名</param>
		void SetTexture(const wstring& texture) {
			GetGameObject()->GetComponent<DrawComp>()->SetTextureResource(texture);
		}
	};

}