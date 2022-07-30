/*!
@file SeekTPS.h
@brief SeekTPSなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	TPS追従コンポーネント
	//--------------------------------------------------------------------------------------
	class SeekTPS : public Component
	{
	public:
		//--------------------------------------------------------------------------------------
		///	追従タイプ
		//--------------------------------------------------------------------------------------
		enum class SeekType
		{
			Normal,				//通常
			Lerp,				//補完
		};

		//--------------------------------------------------------------------------------------
		///	パラメータ
		//--------------------------------------------------------------------------------------
		struct Parametor
		{
			float armRange;		//アームの長さ
			Vec3  armVec;		//アームの方向

			float radY;			//y軸の角度
			float radXZ;		//xz軸の角度

			float speedY;		//y軸の速度
			float speedXZ;		//xz軸の速度

			float maxY;			//y軸の最大
			float minY;			//y軸の最少

			float lerpSpeed;	//補間移動速度
			SeekType seekType;  //追従タイプ

			/// <summary>
			/// コンストラクタ
			/// </summary>
			Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="armRange">アームの長さ</param>
			/// <param name="armVec">アームの方向</param>
			/// <param name="radY">y軸の角度</param>
			/// <param name="radXZ">zy軸の角度</param>
			/// <param name="speedY">y軸の速度</param>
			/// <param name="speedXZ">xz軸の速度</param>
			/// <param name="maxY">y軸の最大角度</param>
			/// <param name="minY">z軸の最大角度</param>
			/// <param name="lerpSpeed">補完移動速度</param>
			/// <param name="seekType">追従タイプ</param>
			Parametor(const float& armRange,
				const Vec3& armVec,
				const float& radY,
				const float& radXZ,
				const float& speedY,
				const float& speedXZ,
				const float& maxY,
				const float& minY,
				const float& lerpSpeed,
				const SeekType& seekType);
		};

	private:

		ex_weak_ptr<GameObject> m_target;   //ターゲット
		Parametor m_param;					//パラメータ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="target">ターゲット</param>
		SeekTPS(const std::shared_ptr<GameObject>& objPtr,
			const std::shared_ptr<GameObject>& target
		):
			SeekTPS(
				objPtr,
				target,
				Parametor())
		{}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="target">ターゲット</param>
		/// <param name="parametor">パラメータ</param>
		SeekTPS(const std::shared_ptr<GameObject>& objPtr,
			const std::shared_ptr<GameObject>& target,
			const Parametor& parametor
		) :
			Component(objPtr),
			m_target(target),
			m_param(parametor)
		{}

		void OnCreate() override;
		void OnUpdate2() override;

	private:
		/// <summary>
		/// 目的地を計算する。
		/// </summary>
		/// <returns>計算した目的地</returns>
		Vec3 CalcuMovePosition();

		/// <summary>
		/// Y方向の入力
		/// </summary>
		void InputYVec();

		/// <summary>
		/// 横方向の入力
		/// </summary>
		void InputRXVec();

		/// <summary>
		/// Y方向の入力最大値チェック
		/// </summary>
		void CheckRimitY();

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// アームの長さの取得
		/// </summary>
		/// <returns>アーム長さ</returns>
		float GetArmRange() const noexcept {
			return m_param.armRange;
		}

		/// <summary>
		/// アームの長さの設定
		/// </summary>
		/// <param name="range">アームの長さ</param>
		void SetArmRange(const float range) noexcept {
			m_param.armRange = range;
		}

		/// <summary>
		/// ターゲットの設定
		/// </summary>
		/// <param name="target">ターゲット</param>
		void SetTarget(const std::shared_ptr<GameObject>& target) noexcept {
			m_target = target;
		}

		/// <summary>
		/// アームの方向の設定
		/// </summary>
		/// <param name="armVec">アームの方向</param>
		void SetArmVec(const Vec3& armVec) noexcept {
			m_param.armVec = armVec;
		}

		/// <summary>
		/// アームのオフセットの取得
		/// </summary>
		/// <returns></returns>
		Vec3 GetArmOffset() const {
			return m_param.armVec * m_param.armRange;
		}

		/// <summary>
		/// 追従タイプの設定
		/// </summary>
		/// <param name="type">追従タイプ</param>
		void SetSeekType(const SeekType type) noexcept {
			m_param.seekType = type;
		}

		/// <summary>
		/// 追従タイプの取得
		/// </summary>
		/// <returns>追従タイプ</returns>
		SeekType GetSeekType() const noexcept {
			return m_param.seekType;
		}

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
	};

}