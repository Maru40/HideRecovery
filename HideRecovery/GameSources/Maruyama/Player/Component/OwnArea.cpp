/*!
@file OwnArea.cpp
@brief OwnArea�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "OwnArea.h"

#include "Maruyama/Interface/I_TeamMember.h"
#include "Maruyama/StageObject/Goal.h"

#include "MaruUtility.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// ���w�G���A�̃p�����[�^
	//--------------------------------------------------------------------------------------

	OwnArea_Parametor::OwnArea_Parametor(const team::TeamType& team, const float radius) :
		team(team),
		radius(radius)
	{}

	//--------------------------------------------------------------------------------------
	/// ���w�G���A�{��
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

			//���w�G���A���Ȃ�...
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

			if (teamMember->GetTeam() == GetTeam()) {	//�����`�[�������o�[�Ȃ�
				AddMember(teamMember);
			}
		}
	}

	void OwnArea::CreateMapOutCollisions(const std::shared_ptr<GameObject>& object) {
		auto objectTrans = object->GetComponent<Transform>();
		auto position = objectTrans->GetPosition();
		auto scale = objectTrans->GetScale();
		auto halfScale = scale * 0.5f;

		constexpr float Width = 0.5f;
		//���s����
		auto forwardPosition = position + (Vec3::Forward() * halfScale.z);
		CreateMapOutCollision(forwardPosition, Vec3::Forward(), scale.x, Width);

		//��O������
		auto backPosition = position + (-Vec3::Forward() * halfScale.z);
		CreateMapOutCollision(backPosition, -Vec3::Forward(), scale.x, Width);

		//�E��
		auto rightPosition = position + (Vec3::Right() * halfScale.x);
		CreateMapOutCollision(rightPosition, Vec3::Right(), scale.z, Width);

		//����
		auto leftPosition = position + (-Vec3::Right() * halfScale.x);
		CreateMapOutCollision(leftPosition, -Vec3::Right(), scale.z, Width);
	}

	void OwnArea::CreateMapOutCollision(const Vec3& startPosition, const Vec3& forward, const float& length, const float& width, const float& height) {
		const float MoveForwardRange = width * 1.5f;
		auto halfHeight = height * 0.5f;
		auto direct = maru::Utility::ConvertForwardOffset(forward, Vec3::Right());
		auto position = startPosition + (direct.GetNormalized() * MoveForwardRange);
		position.y += halfHeight;
		auto object = GetStage()->Instantiate<GameObject>(position, Quat::Identity());
		auto collision = object->AddComponent<CollisionObb>();
		//object->AddTag(L"T_Obstacle");

		constexpr float depth = 1.0f;
		auto objectTrans = object->GetComponent<Transform>();
		objectTrans->SetScale(Vec3(length - width, height, depth));
		objectTrans->SetForward(forward);

		collision->SetFixed(true);
		//collision->SetDrawActive(true);

		m_outCollisonObject.push_back(object);
	}

	void OwnArea::OnCollisionEnter(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock()->GetGameObject();
		auto teamMember = other->GetComponent<I_TeamMember>(false);
		if(!teamMember){
			return;
		}

		if (teamMember->GetTeam() == GetTeam()) {	//�����`�[�������o�[�Ȃ�
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

		if (teamMember->GetTeam() == GetTeam()) {	//�����`�[�������o�[�Ȃ�
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
	/// �A�N�Z�b�T
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