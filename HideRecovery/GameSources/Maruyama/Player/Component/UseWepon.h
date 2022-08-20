
/*!
@file UseWepon.h
@brief UseWeponなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class WeponBase;
	class RotationController;

	namespace maru {
		class ReactiveBool;
	}

	//--------------------------------------------------------------------------------------
	/// ウェポンを使用するクラスのパラメータ
	//--------------------------------------------------------------------------------------
	struct UseWepon_Parametor {
		std::unique_ptr<maru::ReactiveBool> isAim;	//Aim中かどうか

		UseWepon_Parametor();

		UseWepon_Parametor(const bool isAim);

		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		UseWepon_Parametor(const UseWepon_Parametor& parametor);

		virtual ~UseWepon_Parametor() = default;
	};

	//--------------------------------------------------------------------------------------
	/// ウェポンを使用するクラス
	//--------------------------------------------------------------------------------------
	class UseWepon : public Component
	{
	public:
		using Parametor = UseWepon_Parametor;

	private:
		Parametor m_param;					//パラメータ

		std::weak_ptr<WeponBase> m_wepon;	//武器

		std::weak_ptr<RotationController> m_rotationController;	//回転コントローラー

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		UseWepon(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="wepon">武器</param>
		UseWepon(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<WeponBase>& wepon);

		virtual ~UseWepon() = default;

		void OnLateStart() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// エイム中の更新処理
		/// </summary>
		void AimUpdate();

		/// <summary>
		/// 回転更新
		/// </summary>
		void RotationUpdate();

		/// <summary>
		/// エイム状態を切り替えたときに呼び出したい処理の設定
		/// </summary>
		void SettingReactiveIsAim();

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// エイム中の設定
		/// </summary>
		/// <param name="isAim">エイム中かどうか</param>
		void SetIsAim(const bool isAim);

		/// <summary>
		/// エイム中かどうか
		/// </summary>
		/// <returns>エイム中ならtrue</returns>
		bool IsAim() const;

		/// <summary>
		/// 武器の設定
		/// </summary>
		/// <param name="wepon">武器</param>
		void SetWepon(const std::shared_ptr<WeponBase>& wepon) noexcept;

		/// <summary>
		/// 武器の取得
		/// </summary>
		/// <returns>武器</returns>
		std::shared_ptr<WeponBase> GetWepon() const noexcept;
		
	};

}