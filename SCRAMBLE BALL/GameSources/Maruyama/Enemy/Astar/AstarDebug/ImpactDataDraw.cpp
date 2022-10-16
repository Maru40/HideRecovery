
/*!
@file ImpactDataDraw.cpp
@brief ImpactDataDraw�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "ImpactDataDraw.h"

#include "Maruyama/Enemy/Astar/AstarDebug/ImpactDataDraw.h"
#include "Maruyama/Enemy/Astar/NavGraphNode.h"

#include "Maruyama/UI/3D/Object/NumbersObject.h"
#include "Maruyama/UI/3D/Component/NumbersCtrl.h"

#include "Maruyama/Utility/Component/BillBoard_Ex.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�\���f�[�^
	//--------------------------------------------------------------------------------------

	ImpactDataDraw::DrawData::DrawData():
		DrawData(nullptr, nullptr)
	{}

	ImpactDataDraw::DrawData::DrawData(const std::shared_ptr<NumbersObject>& dangerNumber, const std::shared_ptr<NumbersObject>& areaIndex) :
		dangerNumber(dangerNumber),
		areaIndex(areaIndex)
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
		m_drawData(DrawData())
	{}
	
	void ImpactDataDraw::OnCreate() {
		auto numbersObject = CreateNumbersObject(Vec3(0.0f, 0.5f, 0.0f));
		auto areaIndexObject = CreateNumbersObject(Vec3(0.0f, 1.0f, 0.0f), static_cast<float>(m_node->GetAreaIndex()));

		m_drawData = DrawData(numbersObject, areaIndexObject);

		//GetGameObject()->AddComponent<BillBoard_Ex>();
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

	std::shared_ptr<NumbersObject> ImpactDataDraw::CreateNumbersObject(const Vec3& offset, const float number) {
		maru::ImpactData data = m_node->GetImpactData();

		auto object = GetStage()->AddGameObject<NumbersObject>();
		auto objectTrans = object->GetComponent<Transform>();
		objectTrans->SetPosition(transform->GetPosition() + offset);

		object->SetParent(GetGameObject());	 //�e�̐ݒ�

		if (auto numbersController = object->GetComponent<NumbersCtrl>(false)) {
			numbersController->SetTexture(L"NumberWhite_TX");
			numbersController->SetValue(number);
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