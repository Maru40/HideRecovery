/*!
@file ParticleManager.h
@brief ParticleManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Utility/SingletonComponent/HitStopManager.h"

#include "Maruyama/Utility/ReactiveBool.h"
#include "Maruyama/UI/BuilderVertexPCT.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class BoardObject;
	class GameTimer;

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// パーティクル管理のパラメータ
		//--------------------------------------------------------------------------------------
		struct ParticleManager_Parametor
		{
			Builder::VertexPCTParametor vertexPCTParam;    //パーティクルに使いたい板ポリ用のデータ
			Vec2 powerRandomVec;                           //パワーのランダム幅
			Vec3 shotDirectRandomVec;                      //力を加える方向のランダム幅
			Vec2 sizeRandomVec;                            //板ポリの大きさのランダム幅
			float liveTime = 1.0f;                         //生成してから削除されるまでの時間
			float intervalTime = 0.0f;                     //ループ生成時のインターバルタイム
			Vec3 gravityScale = Vec3(0.0f, -9.8f, 0.0f);   //重力の大きさ  
			uint16_t numOneFrameCreate;                    //1フレームに生成する量
			bool isLoop = true;                            //ループするかどうか
			bool isBillBoard = false;                      //ビルボードさせるかどうか
			bool isForwardRotation = false;                //Shot方向に回転するかどうか
			bool isAlphaFade = false;                      //透明化フェードをさせるかどうか
			bool isGravity = false;                        //重力をつけるかどうか
			bool isRayHitDestory = false;                  //RayHitで壁にぶつかった時の削除をするかどうか
			ex_weak_ptr<GameObject> parent = nullptr;      //ペアレント設定

			/// <summary>
			/// コンストラクタ
			/// </summary>
			ParticleManager_Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="vertexPCTParam">パーティクルに使いたい板ポリ用のデータ</param>
			ParticleManager_Parametor(const Builder::VertexPCTParametor& vertexPCTParam);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="vertexPCTParam">パーティクルに使いたい板ポリ用のデータ</param>
			/// <param name="powerRandomVec">パワーのランダム幅</param>
			/// <param name="shotDirectRandomVec">力を加える方向のランダム幅</param>
			/// <param name="scaleRandomVec">板ポリの大きさのランダム幅</param>
			/// <param name="liveTime">生成してから削除されるまでの時間</param>
			/// <param name="intervalTime">ループ生成時のインターバルタイム</param>
			/// <param name="numOneFrameCreate">1フレームに生成する量</param>
			/// <param name="isLoop">ループするかどうか</param>
			ParticleManager_Parametor(
				const Builder::VertexPCTParametor& vertexPCTParam,
				const Vec2& powerRandomVec,
				const Vec3& shotDirectRandomVec,
				const Vec2& scaleRandomVec,
				const float& liveTime,
				const float& intervalTime,
				const uint16_t numOneFrameCreate,
				const bool isLoop
			);
		};

		//--------------------------------------------------------------------------------------
		/// パーティクル管理
		//--------------------------------------------------------------------------------------
		class ParticleManager : public Component
		{
		public:
			using Parametor = ParticleManager_Parametor;

		private:
			Parametor m_param;
			uint16_t m_loopCount = 0;                          //ループカウント

			std::vector<ex_weak_ptr<BoardObject>> m_particels; //生成したパーティグル群

			std::unique_ptr<GameTimer> m_timer = nullptr;      //タイマー管理

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			ParticleManager(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="parametor">パラメータ</param>
			ParticleManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

			void OnCreate() override;
			void OnUpdate() override;

			void OnDrawActive() override;

			void OnDestroy() override;

		private:
			/// <summary>
			/// 更新処理
			/// </summary>
			void UpdateProcess();

			/// <summary>
			/// ループ更新
			/// </summary>
			void LoopUpdate();

			/// <summary>
			/// パーティクルの更新
			/// </summary>
			void ParticlesUpdate();

			/// <summary>
			/// パーティクルの透明度更新
			/// </summary>
			void AlphaFadeUpdate();

			/// <summary>
			/// パーティクルの生成
			/// </summary>
			void CreateParticle();

			/// <summary>
			/// パーティクルを複数生成
			/// </summary>
			void CreateParticles();

			/// <summary>
			/// RayHit時に消滅する場合にする処理
			/// </summary>
			/// <param name="particle">生成したパーティクル</param>
			void SettingRayHitDestory(const std::shared_ptr<BoardObject>& particle);

			/// <summary>
			/// パーティクルを飛ばす処理
			/// </summary>
			/// <param name="particle">生成したパーティクル</param>
			void ShotParticle(const std::shared_ptr<BoardObject>& particle);

			/// <summary>
			/// パーティクルの飛ばす方向を計算
			/// </summary>
			/// <returns>パーティクルを飛ばす方向</returns>
			Vec3 CalculateShotDirect();

		public:
			/// <summary>
			/// 生成したパーティクル配列の取得
			/// </summary>
			/// <returns>生成したパーティクル配列</returns>
			std::vector<ex_weak_ptr<BoardObject>> GetParticles() const;

		};

	}
}