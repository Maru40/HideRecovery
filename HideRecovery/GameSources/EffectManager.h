
/*!
@file EffectManager.h
@brief EffectManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	namespace Effect {
		struct ParticleData;
	}

	//--------------------------------------------------------------------------------------
	///	エフェクト管理クラス
	//--------------------------------------------------------------------------------------
	class EffectManager : public maru::SingletonComponent<EffectManager>
	{
	public:
		/// <summary>
		/// エフェクトID
		/// </summary>
		enum class ID
		{

		};

	private:
		map<ID, std::function<std::shared_ptr<GameObject>(const Vec3&, const Quat&)>> m_effectMap; //エフェクトマップ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		EffectManager(const std::shared_ptr<GameObject>& objPtr);
		
		void OnCreate() override;

	private:
		/// <summary>
		/// エフェクトマップのセッティング
		/// </summary>
		void SettingEffectMap();

	public:
		/// <summary>
		/// エフェクトの生成
		/// </summary>
		/// <param name="id">生成するエフェクトID</param>
		/// <param name="position">生成する位置</param>
		/// <param name="quat">生成する向き</param>
		/// <returns>生成したエフェクトオブジェクト</returns>
		std::shared_ptr<GameObject> Play(const ID id, const Vec3& position, const Quat& quat = Quat::Identity());

	private:
		/// <summary>
		/// パーティクルの生成
		/// </summary>
		/// <param name="data">生成用のデータ</param>
		/// <param name="position">生成する位置</param>
		/// <param name="quat">生成する向き</param>
		/// <returns>生成したパーティクルオブジェクト</returns>
		std::shared_ptr<GameObject> CreatePrticle(const Effect::ParticleData& data, const Vec3& position, const Quat& quat = Quat::Identity());

		/// <summary>
		/// パーティクルの複数生成
		/// </summary>
		/// <param name="data">生成用のデータ配列</param>
		/// <param name="position">生成する位置</param>
		/// <param name="quat">生成する向き</param>
		/// <returns>生成したパーティクル配列</returns>
		std::shared_ptr<GameObject> CreatePrticles(const std::vector<Effect::ParticleData>& data, const Vec3& position, const Quat& quat = Quat::Identity());

		/// <summary>
		/// エフェクトの生成
		/// </summary>
		/// <param name="position">生成する位置</param>
		/// <param name="quat">生成する向き</param>
		/// <returns>生成したエフェクト</returns>
		template<class T>
		std::shared_ptr<GameObject> CreateEffect(const Vec3& position, const Quat& quat = Quat::Identity()) {
			auto object = GetStage()->Instantiate<GameObject>(position, quat);
			object->AddComponent<T>();

			return object;
		}

	public:
		//--------------------------------------------------------------------------------------
		///	パーティクルの生成用データ取得関数
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// バーニアのVLineを使ったデータを返す。
		/// </summary>
		/// <returns>バーニアのVLineを使ったデータ</returns>
		Effect::ParticleData GetVernierVLineData();

		/// <summary>
		/// バーニアのPointを使ったデータを返す。
		/// </summary>
		/// <returns>バーニアのPointを使ったデータ</returns>
		Effect::ParticleData GetVernierPointData();

		/// <summary>
		/// バーニアに必要なデータ一式を返す。
		/// </summary>
		/// <returns>バーニアに必要なデータ一式</returns>
		std::vector<Effect::ParticleData> GetVernierData();

	};

}