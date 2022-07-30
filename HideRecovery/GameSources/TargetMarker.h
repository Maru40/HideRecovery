/*!
@file TargetMarker.h
@brief TargetMarkerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ターゲットマーカー
	//--------------------------------------------------------------------------------------
	class TargetMarker : public Component
	{
	public:
		/// <summary>
		/// パラメータ
		/// </summary>
		struct Parametor;

	private:
		std::unique_ptr<Parametor> m_param; //パラメータ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		TargetMarker(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;

	private:
		/// <summary>
		/// サークルオブジェクトの生成
		/// </summary>
		void CreateCircleObject();

		/// <summary>
		/// セグメントUIの生成
		/// </summary>
		/// <returns>生成UI</returns>
		std::shared_ptr<GameObject> AddSegmentObject();

		/// <summary>
		/// 使わないセグメントの非表示
		/// </summary>
		/// <param name="useNum">使わないセグメントの数</param>
		void NotUseSegmentManager(const int& useNum);

	public:
		/// <summary>
		/// 使用するマーカーの数を受け取る
		/// </summary>
		/// <param name="useNum">使用するマーカーの数</param>
		void AcceptUseSegmentMarker(const int& useNum);

		/// <summary>
		/// Success状態にする。
		/// </summary>
		/// <param name="segment">Success状態にするセグメント</param>
		void StartSuccess(const std::shared_ptr<GameObject>& segment);

		/// <summary>
		/// Success状態終了する
		/// </summary>
		void EndSuccess(); 

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// サークルマーカーを取得
		/// </summary>
		/// <returns>サークルマーカーの取得</returns>
		std::shared_ptr<GameObject> GetCircleMarker() const;

		/// <summary>
		/// ターゲット用のセグメントマーカーを取得
		/// </summary>
		/// <returns>ターゲット用のセグメントマーカー</returns>
		std::shared_ptr<GameObject> GetTargetSegmentMarker() const;

		/// <summary>
		/// Success時のセグメントマーカーを取得
		/// </summary>
		/// <returns>Success時のセグメントマーカーを/returns>
		std::shared_ptr<GameObject> GetSuccessSegmentMarker() const;

		/// <summary>
		/// 指定した数のセグメントマーカーを取得
		/// </summary>
		/// <param name="getNum">取得したいセグメントマーカーを数</param>
		/// <returns>指定した数のセグメントマーカーを/returns>
		vector<std::shared_ptr<GameObject>> GetSegmentMarkers(const int& getNum);

		/// <summary>
		/// 全てのセグメントマーカーを取得
		/// </summary>
		/// <returns>すべてのセグメントマーカーを/returns>
		vector<ex_weak_ptr<GameObject>> GetAllSegmentMarkers() const;

		/// <summary>
		/// セグメントマーカーを使用するかどうかを設定
		/// </summary>
		/// <param name="isUse">セグメントマーカーを使用するかどうか</param>
		void SetIsUseMarker(const bool isUse);

		/// <summary>
		/// 現在使用中のマーカーの数を取得
		/// </summary>
		/// <returns>現在使用中のマーカー</returns>
		int GetUseNum() const;
	};

}

//endbasecross