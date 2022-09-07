/*!
@file EffectBase.h
@brief EffectBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/UI/BuilderVertexPCT.h"
#include "AuraBase.h"

#include "ParticleManager.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class BoardObject;

	namespace Effect {
		class AuraObjectBase;
		struct Aura_Parametor;

		//--------------------------------------------------------------------------------------
		/// オフセットデータ
		//--------------------------------------------------------------------------------------
		struct OffsetData {
			Vec3 position;  //位置
			Vec3 rotation;  //回転
			Vec3 scale;     //大きさ

			OffsetData();
		};

		//--------------------------------------------------------------------------------------
		/// エフェクトボード生成用のデータ
		//--------------------------------------------------------------------------------------
		struct BoardData {
			Builder::VertexPCTParametor vertexPCTParam;
			Vec3 rotation;

			BoardData(const Builder::VertexPCTParametor& vertexPCTParam, const Vec3& rotation);
		};

		//--------------------------------------------------------------------------------------
		/// オーラ生成用のデータ
		//--------------------------------------------------------------------------------------
		struct CreateAuraData
		{
			Aura_Parametor auraParam;
			OffsetData offsetData;

			CreateAuraData(const Aura_Parametor& auraParam, const OffsetData offsetData = OffsetData());
		};

		//--------------------------------------------------------------------------------------
		/// パーティクル生成用のデータ
		//--------------------------------------------------------------------------------------
		struct ParticleData
		{
			maru::ParticleManager::Parametor particleParam;
			OffsetData offsetData;

			ParticleData(const maru::ParticleManager::Parametor particleParam, const OffsetData& offsetData = OffsetData());
		};

		//--------------------------------------------------------------------------------------
		/// エフェクトの基底クラス
		//--------------------------------------------------------------------------------------
		class Base : public Component
		{
		private:
			std::vector<ex_weak_ptr<GameObject>> m_effectObjects;                //全てのエフェクトオブジェクト
			std::vector<ex_weak_ptr<BoardObject>> m_boardObjects;                //ボードオブジェクト
			std::vector<ex_weak_ptr<AuraObjectBase>> m_auraObjects;              //オーラオブジェクト
			std::vector<ex_weak_ptr<maru::ParticleManager>> m_particleManagers;  //パーティクルマネージャ―

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			Base(const std::shared_ptr<GameObject>& objPtr);

			virtual void OnCreate() override {};

		public:
			/// <summary>
			/// ボードエフェクト生成
			/// </summary>
			/// <param name="data">ボードデータ</param>
			/// <returns>生成したボード</returns>
			std::shared_ptr<BoardObject> CreateBoardEffect(const BoardData& data);

			/// <summary>
			/// ボードエフェクトの複数生成
			/// </summary>
			/// <param name="datas">ボードデータ配列</param>
			/// <returns>生成した全てのボード</returns>
			std::vector<std::shared_ptr<BoardObject>> CreateBoardEffects(const std::vector<BoardData>& datas);

			/// <summary>
			/// オーラエフェクト生成
			/// </summary>
			/// <param name="data">オーラエフェクトデータ</param>
			/// <returns>生成したオーラエフェクト</returns>
			std::shared_ptr<AuraObjectBase> CreateAuraObject(const CreateAuraData& data);

			/// <summary>
			/// オーラエフェクトの複数生成
			/// </summary>
			/// <param name="datas">オーラエフェクトのデータ配列</param>
			/// <returns>生成した全てのオーラオブジェクト</returns>
			std::vector<std::shared_ptr<AuraObjectBase>> CreateAuraObjects(const std::vector<CreateAuraData>& datas);

			/// <summary>
			/// パーティクル生成
			/// </summary>
			/// <param name="data">パーティクルデータ</param>
			/// <returns>生成したパーティクル</returns>
			std::shared_ptr<maru::ParticleManager> CreateParticle(const ParticleData& data);

			/// <summary>
			/// パーティクルの複数生成
			/// </summary>
			/// <param name="datas">パーティクルのデータ配列</param>
			/// <returns>生成した全てのパーティクル</returns>
			std::vector<std::shared_ptr<maru::ParticleManager>> CreateParticles(const std::vector<ParticleData>& datas);

		protected:
			/// <summary>
			/// エフェクトオブジェクトの追加
			/// </summary>
			/// <param name="objPtr">追加するオブジェクト</param>
			/// <param name="offsetData">オフセットデータ</param>
			void AddEffectObject(const std::shared_ptr<GameObject>& objPtr, const OffsetData& offsetData = OffsetData());

			/// <summary>
			/// エフェクトオブジェクトの配列を取得
			/// </summary>
			/// <returns>エフェクトオブジェクトの配列</returns>
			std::vector<ex_weak_ptr<GameObject>> GetEffectObjects() const;

			/// <summary>
			/// オーラオブジェクトの追加
			/// </summary>
			/// <param name="auraObject">追加するオーラオブジェクト</param>
			/// <param name="offsetData">オフセットデータ</param>
			void AddAuraObject(const std::shared_ptr<AuraObjectBase>& auraObject, const OffsetData& offsetData);

			/// <summary>
			/// オーラオブジェクトの配列を取得
			/// </summary>
			/// <returns>オーラオブジェクトの配列</returns>
			std::vector<ex_weak_ptr<AuraObjectBase>> GetAuraObjects() const;

			/// <summary>
			/// パーティクルマネージャーの追加
			/// </summary>
			/// <param name="particleManager">パーティクルマネージャー</param>
			/// <param name="offsetData">オフセットデータ</param>
			void AddParticleManager(const std::shared_ptr<maru::ParticleManager>& particleManager, const OffsetData& offsetData);

			/// <summary>
			/// パーティクルマネージャーの取得
			/// </summary>
			/// <returns>パーティクルマネージャー</returns>
			std::vector<ex_weak_ptr<maru::ParticleManager>> GetParticleManagers() const;

		public:
			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// エフェクトの描画をするか設定
			/// </summary>
			/// <param name="isDraw">エフェクトを描画するかどうか</param>
			void SetEffectsDrawActive(const bool isDraw);
		};

	}
}