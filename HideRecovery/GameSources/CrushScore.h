/*!
@file CrushScore.h
@brief 潰した時の成功判定
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//-----------------------------------------------------------------------------------------------
	/// スライムを上手くつぶした時のランク表示クラス
	//-----------------------------------------------------------------------------------------------
	class CrushScore : public GameObject
	{
		Vec3 m_StartPos = Vec3(500.0f, 0.0f, 0.0f); //! 初期位置
		Vec3 m_MovePower = Vec3(0.0f, 230.0f, 0.0f);
		std::list<std::shared_ptr<GameObject>> m_ScoreUi; //! スコア表示のUI

	public:
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief コンストラクタ
		*/
		CrushScore::CrushScore(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}
		//-----------------------------------------------------------------------------------------------
		void OnUpdate() override;
		//-----------------------------------------------------------------------------------------------
		/**
		* @brief UIの追加
		* @param[in] texture 使用するテクスチャ
		*/
		void DeployUi(const wstring& texture); //Ui配置

		/// UIの移動
		void MoveUi();

		/**
		* @brief UIのフェードアウト
		* @param[in,out] UI 対象のUIポインタ
		* @param[in] position UIの位置
		* @param[in] delta デルタ時間
		*/
		void FadeOut(const std::shared_ptr<GameObject>& UI, const Vec3& position, float delta);
		//-----------------------------------------------------------------------------------------------
	};
}
