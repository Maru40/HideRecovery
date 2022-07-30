/*!
@file WaterDraw.h
@brief WaterDrawなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "BulletBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class LaserBaseDraw;
	struct ParticleColliderManager_Parametor;
	
	namespace maru {
		class ParticleManager;
	}

	//--------------------------------------------------------------------------------------
	/// 水圧ジェット描画パラメータ
	//--------------------------------------------------------------------------------------
	struct WaterDraw_Parametor
	{
		std::shared_ptr<ParticleColliderManager_Parametor> particleCollisionManagerParam; //パーティクルマネージャのパラメータ

		WaterDraw_Parametor();
	};

	//--------------------------------------------------------------------------------------
	/// 水圧ジェット描画
	//--------------------------------------------------------------------------------------
	class WaterDraw : public BulletBase
	{
	public:
		using Parametor = WaterDraw_Parametor;

	private:
		Parametor m_param;                                    //パラメータ

		ex_weak_ptr<GameObject> m_body;                       //本体
		ex_weak_ptr<GameObject> m_boardAura;                  //オーラ
		ex_weak_ptr<maru::ParticleManager> m_particleManager; //パーティクルマネージャ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスが所有するゲームオブジェクト</param>
		WaterDraw(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;

	private:
		/// <summary>
		/// パーティクルマネージャ―の生成
		/// </summary>
		void CreatePartilceManager();

		/// <summary>
		/// 飛ばす処理
		/// </summary>
		/// <param name="direct">飛ばす方向</param>
		void Shot(const Vec3& direct) override;

		/// <summary>
		/// 飛ばす処理の終了時に呼ぶ処理
		/// </summary>
		void ShotEnd() override;

		/// <summary>
		/// Bodyの位置を設定する。
		/// </summary>
		void SetBodyPosition();

		/// <summary>
		/// パーティクルコリジョンの長さを設定する
		/// </summary>
		void SetParticleCollisionLength();

	};

}