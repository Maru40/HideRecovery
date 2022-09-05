/*!
@file EffectBase.cpp
@brief EffectBaseのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/UI/BuilderVertexPCT.h"
#include "EffectBase.h"

//ボード系
#include "Maruyama/Utility/Component/BillBoard.h"
#include "Maruyama/UI/BuilderVertexPCT.h"
#include "Maruyama/UI/Object/BoardObject.h"

//オーラ系
#include "../Object/AuraObjectBase.h"
#include "AuraBase.h"

#include "MaruUtility.h"

namespace basecross {
	namespace Effect {

		//--------------------------------------------------------------------------------------
		/// オフセットデータ
		//--------------------------------------------------------------------------------------

		OffsetData::OffsetData():
			position(Vec3(0.0f)), rotation(Vec3(0.0f)), scale(Vec3(1.0f))
		{}

		//--------------------------------------------------------------------------------------
		/// エフェクトボード生成用のデータ
		//--------------------------------------------------------------------------------------

		BoardData::BoardData(const Builder::VertexPCTParametor& vertexPCTParam, const Vec3& rotation)
			:vertexPCTParam(vertexPCTParam), rotation(rotation)
		{}

		//--------------------------------------------------------------------------------------
		/// オーラ生成用のデータ
		//--------------------------------------------------------------------------------------

		CreateAuraData::CreateAuraData(const Aura_Parametor& auraParam, const OffsetData offsetData):
			auraParam(auraParam), offsetData(offsetData)
		{}

		//--------------------------------------------------------------------------------------
		/// パーティクル生成用のデータ
		//--------------------------------------------------------------------------------------

		ParticleData::ParticleData(const maru::ParticleManager::Parametor particleParam, const OffsetData& offsetData) :
			particleParam(particleParam), offsetData(offsetData)
		{}

		//--------------------------------------------------------------------------------------
		/// エフェクトの基底クラス
		//--------------------------------------------------------------------------------------

		Base::Base(const std::shared_ptr<GameObject>& objPtr)
			:Component(objPtr)
		{}

		std::shared_ptr<BoardObject> Base::CreateBoardEffect(const BoardData& data) {
			auto effectObject = GetStage()->AddGameObject<BoardObject>(data.vertexPCTParam);
			effectObject->AddComponent<BillBoard>(data.rotation);

			return effectObject;
		}

		std::vector<std::shared_ptr<BoardObject>> Base::CreateBoardEffects(const std::vector<BoardData>& datas) {
			std::vector<std::shared_ptr<BoardObject>> effectObjects;

			for (auto& data : datas) {
				auto effectObject = CreateBoardEffect(data);
				effectObjects.push_back(effectObject);
				AddEffectObject(effectObject);
			}

			return effectObjects;
		}

		std::shared_ptr<AuraObjectBase> Base::CreateAuraObject(const CreateAuraData& data) {
			auto auraObject = GetStage()->AddGameObject<AuraObjectBase>(data.auraParam);
			return auraObject;
		}

		std::vector<std::shared_ptr<AuraObjectBase>> Base::CreateAuraObjects(const std::vector<CreateAuraData>& datas) {
			std::vector<std::shared_ptr<AuraObjectBase>> auraObjects;

			for (auto& data : datas) {
				ex_weak_ptr<AuraObjectBase> auraObject = CreateAuraObject(data);
				auraObjects.push_back(auraObject.GetShard());
				AddAuraObject(auraObject.GetShard(), data.offsetData);
			}

			return auraObjects;
		}

		std::shared_ptr<maru::ParticleManager> Base::CreateParticle(const ParticleData& data) {
			auto particleManager = GetStage()->AddGameObject<GameObject>()->AddComponent<maru::ParticleManager>(data.particleParam);

			return particleManager;
		}

		std::vector<std::shared_ptr<maru::ParticleManager>> Base::CreateParticles(const std::vector<ParticleData>& datas) {
			std::vector<std::shared_ptr<maru::ParticleManager>> particleManagers;

			for (auto& data : datas) {
				auto particleManager = CreateParticle(data);
				particleManagers.push_back(particleManager);
				AddParticleManager(particleManager, data.offsetData);
			}

			return particleManagers;
		}

		void Base::AddEffectObject(const std::shared_ptr<GameObject>& objPtr, const OffsetData& offsetData) {
			m_effectObjects.push_back(objPtr);

			//Parent設定
			objPtr->SetParent(GetGameObject());
			auto objTrans = objPtr->GetComponent<Transform>();
			//ポジションオフセット
			objTrans->SetPosition(offsetData.position);
			//回転オフセット
			auto quat = maru::Utility::ConvertRotationOffset(objPtr, offsetData.rotation);
			objTrans->SetQuaternion(quat);
			//スケールオフセット
			objTrans->SetScale(objTrans->GetScale() * offsetData.scale);
		}

		std::vector<ex_weak_ptr<GameObject>> Base::GetEffectObjects() const {
			return m_effectObjects;
		}

		void Base::AddAuraObject(const std::shared_ptr<AuraObjectBase>& auraObject, const OffsetData& offsetData) {
			m_auraObjects.push_back(auraObject);
			AddEffectObject(auraObject, offsetData);
		}

		std::vector<ex_weak_ptr<AuraObjectBase>> Base::GetAuraObjects() const {
			return m_auraObjects;
		}

		void Base::AddParticleManager(const std::shared_ptr<maru::ParticleManager>& particleMangaer, const OffsetData& offsetData) {
			m_particleManagers.push_back(particleMangaer);
			AddEffectObject(particleMangaer->GetGameObject(), offsetData);
		}

		std::vector<ex_weak_ptr<maru::ParticleManager>> Base::GetParticleManagers() const {
			return m_particleManagers;
		}

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		void Base::SetEffectsDrawActive(const bool isDraw) {
			for (auto& effect : m_effectObjects) {
				effect->SetDrawActive(isDraw);
			}
		}
	}
}