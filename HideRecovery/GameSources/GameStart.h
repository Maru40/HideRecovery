/*!
@file GameStart.h
@brief 開始演出
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//-----------------------------------------------------------------------------------------------
	/// ゲームスタート時の演出クラス
	//-----------------------------------------------------------------------------------------------
	class GameStart : public GameObject
	{
		float m_ReadyMaxPosY = 500.0f; //! Readyの終点位置
		float m_ReadyMoveSpeed = 500.0f; //! Readyの動く速さ

		Vec3 m_GoMaxScale = Vec3(2.0f, 2.0f, 1.0f); //! Goの最大の大きさ
		float m_GoGrowthSpeed = 12.0f; //! Goの大きくなる速さ

		std::shared_ptr<GameObject> m_Ready; //! Readyの板ポリ
		std::shared_ptr<GameObject> m_Go; //! Goの板ポリ
		std::shared_ptr<Transform> m_ReadyTrans; //! Readyのトランスフォーム
		std::shared_ptr<Transform> m_GoTrans; //! Goのトランスフォーム
		std::shared_ptr<Component> m_TimerComp; //! Goのトランスフォーム

	public:
		//-----------------------------------------------------------------------------------------------
		/// コンストラクタ
		GameStart::GameStart(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}
		//-----------------------------------------------------------------------------------------------
		void OnCreate() override;
		void OnUpdate() override;
		//-----------------------------------------------------------------------------------------------
		/// Readyを動かす処理
		void Ready();

		/// Goを動かす処理
		void Go();

		/**
		* @brief スタート演出の終了処理
		* @datalis 停止していたオブジェクトを動かし自身を消去
		*/
		void End();
		//-----------------------------------------------------------------------------------------------
	};
}
