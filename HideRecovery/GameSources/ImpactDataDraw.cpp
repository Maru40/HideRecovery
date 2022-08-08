
/*!
@file ImpactDataDraw.cpp
@brief ImpactDataDrawクラス実体
担当：丸山裕喜
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
	///	表示データ
	//--------------------------------------------------------------------------------------

	ImpactDataDraw::DrawData::DrawData():
		DrawData(nullptr)
	{}

	ImpactDataDraw::DrawData::DrawData(const std::shared_ptr<NumbersObject>& dangerNumber) :
		dangerNumber(dangerNumber)
	{}

	//--------------------------------------------------------------------------------------
	///	影響データ表示用クラス本体
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
		if (!m_node) {	//ノードが存在しないなら処理をしない
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

		object->SetParent(GetGameObject());	 //親の設定

		if (auto numbersController = object->GetComponent<NumbersCtrl>(false)) {
			numbersController->SetTexture(L"NumberWhite_TX");
		}

		return object;
	}

	//--------------------------------------------------------------------------------------
	///	影響データ表示用オブジェクト本体
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