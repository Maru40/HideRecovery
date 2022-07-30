/*!
@file DamageEffect.cpp
@brief DamageEffectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EffectBase.h"
#include "DamageEffect.h"

#include "BuilderVertexPCT.h"
#include "BoardObject.h"

#include "BillBoard.h"

#include "ParticleManager.h"

namespace basecross {

	namespace Effect {

		Damage::Damage(const std::shared_ptr<GameObject>& objPtr)
			:Base(objPtr)
		{}
		
		void Damage::OnCreate() {
			Base::OnCreate();

			//データの生成
			std::vector<ParticleData> datas = {
				CreateLineVData(),
				CretaePointData(),
			};

			CreateParticles(datas);
		}

		ParticleData Damage::CreateLineVData() {
			//バーテックスパラメータの生成
			auto vertexPCTParam = Builder::VertexPCTParametor(Vec3(0.25f, 0.0f, 0.25f), 1.0f, Col4(1.0f, 0.0f, 0.0f, 1.0f), Vec2(512.0f), L"LineV_TX");

			//パーティクルパラメータの生成
			auto particleParam = maru::ParticleManager::Parametor(vertexPCTParam);
			particleParam.powerRandomVec = Vec2(5.0f, 10.0f);                  //ランダム進行力の幅
			particleParam.shotDirectRandomVec = Vec3(360.0f, 360.0f, 360.0f);  //ランダム進行方向の幅
			particleParam.sizeRandomVec = Vec2(1.5f, 2.5f);                    //ランダムサイズの幅
			particleParam.liveTime = 0.6f;           //パーティクルの生存時間
			particleParam.numOneFrameCreate = 25;    //1フレームに生成する数
			particleParam.isLoop = false;            //ループするかどうか
			particleParam.isBillBoard = false;       //ビルボードするかどうか
			particleParam.isForwardRotation = true;  //Shot方向にForwardを向ける。
			particleParam.isAlphaFade = true;        //AlphaFadeをするかどうか

			return ParticleData(particleParam);
		}

		ParticleData Damage::CretaePointData() {
			//バーテックスパラメータの生成
			auto vertexPCTParam = Builder::VertexPCTParametor(Vec3(0.25f, 0.0f, 0.25f), 1.0f, Col4(1.0f, 0.8f, 0.0f, 1.0f), Vec2(512.0f), L"LineV_TX");

			//パーティクルパラメータの生成
			auto particleParam = maru::ParticleManager::Parametor(vertexPCTParam);
			particleParam.powerRandomVec = Vec2(0.5f, 3.0f);                    //ランダム進行力の幅
			particleParam.shotDirectRandomVec = Vec3(360.0f, 360.0f, 360.0f);   //ランダム進行方向の幅
			particleParam.sizeRandomVec = Vec2(1.0f, 2.0f);                     //ランダムサイズの幅
			particleParam.liveTime = 0.6f;            //パーティクルの生存時間
			particleParam.numOneFrameCreate = 40;     //1フレームに生成する数
			particleParam.isLoop = false;             //ループするかどうか
			particleParam.isBillBoard = false;        //ビルボードするかどうか
			particleParam.isForwardRotation = true;   //Shot方向にForwardを向ける。
			particleParam.isAlphaFade = true;         //AlphaFadeをするかどうか

			return ParticleData(particleParam);
		}
	}

}