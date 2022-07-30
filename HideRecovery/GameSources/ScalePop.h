/*!
@file ScalePop.h
@brief ScalePopヘッダ　
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	namespace maru {
		enum class DeltaType;
	}

	//--------------------------------------------------------------------------------------
	///	スケールポップのパラメータ
	//--------------------------------------------------------------------------------------
	struct ScalePop_Parametor {
		Vec3 maxScale;              //最大の大きさ
		Vec3 returnScale;           //一度大きくなってから戻る大きさ
		float speed;                //大きさ変更速度
		float startScale;           //初期の大きさ
		maru::DeltaType deltaType;  //時間計測タイプ

		/// <summary>
		/// コンストラクタ
		/// </summary>
		ScalePop_Parametor();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="maxScale">最大の大きさ</param>
		/// <param name="returnScale">一度大きくなってから戻る大きさ</param>
		/// <param name="speed">大きさ変更速度</param>
		ScalePop_Parametor(const Vec3& maxScale, const Vec3& returnScale, const float& speed);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="maxScale">最大の大きさ</param>
		/// <param name="returnScale">一度大きくなってから戻る大きさ</param>
		/// <param name="speed">大きさ更新速度</param>
		/// <param name="startScale">初期の大きさ</param>
		ScalePop_Parametor(const Vec3& maxScale, const Vec3& returnScale, const float& speed, const float& startScale);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="maxScale">最大の大きさ</param>
		/// <param name="returnScale">一度大きくなってから戻る大きさ</param>
		/// <param name="speed">大きさ変更速度</param>
		/// <param name="startScale">初期の大きさ</param>
		/// <param name="deltaType">時間計測タイプ</param>
		ScalePop_Parametor(const Vec3& maxScale, const Vec3& returnScale, const float& speed, const float& startScale, maru::DeltaType deltaType);
	};

	//--------------------------------------------------------------------------------------
	///	スケールポップ
	//--------------------------------------------------------------------------------------
	class ScalePop : public Component
	{
	public:
		using Parametor = ScalePop_Parametor;

	private:
		Vec3 m_maxScale;                //最大スケール
		Vec3 m_returnScale;				//一度大きくなってから戻るスケール
		float m_speed;					//スケール変更スピード
		maru::DeltaType m_deltaType;	//時間計測タイプ
										
		float m_elapsed = 0.0f;         //現在の計測時間

		/// <summary>
		/// スケールの変動が終了したかどうかを判断
		/// </summary>
		/// <param name="scale">現在のスケール</param>
		/// <param name="targetScale">目標のスケール</param>
		/// <returns>終了するならtrue</returns>
		bool IsStopScale(const Vec3& scale, const Vec3& targetScale);

		/// <summary>
		/// 大きさ更新
		/// </summary>
		void ScaleUpdate();

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		ScalePop(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="parametor">パラメータ</param>
		ScalePop(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnUpdate() override;

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 最大の大きさの設定
		/// </summary>
		/// <param name="scale">最大の大きさ</param>
		void SetMaxScale(const Vec3& scale) noexcept { m_maxScale = scale; }

		/// <summary>
		/// 最大の大きさの取得
		/// </summary>
		/// <returns>最大の大きさ</returns>
		Vec3 GetMaxScale() const noexcept { return m_maxScale; }

		/// <summary>
		/// 一度大きくなってから戻る大きさの設定
		/// </summary>
		/// <param name="scale">一度大きくなってから戻る大きさ</param>
		void SetReturnScale(const Vec3& scale) noexcept { m_returnScale = scale; }

		/// <summary>
		/// 一度大きくなってから戻る大きさの取得
		/// </summary>
		/// <returns>一度大きくなってから戻る大きさ</returns>
		Vec3 GetReturnScale() const noexcept { return m_returnScale; }

		/// <summary>
		/// 大きさ変更速度の設定
		/// </summary>
		/// <param name="speed">大きさ変更速度</param>
		void SetSpeed(const float speed) noexcept { m_speed = speed; }

		/// <summary>
		/// 大きさ変更速度の取得
		/// </summary>
		/// <returns>大きさ変更速度</returns>
		float GetSpeed() const noexcept { return m_speed; }

		/// <summary>
		/// 大きさ変更開始
		/// </summary>
		/// <param name="speed">大きさ変更速度</param>
		/// <param name="startScaleSize">開始時の大きさ</param>
		void StartPop(const float& speed, const float& startScaleSize = 0.0f) {
			m_speed = speed;
			m_elapsed = startScaleSize;
			ScaleUpdate();
		}

		/// <summary>
		/// 終了判断
		/// </summary>
		/// <returns>大きさ変更が完了したらtrue</returns>
		bool IsPopEnd() const {
			return m_speed == 0.0f;
		}
	};

}

//endbasecross