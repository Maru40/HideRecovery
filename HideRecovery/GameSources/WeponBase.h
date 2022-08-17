
/*!
@file WeponBase.h
@brief WeponBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ウェポンのパラメータ
	//--------------------------------------------------------------------------------------
	struct WeponBase_Parametor {
		Vec3 bulletInstanceOffset;	//バレットを生成する位置のオフセット

		WeponBase_Parametor();
	};

	//--------------------------------------------------------------------------------------
	/// ウェポンの基底クラス
	//--------------------------------------------------------------------------------------
	template<class BulletType>
	class WeponBase : public Component
	{
	public:
		using Parametor = WeponBase_Parametor;

	private:
		Parametor m_param;

	public:
		WeponBase(const std::shared_ptr<GameObject>& objPtr):
			Component(objPtr)
		{}

		/// <summary>
		/// 弾を撃つ
		/// </summary>
		/// <param name="direct">撃つ方向</param>
		virtual void Shot(const Vec3& direct) = 0;

		/// <summary>
		/// 弾の生成
		/// </summary>
		template<class... Ts>
		std::shared_ptr<BulletType> InstantiateBullet(const Vec3& position, const Quat& quat, Ts... params) {
			auto bulletObject = GetStage()->Instantiate<BulletType>(position, quat, params...);
			//auto bullet = bulletObject->AddComponent<BulletType>();

			return bulletObject;
		}

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// パラメータの設定
		/// </summary>
		/// <param name="parametor">パラメータ</param>
		void SetParametor(const Parametor& parametor) noexcept { m_param = parametor; }

		/// <summary>
		/// パラメータの取得
		/// </summary>
		/// <returns>パラメータ</returns>
		Parametor GetParametor() const noexcept { return m_param; }

		/// <summary>
		/// バレットを生成する位置のオフセットの設定
		/// </summary>
		/// <param name="offset">バレットを生成する位置のオフセット</param>
		void SetBulletInstanceOffset(const Vec3& offset) noexcept { m_param.bulletInstanceOffset = offset; }

		/// <summary>
		/// バレットを生成する位置のオフセットの取得
		/// </summary>
		/// <returns>バレットを生成する位置のオフセット</returns>
		Vec3 GetBulletInstanceOffset() const noexcept { return m_param.bulletInstanceOffset; }

	};

}