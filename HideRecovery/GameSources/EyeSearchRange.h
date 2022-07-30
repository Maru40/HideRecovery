/*!
@file EyeSearchRangeParam.h
@brief EyeSearchRangeParamなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"
#include "DelegateSystem.h"

namespace basecross {

	//------------------------------------------------------------------------------------------------
	/// 視界ターゲット管理用のパラメータ
	//------------------------------------------------------------------------------------------------
	struct EyeTargetParametor {
		std::shared_ptr<GameObject> target;  //ターゲットのポインタ
		mutable bool isFind;                 //見つけた状態かどうかを返す

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="target">ターゲットのポインタ</param>
		EyeTargetParametor(const std::shared_ptr<GameObject>& target):
			target(target),
			isFind(false)
		{}
	};

	//------------------------------------------------------------------------------------------------
	/// 視界範囲パラメータ
	//------------------------------------------------------------------------------------------------
	struct EyeSearchRangeParametor {
		float length; //索敵範囲(円形)
		float height; //索敵範囲(高さ)
		float rad;    //索敵範囲(角度)

		/// <summary>
		/// コンストラクタ
		/// </summary>
		EyeSearchRangeParametor()
			:EyeSearchRangeParametor(20.0f,3.0f,XMConvertToRadians(30.0f))
		{}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="length">索敵範囲(円形)</param>
		/// <param name="height">索敵範囲(高さ)</param>
		/// <param name="rad">索敵範囲(角度)</param>
		EyeSearchRangeParametor(const float& length,
			const float& height,
			const float& rad
		):
			length(length),
			height(height),
			rad(rad)
		{}
	};


	//------------------------------------------------------------------------------------------------
	/// 視界範囲をコントロールするコンポーネント
	//------------------------------------------------------------------------------------------------
	class EyeSearchRange : public Component
	{
		std::vector<EyeTargetParametor> m_targetParams; //範囲に入っているかの対象になるオブジェクト

		EyeSearchRangeParametor m_param;                //視界範囲パラメータ

		/// <summary>
		/// ターゲットが自分を中心に球体状の範囲内にいるかどうか判断
		/// </summary>
		/// <param name="target">索敵ターゲット</param>
		/// <returns>範囲内ならtrue</returns>
		bool IsRange(const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// ターゲットが自分の索敵範囲内の「高さ」にいるかどうかを判断
		/// </summary>
		/// <param name="target">索敵ターゲット</param>
		/// <returns>範囲内ならtrue</returns>
		bool IsHeight(const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// ターゲットが自分の索敵範囲内の角度にいるか判断
		/// </summary>
		/// <param name="target">索敵ターゲット</param>
		/// <returns>範囲内ならtrue</returns>
		bool IsRad(const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// Rayを飛ばしたときのチェック
		/// </summary>
		/// <param name="targetParam">索敵ターゲット</param>
		/// <returns>範囲内ならtrue</returns>
		bool IsRay(const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// ターゲットが索敵範囲内にいるときに呼び出される関数
		/// </summary>
		void Hit(const EyeTargetParametor& targetParam);

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するオブジェクト</param>
		EyeSearchRange(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するオブジェクト</param>
		/// <param name="param">視界範囲パラメータ</param>
		EyeSearchRange(const std::shared_ptr<GameObject>& objPtr,
			const EyeSearchRangeParametor& param);

		void OnUpdate() override;

		/// <summary>
		/// 視界内にいるならtrueを返す
		/// </summary>
		/// <param name="target">ターゲット</param>
		/// <returns>視界の中にいるならtrue</returns>
		bool IsInEyeRange(std::shared_ptr<GameObject>& target);

		/// <summary>
		/// 視界内にいるならtrueを返す
		/// </summary>
		/// <param name="target">ターゲット</param>
		/// <returns>視界の中にいるならtrue</returns>
		bool IsInEyeRange(std::shared_ptr<GameObject>& target, const float& length);

		//------------------------------------------------------------------------------------------------
		/// アクセッサ
		//------------------------------------------------------------------------------------------------

		/// <summary>
		/// ターゲットの追加
		/// </summary>
		/// <param name="obj">ターゲットのゲームオブジェクト</param>
		void AddTarget(const std::shared_ptr<GameObject>& target) {
			m_targetParams.push_back(EyeTargetParametor(target));
		}

		/// <summary>
		/// 視界範囲パラメータの設定
		/// </summary>
		/// <param name="param">視界範囲パラメータ</param>
		void SetParametor(const EyeSearchRangeParametor& param) noexcept {
			m_param = param;
		}

		/// <summary>
		/// 視界範囲パラメータの取得
		/// </summary>
		/// <returns>視界範囲パラメータ</returns>
		EyeSearchRangeParametor GetParametor() const noexcept {
			return m_param;
		}

		/// <summary>
		/// 視界範囲(円形)の設定
		/// </summary>
		/// <param name="length">視界範囲(円形)</param>
		void SetEyeLength(const float& length) noexcept {
			m_param.length = length;
		}

		/// <summary>
		/// 視界範囲(円形)の取得
		/// </summary>
		/// <returns>視界範囲(円形)</returns>
		float GetEyeLength() const noexcept {
			return m_param.length;
		}

		/// <summary>
		/// 視界範囲(高さ)の取得
		/// </summary>
		/// <param name="height">視界範囲(高さ)</param>
		void SetEyeHeight(const float& height) noexcept {
			m_param.height = height;
		}

		/// <summary>
		/// 視界範囲(高さ)の取得
		/// </summary>
		/// <returns>視界範囲(高さ)</returns>
		float GetEyeHeight() const noexcept {
			return m_param.height;
		}

		/// <summary>
		/// 視界範囲(デグリー角)の設定
		/// </summary>
		/// <param name="degree">視界範囲(デグリー角)</param>
		void SetEyeDegree(const float& degree) {
			m_param.rad = XMConvertToRadians(degree);
		}

		/// <summary>
		/// 視界範囲(デグリー角)の取得
		/// </summary>
		/// <returns>視界範囲(デグリー角)</returns>
		float GetEyeDegree() const {
			return XMConvertToDegrees(m_param.rad);
		}

		/// <summary>
		/// 視界範囲(ラジアン角)の設定
		/// </summary>
		/// <param name="radian">視界範囲(ラジアン角)</param>
		void SetEyeRadian(const float& radian) noexcept {
			m_param.rad = radian;
		}

		/// <summary>
		/// 視界範囲(ラジアン角)の取得
		/// </summary>
		/// <returns>視界範囲(ラジアン角)</returns>
		float GetEyeRadian() const noexcept {
			return m_param.rad;
		}

		/// <summary>
		/// 見つけている状態かどうかを判断する
		/// </summary>
		/// <param name="target">ターゲット</param>
		/// <returns>視界の範囲ならtrue</returns>
		bool IsLookTarget(const std::shared_ptr<GameObject>& target) {
			for (auto param : m_targetParams) {
				if (target == param.target) {  //同じ種類だったら
					return param.isFind;
				}
			}

			return false;
		}

	};

}

//endbasecross