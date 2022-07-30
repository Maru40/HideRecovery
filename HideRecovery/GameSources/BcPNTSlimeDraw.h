/*!
@file BcPNTSlimeDraw.h
@brief BcPNTSlimeDraw
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// スフィアデータ
	//--------------------------------------------------------------------------------------
	struct SphereData {
		Vec3 position; //位置
		float radius;  //半径
		Col4 color;    //色
	};

	struct SlimeConstants {
		Col4 diffuseColor;
		Col4 emissiveColor;
		Col4 specularColorAndPower;

		Col4 lightDirection[3];
		Col4 lightDiffuseColor[3];
		Col4 lightSpecularColor[3];

		Vec4 eyePosition;

		Col4 fogColor;
		Vec4 fogVector;

		Mat4x4 world;
		Vec4 worldInverseTranspose[3];
		Mat4x4 worldViewProj;
		//汎用フラグ
		XMUINT4 activeFlg;
		//以下影
		Vec4 lightPos;
		Vec4 eyePos;
		Mat4x4 lightView;
		Mat4x4 lightProjection;

		int useNumSpheres;                     //使用するスフィアの数
		int numRayLoop = 10;                   //Rayループ回数
		float rayHitDistance = 0.025f;         //rayHitDistanceを上げれば滑らかさが増す。(法線と深度の制度が下がる。)
		float smoothMinConnectValue = 1.25f;   //コネクト用の定数。大きければ滑らかに繋がり、小さければ通常のスフィアに近づく。

		Col4 rimColor = Col4(1.0000000f, 0.9607844f, 0.8078432f, 1.0f);  //リムカラー
		float rimPower = 1.75f;                //リムパワー
		float minRimRate = 0.2f;               //最小リムレート
		float maxRimRate = 4.0f;               //最大リムレート
		float colorDistinctness = 0.7f;        //色の分離具合。小さいほど色が混ざり、大きいほど色がはっきりする。

		SphereData sphereDatas[256];           //スフィアデータ配列
	};

	//コンスタントバッファの実装
	DECLARE_DX11_CONSTANT_BUFFER(SlimeBuffer, SlimeConstants)

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	namespace Metaball {
		class ChildrenRender;
	}

	//--------------------------------------------------------------------------------------
	/// スライムのドローコンポーネント
	//--------------------------------------------------------------------------------------
	class BcPNTSlimeDraw :public BcBaseDraw
	{
		//イディオム
		struct Impl;
		unique_ptr<Impl> pImpl;

		vector<ex_weak_ptr<Metaball::ChildrenRender>> m_renders;  //書き込むデータ配列

		bool m_isDraw = false;  //通常レンダーで描画しないようにするための処理

	public :
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		BcPNTSlimeDraw(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override{}

		void OnDraw() override;

	private:
		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="data">メッシュデータ</param>
		void DrawStatic2(const MeshPrimData& data);

		/// <summary>
		/// コンスタントバッファの設定
		/// </summary>
		/// <param name="pointCb">設定を書き込む変数</param>
		/// <param name="data">メッシュデータ</param>
		void SetSlimeConstants(SlimeConstants& pointCb, const MeshPrimData& data);

		/// <summary>
		/// 描画するかどうか
		/// </summary>
		/// <param name="targetPosition">ターゲットの位置</param>
		/// <returns>描画するならtrue</returns>
		bool IsMetaballDraw(const Vec3& targetPosition);

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// アクティブにする
		/// </summary>
		void Active() {
			m_isDraw = true;
		}

		/// <summary>
		/// 描画するターゲットの配列設定
		/// </summary>
		/// <param name="renders">描画するターゲット配列</param>
		void SetRenders(const vector<ex_weak_ptr<Metaball::ChildrenRender>>& renders);

		/// <summary>
		/// 描画するターゲットの追加
		/// </summary>
		/// <param name="render">追加するターゲット</param>
		void AddRender(const std::shared_ptr<Metaball::ChildrenRender>& render);

		/// <summary>
		/// 描画するターゲットの削除
		/// </summary>
		/// <param name="render">削除するターゲット</param>
		void RemoveRender(const ex_weak_ptr<Metaball::ChildrenRender>& render);

		/// <summary>
		/// 全てのメタボールの中心位置を取得
		/// </summary>
		/// <returns>全てのメタボールの中心位置</returns>
		Vec3 GetChildrenCenterPosition() const;
	};

}

//endbasecross