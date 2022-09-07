/*!
@file TargetManager.h
@brief TargetManager
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	ターゲット管理
	//--------------------------------------------------------------------------------------
	class TargetManager : public Component
	{
	public:
		/// <summary>
		/// 見失ったターゲットのデータ
		/// </summary>
		struct LostData;

		/// <summary>
		/// パラメータ
		/// </summary>
		struct Parametor;

	private:
		std::unique_ptr<Parametor> m_paramPtr;	//パラメータ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		TargetManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ターゲットが存在するかどうか
		/// </summary>
		/// <returns>ターゲットが存在するならtrue</returns>
		bool HasTarget() const;

		/// <summary>
		/// ターゲットの位置を取得する
		/// </summary>
		/// <returns>ターゲットの位置</returns>
		Vec3 GetTargetPosition() const;

		/// <summary>
		/// ターゲットを見失った位置を取得する
		/// </summary>
		/// <returns>ターゲットを見失った位置</returns>
		Vec3 GetLostPosition() const;
	
		/// <summary>
		/// ターゲットの設定
		/// </summary>
		/// <param name="target">ターゲット</param>
		void SetTarget(const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// ターゲットの取得
		/// </summary>
		/// <returns>ターゲット</returns>
		std::shared_ptr<GameObject> GetTarget() const;

		/// <summary>
		/// ターゲット方向のベクトルを返す。
		/// </summary>
		/// <returns>ターゲット方向のベクトル</returns>
		Vec3 CalcuToTargetVec();
	};

}

//endbasecrosss