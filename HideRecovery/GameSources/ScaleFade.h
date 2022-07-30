/*!
@file ScaleFade.h
@brief ScaleFade
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "SingletonComponent.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	大きさフェードのパラメータ
	//--------------------------------------------------------------------------------------
	struct ScaleFade_Parametor
	{
		float speed;                                  //フェードスピード
		Vec3 direct = Vec3(-1.0f);                    //フェード方向
		Vec3 targetScale = Vec3(0.0f);                //目標スケール
		std::function<void()> endFunction = nullptr;  //フェード終了時に呼び出すイベント
	};

	//--------------------------------------------------------------------------------------
	///	大きさフェードコンポーネント
	//--------------------------------------------------------------------------------------
	class ScaleFade : public Component
	{
	public:
		using Parametor = ScaleFade_Parametor;

	private:
		Parametor m_param; //パラメータ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="parametor">パラメータ</param>
		ScaleFade(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnUpdate() override;

	private:
		/// <summary>
		/// 大きさ更新
		/// </summary>
		void UpdateScale();

		/// <summary>
		/// スケールの変更を止めるかどうか
		/// </summary>
		/// <param name="scale">現在のスケール</param>
		/// <param name="targetScale">目標スケール</param>
		/// <returns>目標スケールより小さくなったらtrue</returns>
		bool IsStopScale(const Vec3& scale, const Vec3& targetScale) const;

	public:
		/// <summary>
		/// 終了判断
		/// </summary>
		/// <returns>大きさの更新が終了していたらtrue</returns>
		bool IsEnd() const {
			return IsStopScale(transform->GetScale(), m_param.targetScale);
		}
	};

}