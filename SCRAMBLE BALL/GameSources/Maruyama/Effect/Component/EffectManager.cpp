
/*!
@file EffectManager.cpp
@brief EffectManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "EffectManager.h"

#include "EffectBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	パーティクル生成用の前方宣言関数
	//--------------------------------------------------------------------------------------

	/// <summary>
	/// スライムが潰れた時のパーティクルデータ
	/// </summary>
	Effect::ParticleData GetSlimeCrushData();
	/// <summary>
	/// ジェットスパークデータ
	/// </summary>
	Effect::ParticleData GetJetSpakeData();


	//--------------------------------------------------------------------------------------
	///	エフェクト管理クラス
	//--------------------------------------------------------------------------------------

	std::weak_ptr<EffectManager>  maru::SingletonComponent<EffectManager>::sm_instance;

	EffectManager::EffectManager(const std::shared_ptr<GameObject>& objPtr)
		:SingletonComponent(objPtr)
	{}

	void EffectManager::OnCreate() {
		SettingEffectMap();
	}

	void EffectManager::SettingEffectMap() {
		//m_effectMap[ID::TackleDamage] = [&](const Vec3& position, const Quat& quat) { return CreateEffect<Effect::Damage>(position, quat); };
		//m_effectMap[ID::JetSpake] = [&](const Vec3& position, const Quat& quat) { return CreatePrticle(GetJetSpakeData(), position, quat); };
		//m_effectMap[ID::Vernier] = [&](const Vec3& position, const Quat& quat) { return CreatePrticles(GetVernierData(), position, quat); };
		//m_effectMap[ID::SlimeCrush] = [&](const Vec3& position, const Quat& quat) { return CreatePrticle(GetSlimeCrushData(), position, quat); };
	}

	std::shared_ptr<GameObject> EffectManager::Play(const ID id, const Vec3& position, const Quat& quat) {
		if (!m_effectMap.count(id)) {  //particleが存在しないなら
			DebugObject::AddString(L"idが存在しません");
			return nullptr;
		}

		auto& func = m_effectMap[id];
		return func ? func(position, quat) : nullptr;
	}


	std::shared_ptr<GameObject> EffectManager::CreatePrticle(const Effect::ParticleData& data, const Vec3& position, const Quat& quat) {
		auto object = GetStage()->Instantiate<GameObject>(position, quat);
		auto particleManager = object->AddComponent<maru::ParticleManager>(data.particleParam);

		return object;
	}

	std::shared_ptr<GameObject> EffectManager::CreatePrticles(const std::vector<Effect::ParticleData>& data, const Vec3& position, const Quat& quat) {
		auto object = GetStage()->Instantiate<GameObject>(position, quat);
		auto effect = object->AddComponent<Effect::Base>();
		effect->CreateParticles(data);

		return object;
	}


	//--------------------------------------------------------------------------------------
	///	パーティクルの生成用データ取得関数
	//--------------------------------------------------------------------------------------

	//スライムが潰れるときのパーティクルデータ----------------------------------------------

	Effect::ParticleData GetSlimeCrushData() {
		//バーテックスパラメータの生成
		auto vertexPCTParam =
			Builder::VertexPCTParametor(Vec3(0.5f, 0.5f, 0.0f), 1.0f, Col4(0.1f, 0.9f, 0.1f, 1.0f), Vec2(512.0f), L"Point_TX");

		//パーティクルパラメータの生成
		auto particleParam = maru::ParticleManager::Parametor(vertexPCTParam);
		particleParam.powerRandomVec = Vec2(2.0f, 2.0f);                   //ランダム進行力の幅
		particleParam.shotDirectRandomVec = Vec3(360.0f, 360.0f, 360.0f);      //ランダム進行方向の幅
		particleParam.sizeRandomVec = Vec2(0.5f, 1.0f);                    //ランダムサイズの幅
		particleParam.liveTime = 0.5f;            //パーティクルの生存時間
		particleParam.numOneFrameCreate = 20;     //1フレームに生成する数
		particleParam.isLoop = false;             //ループするかどうか
		particleParam.isBillBoard = true;         //ビルボードするかどうか
		particleParam.isForwardRotation = false;  //Shot方向にForwardを向ける。
		particleParam.isAlphaFade = true;         //AlphaFadeをするかどうか

		return Effect::ParticleData(particleParam);
	}

	//--------------------------------------------------------------------------------------

	//ジェットスパークデータ----------------------------------------------------------------

	Effect::ParticleData GetJetSpakeData() {
		//バーテックスパラメータの生成
		auto vertexPCTParam = 
			Builder::VertexPCTParametor(Vec3(0.25f, 0.0f, 0.25f), 1.0f, Col4(1.0f, 0.5f, 0.5f, 1.0f), Vec2(512.0f), L"LineV_TX");

		//パーティクルパラメータの生成
		auto particleParam = maru::ParticleManager::Parametor(vertexPCTParam);
		particleParam.powerRandomVec = Vec2(2.0f, 6.0f);                  //ランダム進行力の幅
		particleParam.shotDirectRandomVec = Vec3(45.0f, 0.0f, 45.0f);  //ランダム進行方向の幅
		particleParam.sizeRandomVec = Vec2(1.0f, 1.0f);                    //ランダムサイズの幅
		particleParam.liveTime = 1.0f;           //パーティクルの生存時間
		particleParam.numOneFrameCreate = 40;    //1フレームに生成する数
		particleParam.isLoop = false;            //ループするかどうか
		particleParam.isBillBoard = false;       //ビルボードするかどうか
		particleParam.isForwardRotation = true;  //Shot方向にForwardを向ける。
		particleParam.isAlphaFade = true;        //AlphaFadeをするかどうか

		return Effect::ParticleData(particleParam);
	}

	//--------------------------------------------------------------------------------------

	//バーニアデータ------------------------------------------------------------------------

	Effect::ParticleData EffectManager::GetVernierVLineData() {
		//バーテックスパラメータの生成
		auto vertexPCTParam =
			Builder::VertexPCTParametor(Vec3(0.25f, 0.0f, 0.25f), 1.0f, Col4(0.0f, 0.5f, 1.0f, 1.0f), Vec2(512.0f), L"LineV_TX");

		//パーティクルパラメータの生成
		auto particleParam = maru::ParticleManager::Parametor(vertexPCTParam);
		particleParam.powerRandomVec = Vec2(2.0f, 5.0f);                  //ランダム進行力の幅
		particleParam.shotDirectRandomVec = Vec3(45.0f, 0.0f, 45.0f);  //ランダム進行方向の幅
		particleParam.sizeRandomVec = Vec2(5.0f, 5.0f);                    //ランダムサイズの幅
		particleParam.liveTime = 0.5f;           //パーティクルの生存時間
		particleParam.numOneFrameCreate = 40;    //1フレームに生成する数
		particleParam.isLoop = false;            //ループするかどうか
		particleParam.isBillBoard = false;       //ビルボードするかどうか
		particleParam.isForwardRotation = true;  //Shot方向にForwardを向ける。
		particleParam.isAlphaFade = true;        //AlphaFadeをするかどうか
		particleParam.isGravity = false;         //重力を使用するかどうか
		particleParam.gravityScale = Vec3(0.0f, -9.8f, 0.0f);  //重力の大きさ

		return Effect::ParticleData(particleParam);
	}

	Effect::ParticleData EffectManager::GetVernierPointData() {
		//バーテックスパラメータの生成
		auto vertexPCTParam = 
			Builder::VertexPCTParametor(Vec3(0.5f, 0.5f, 0.0f), 1.0f, Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(512.0f), L"Point_TX");

		//パーティクルパラメータの生成
		auto particleParam = maru::ParticleManager::Parametor(vertexPCTParam);
		particleParam.powerRandomVec = Vec2(0.3f, 0.3f);                   //ランダム進行力の幅
		particleParam.shotDirectRandomVec = Vec3(15.0f, 0.0f, 15.0f);      //ランダム進行方向の幅
		particleParam.sizeRandomVec = Vec2(0.3f, 0.7f);                    //ランダムサイズの幅
		particleParam.liveTime = 0.5f;            //パーティクルの生存時間
		particleParam.numOneFrameCreate = 20;     //1フレームに生成する数
		particleParam.isLoop = false;             //ループするかどうか
		particleParam.isBillBoard = true;         //ビルボードするかどうか
		particleParam.isForwardRotation = false;  //Shot方向にForwardを向ける。
		particleParam.isAlphaFade = true;         //AlphaFadeをするかどうか

		return Effect::ParticleData(particleParam);
	}

	std::vector<Effect::ParticleData> EffectManager::GetVernierData() {
		std::vector<Effect::ParticleData> datas = {
			GetVernierVLineData(),
			GetVernierPointData(),
		};

		return datas;
	}

	//--------------------------------------------------------------------------------------
}