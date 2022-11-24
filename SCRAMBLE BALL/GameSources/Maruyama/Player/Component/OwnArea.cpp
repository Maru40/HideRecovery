/*!
@file OwnArea.cpp
@brief OwnAreaクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "OwnArea.h"

#include "Maruyama/Interface/I_TeamMember.h"
#include "Maruyama/StageObject/Goal.h"

#include "Maruyama/Utility/Utility.h"

#include "Watanabe/Shader/BarrierShader.h"
#include "Watanabe/Component/DissolveAnimator.h"
#include "Watanabe/Utility/AdvMeshUtil.h"
#include "MainStage.h"
#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// 自陣エリアのパラメータ
	//--------------------------------------------------------------------------------------

	OwnArea_Parametor::OwnArea_Parametor(const team::TeamType& team, const float radius) :
		team(team),
		radius(radius)
	{}

	//--------------------------------------------------------------------------------------
	/// 自陣エリア本体
	//--------------------------------------------------------------------------------------

	OwnArea::OwnArea(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor) :
		Component(objPtr),
		m_param(parametor)
	{}

	void OwnArea::OnCreate() {
		auto& rect = m_param.rect;
		auto scale = transform->GetScale();

		rect.centerPosition = transform->GetPosition();
		rect.width = scale.x;
		rect.depth = scale.z;

		// MainStageのみ作成
		if (GetGameObject()->GetTypeStage<MainStage>(false))
			CreateMapOutCollisions(GetGameObject());
	}

	void OwnArea::OnLateStart() {
	}

	void OwnArea::OnUpdate() {
		if (m_members.size() == 0) {
			SearchPlayers();
		}

		for (auto& weakMember : m_members) {
			auto member = weakMember.lock();
			if (!member) {
				continue;
			}

			//自陣エリア内なら...
			member->SetIsInArea(IsInArea(member));
		}
	}

	void OwnArea::SearchPlayers() {
		auto players = maru::Utility::FindGameObjects<PlayerObject>(GetStage());
		for (auto& player : players) {
			auto teamMember = player->GetComponent<I_TeamMember>();
			if (!teamMember) {
				continue;
			}

			if (teamMember->GetTeam() == GetTeam()) {	//同じチームメンバーなら
				AddMember(teamMember);
			}
		}
	}

	void OwnArea::CreateMapOutCollisions(const std::shared_ptr<GameObject>& object) {
		auto objectTrans = object->GetComponent<Transform>();
		auto position = objectTrans->GetPosition();
		auto scale = objectTrans->GetScale();
		auto halfScale = scale * 0.5f;

		//奥行生成
		auto forwardPosition = position + (Vec3::Forward() * halfScale.z);
		CreateMapOutCollision(forwardPosition, Vec3::Forward(), scale.x);

		//手前側生成
		auto backPosition = position + (-Vec3::Forward() * halfScale.z);
		CreateMapOutCollision(backPosition, -Vec3::Forward(), scale.x);

		//右側
		auto rightPosition = position + (Vec3::Right() * halfScale.x);
		CreateMapOutCollision(rightPosition, Vec3::Right(), scale.z);

		//左側
		auto leftPosition = position + (-Vec3::Right() * halfScale.x);
		CreateMapOutCollision(leftPosition, -Vec3::Right(), scale.z);
	}

	void OwnArea::CreateMapOutCollision(const Vec3& startPosition, const Vec3& forward, const float& width, const float& height, const float& depth) {
		const float halfHeight = height * 0.5f;
		const float halfDepth = depth * 0.5f;

		// 指定位置からhalfDepth分外側へ
		auto position = startPosition + (forward.GetNormalized() * halfDepth);
		position.y += halfHeight;

		auto object = GetStage()->Instantiate<GameObject>(position, Quat::Identity());
		auto collision = object->AddComponent<CollisionObb>();
		collision->SetFixed(true);

		auto objectTrans = object->GetComponent<Transform>();
		objectTrans->SetScale(Vec3(width, height, depth));
		objectTrans->SetForward(forward);

		{
			float _height = height * 0.8f;
			// 場所により壁と重なる箇所があるため、内側に少し移動させる
			auto position = startPosition - (forward.GetNormalized() * halfDepth * 0.1f);
			position.y += _height * 0.5f;

			auto planeObj = GetStage()->Instantiate<GameObject>(position, Quat::Identity());
			auto objectTrans = planeObj->GetComponent<Transform>();
			objectTrans->SetScale(Vec3(width, _height, 1));
			objectTrans->SetForward(forward);

			auto drawComp = planeObj->AddComponent<BarrierShader>();
			Vec3 normal(0, 0, 1);
			Vec2 halfSize = Vec2(1) * 0.5f;
			// 頂点のデータ (番号は左上から右下まで)
			vector<VertexPositionNormalTexture> vertices = {
				{Vec3(-halfSize.x,+halfSize.y,0.0f),normal,Vec2(0,0)}, //0
				{Vec3(+halfSize.x,+halfSize.y,0.0f),normal,Vec2(1,0)}, //1

				{Vec3(-halfSize.x,-halfSize.y,0.0f),normal,Vec2(0,1)}, //2
				{Vec3(+halfSize.x,-halfSize.y,0.0f),normal,Vec2(1,1)},  //3
			};

			// 頂点インデックス
			vector<uint16_t> indices = {
				0, 1, 2, // 上の三角形
				2, 1, 3  // 下の三角形
			};
			auto meshData = MeshResource::CreateMeshResource(vertices, indices, true);
			drawComp->SetOriginalMeshResource(meshData);
			drawComp->SetOriginalMeshUse(true);
			auto teamColor = team::GetTeamColor(m_param.team);
			drawComp->SetDiffuse(teamColor);

			auto dissolveAnimator = planeObj->AddComponent<DissolveAnimator>();
			dissolveAnimator->SetPlayLength(0.5f);

			planeObj->SetAlphaActive(true);

			m_dissolveAnimators.push_back(planeObj);
		}

		m_outCollisonObject.push_back(object);
	}

	void OwnArea::SetOutCollisionActive(const bool isActive) {
		for (auto object : m_outCollisonObject) {
			auto _object = object.lock();
			if (!_object)
				continue;
			_object->SetActive(isActive);
		}

		for (auto& animator : m_dissolveAnimators) {
			auto _animator = animator.lock();
			if (!_animator)
				continue;

			if (auto dissolveAnimator = _animator->GetComponent<DissolveAnimator>(false)) {
				// 非表示にする場合のみアニメーション
				if (!isActive) {
					dissolveAnimator->SetPlayEndEvent(
						[_animator]() {
							// 再生終了時に非アクティブ
							_animator->SetActive(false);
						}
					);
					dissolveAnimator->Start();
				}
				else {
					dissolveAnimator->Reset();
					_animator->SetActive(isActive);
				}
			}
		}
	}

	void OwnArea::OnCollisionEnter(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock()->GetGameObject();
		auto teamMember = other->GetComponent<I_TeamMember>(false);
		if (!teamMember) {
			return;
		}

		if (teamMember->GetTeam() == GetTeam()) {	//同じチームメンバーなら
			teamMember->SetIsInArea(true);
			Debug::GetInstance()->Log(L"InArea");
		}
	}

	void OwnArea::OnCollisionExit(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock()->GetGameObject();
		auto teamMember = other->GetComponent<I_TeamMember>(false);
		if (!teamMember) {
			return;
		}

		if (teamMember->GetTeam() == GetTeam()) {	//同じチームメンバーなら
			teamMember->SetIsInArea(false);
			Debug::GetInstance()->Log(L"OutArea");
		}
	}

	bool OwnArea::IsInArea(const std::shared_ptr<I_TeamMember>& member) {
		return IsInArea(member->GetOwnerObject());
	}

	bool OwnArea::IsInArea(const std::shared_ptr<GameObject>& member) {
		auto memberTrans = member->GetComponent<Transform>();
		auto position = memberTrans->GetPosition();

		return m_param.rect.IsInRect(position);
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void OwnArea::AddMember(const std::shared_ptr<I_TeamMember>& member) {
		member->SetOwnArea(GetThis<OwnArea>());
		m_members.push_back(member);
	}

	bool OwnArea::RemoveMember(const std::shared_ptr<I_TeamMember>& member) {
		auto iter = m_members.begin();
		while (iter != m_members.end()) {
			if ((*iter).lock() == member) {
				iter = m_members.erase(iter);
				return true;
			}
			iter++;
		}

		return false;
	}
}