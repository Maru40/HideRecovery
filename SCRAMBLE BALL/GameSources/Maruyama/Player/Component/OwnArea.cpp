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

#include "Maruyama/Utility/Utility.h"

#include "Watanabe/Shader/StaticModelDraw.h"
#include "Watanabe/Component/DissolveAnimator.h"
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

		//���s����
		auto forwardPosition = position + (Vec3::Forward() * halfScale.z);
		CreateMapOutCollision(forwardPosition, Vec3::Forward(), scale.x);

		//��O������
		auto backPosition = position + (-Vec3::Forward() * halfScale.z);
		CreateMapOutCollision(backPosition, -Vec3::Forward(), scale.x);

		//�E��
		auto rightPosition = position + (Vec3::Right() * halfScale.x);
		CreateMapOutCollision(rightPosition, Vec3::Right(), scale.z);

		//����
		auto leftPosition = position + (-Vec3::Right() * halfScale.x);
		CreateMapOutCollision(leftPosition, -Vec3::Right(), scale.z);
	}

	void OwnArea::CreateMapOutCollision(const Vec3& startPosition, const Vec3& forward, const float& width, const float& height, const float& depth) {
		const float halfHeight = height * 0.5f;
		const float halfDepth = depth * 0.5f;

		// �w��ʒu����halfDepth���O����
		auto position = startPosition + (forward.GetNormalized() * halfDepth);
		position.y += halfHeight;

		auto object = GetStage()->Instantiate<GameObject>(position, Quat::Identity());
		auto collision = object->AddComponent<CollisionObb>();
		collision->SetFixed(true);

		auto objectTrans = object->GetComponent<Transform>();
		objectTrans->SetScale(Vec3(width, height, depth));
		objectTrans->SetForward(forward);

		auto drawComp = object->AddComponent<StaticModelDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		drawComp->SetEnabledDissolve(true);
		drawComp->SetDiffuse(Col4(1, 0, 0, 0.5f));

		object->AddComponent<DissolveAnimator>();

		object->SetAlphaActive(true);

		m_outCollisonObject.push_back(object);
	}

	void OwnArea::SetOutCollisionActive(const bool isActive) {
		for (auto object : m_outCollisonObject) {
			auto _object = object.lock();
			if (!_object)
				continue;

			// �\���͕��ʂ�
			if (isActive) {
				_object->SetActive(isActive);
				continue;
			}

			// ��\���̏ꍇ�f�B�]�u���A�j���[�V�������Đ�

			// �����蔻��𖳌�
			auto collision = _object->GetComponent<CollisionObb>();
			collision->SetUpdateActive(false);

			if (auto animator = _object->GetComponent<DissolveAnimator>(false)) {
				animator->SetPlayEndEvent(
					[_object, isActive]() {
						// �Đ��I�����ɔ�A�N�e�B�u
						_object->SetActive(isActive);
					}
				);
				animator->Start();
			}
		}
	}

	void OwnArea::OnCollisionEnter(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock()->GetGameObject();
		auto teamMember = other->GetComponent<I_TeamMember>(false);
		if (!teamMember) {
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