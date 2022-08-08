
/*!
@file ImpactDataDraw.cpp
@brief ImpactDataDraw�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "ImpactDataDraw.h"

#include "I_Impact.h"
#include "NavGraphNode.h"

#include "NumbersObject.h"
#include "NumbersCtrl.h"

#include "BillBoard_Ex.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�\���f�[�^
	//--------------------------------------------------------------------------------------

	ImpactDataDraw::DrawData::DrawData():
		DrawData(nullptr)
	{}

	ImpactDataDraw::DrawData::DrawData(const std::shared_ptr<NumbersObject>& dangerNumber) :
		dangerNumber(dangerNumber)
	{}

	//--------------------------------------------------------------------------------------
	///	�e���f�[�^�\���p�N���X�{��
	//--------------------------------------------------------------------------------------

	ImpactDataDraw::ImpactDataDraw(
		const std::shared_ptr<GameObject>& objPtr, 
		const std::shared_ptr<NavGraphNode>& node
	) :
		Component(objPtr), 
		m_node(node),
		m_drawData(DrawData(nullptr))
	{}
	
	void ImpactDataDraw::OnCreate() {
		auto numbersObject = CreateDangerNumbersObject(Vec3(0.0f, 1.0f, 0.0f));

		m_drawData = DrawData(numbersObject);

		GetGameObject()->AddComponent<BillBoard_Ex>();
	}

	void ImpactDataDraw::OnUpdate() {
		if (!m_node) {	//�m�[�h�����݂��Ȃ��Ȃ珈�������Ȃ�
			return;
		}

		if (auto& dangerNumber = m_drawData.dangerNumber) {
			auto numbersController = dangerNumber->GetComponent<NumbersCtrl>();
			//numbersController->SetValue(m_node->GetImpactData().dangerValue);
		}
	}

	std::shared_ptr<NumbersObject> ImpactDataDraw::CreateDangerNumbersObject(const Vec3& offset) {
		maru::ImpactData data = m_node->GetImpactData();

		auto object = GetStage()->AddGameObject<NumbersObject>();
		auto objectTrans = object->GetComponent<Transform>();
		objectTrans->SetPosition(transform->GetPosition() + offset);

		object->SetParent(GetGameObject());	 //�e�̐ݒ�

		if (auto numbersController = object->GetComponent<NumbersCtrl>(false)) {
			numbersController->SetTexture(L"NumberWhite_TX");
		}

		return object;
	}

	//--------------------------------------------------------------------------------------
	///	�e���f�[�^�\���p�I�u�W�F�N�g�{��
	//--------------------------------------------------------------------------------------

	ImpactDataDrawObject::ImpactDataDrawObject(
		const std::shared_ptr<Stage>& stage, 
		const std::shared_ptr<NavGraphNode>& node
	) :
		GameObject(stage), 
		m_node(node)
	{}

	void ImpactDataDrawObject::OnCreate() {
		AddComponent<ImpactDataDraw>(m_node.GetShard());
	}

}