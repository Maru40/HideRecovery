/*!
@file TargetMarkerManager.h
@brief TargetMarkerManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class TargetMarker;

	//--------------------------------------------------------------------------------------
	/// ターゲットマーカー管理
	//--------------------------------------------------------------------------------------
	class TargetMarkerManager : public Component
	{
	public:
		/// <summary>
		/// ターゲットデータ
		/// </summary>
		struct TargetData;

		/// <summary>
		/// パラメータ
		/// </summary>
		struct Parametor;

	private:
		std::unique_ptr<TargetData> m_targetDataPtr;  //ターゲットデータ
		std::unique_ptr<Parametor> m_paramPtr;        //パラメータ

		ex_weak_ptr<TargetMarker> m_marker;           //マーカー

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		TargetMarkerManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// マーカーの更新
		/// </summary>
		void MarkerUpdate();

		/// <summary>
		/// サークルマーカーの更新
		/// </summary>
		void CircleMarkerUpdate();

		/// <summary>
		/// ターゲットマーカーの更新
		/// </summary>
		void TargetMarkerUpdate();

		/// <summary>
		/// セグメントマーカーの更新
		/// </summary>
		void SegmentMarkerUpdate();

		/// <summary>
		/// 成功マーカーの更新
		/// </summary>
		void SuccessMarkerUpdate();

		/// <summary>
		/// セグメントの設定
		/// </summary>
		/// <param name="segment">セグメントのポイント</param>
		/// <param name="direct">向ける方向</param>
		/// <param name="color">カラー</param>
		void SettingSegment(const std::shared_ptr<GameObject>& segment, Vec3 direct, const Col4& color);

		/// <summary>
		/// 成功状態のセグメントのセッティングをする
		/// </summary>
		void SettingSuccessSegment();

		/// <summary>
		/// 潰しが成功する角度に追い込めたかどうか
		/// </summary>
		bool IsSuccess() const; 

		/// <summary>
		/// 成功状態の方向のセグメントを見つける
		/// </summary>
		/// <returns>成功状態のセグメント</returns>
		std::shared_ptr<GameObject> FindSuccessSegment() const;

	public:

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// マーカーのアクティブ設定
		/// </summary>
		/// <param name="isActive">アクティブかどうか</param>
		void SetMarkerActive(const bool isActive);

		/// <summary>
		/// ターゲットが存在するかどうか
		/// </summary>
		/// <returns>ターゲットが存在するならtrue</returns>
		bool HasTarget() const;

		/// <summary>
		/// ターゲットの設定
		/// </summary>
		/// <param name="target">ターゲット</param>
		void SetTarget(const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// ターゲットデータの設定
		/// </summary>
		/// <param name="data">ターゲットデータ</param>
		void SetTargetData(const TargetData& data);

		/// <summary>
		/// ターゲットの取得
		/// </summary>
		/// <returns>ターゲット</returns>
		std::shared_ptr<GameObject> GetTarget() const;
	};

}

//endbasecross