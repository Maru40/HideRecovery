/*!
@file TackleCameraObject.h
@brief TackleCameraObjectヘッダ　
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	タックルカメラオブジェクト
	//--------------------------------------------------------------------------------------
	class TackleCameraObject : public GameObject
	{
		ex_weak_ptr<GameObject> m_seekTarget = nullptr; //追従ターゲットのオブジェクト

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		/// <param name="seekTarget">追従ターゲットのオブジェクト</param>
		TackleCameraObject(const std::shared_ptr<Stage>& stage, const std::shared_ptr<GameObject>& seekTarget);

		void OnCreate() override;

	private:
		/// <summary>
		/// 追従セッティング
		/// </summary>
		/// <param name="target">追従するターゲットのオブジェクト</param>
		void SettingSeek(const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// 注視点セッティング
		/// </summary>
		/// <param name="target">注視するターゲットのオブジェクト</param>
		void SettingLookAt(const std::shared_ptr<GameObject>& target);

	public:

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 追従ターゲットのセット
		/// </summary>
		/// <param name="target">追従するターゲット</param>
		void SetSeekTarget(const std::shared_ptr<GameObject>& target) noexcept;

		/// <summary>
		/// 注視するターゲットのセット
		/// </summary>
		/// <param name="target">注視するターゲット</param>
		void SetLockAtTarget(const std::shared_ptr<GameObject>& target) noexcept;
	};

}

//endbasecross