#include "stdafx.h"

#include "WatanabeStage.h"
#include "../DebugClass/Debug.h"
#include "../Effekseer/EfkEffect.h"
#include "Patch/PlayerInputer.h"
#include "Maruyama/Player/Component/PlayerController.h"
#include "Scene.h"
#include "../Shader/StaticModelDraw.h"
#include "../UI/UIObjects.h"
#include "../Camera/TestCamera.h"
#include "../StageObject/CameraTarget.h"
#include "../Component/DissolveAnimator.h"
#include "../Utility/AdvMeshUtil.h"
#include "../Shader/BarrierShader.h"

namespace basecross {
	void WatanabeStage::CreateViewLight() {
		const Vec3 eye(5.0f, 5.0f, -5.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<GameCamera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetTarget(AddGameObject<CameraTarget>(PtrCamera));
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	void WatanabeStage::OnCreate() {
		CreateViewLight();
		AddGameObject<EfkInterface>();
		AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"WatanabeStage");

		{
			auto obj = AddGameObject<GameObject>();
			auto drawComp = obj->AddComponent<StaticModelDraw>();
			drawComp->SetMeshResource(L"DEFAULT_CUBE");
			drawComp->SetTextureResource(L"Floor_TX");

			auto objectTrans = obj->GetComponent<Transform>();
			objectTrans->SetScale(40, 0.4, 25);

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
	}

	void WatanabeStage::OnUpdate() {
		const auto& inputDevice = App::GetApp()->GetMyInputDevice();
		const auto& keyBoard = inputDevice->GetKeyBoard();
		const auto& pad = inputDevice->GetXInputGamePad();
		//if (keyBoard.IsInputDown(KeyCode::Alpha1)) {
		//	m_gameStartUI->Start();
		//	Debug::GetInstance()->Log(L"StartUI");
		//}
		if (pad.IsInputDown(XInputCode::Start)) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToWatanabeStage");
		}

		if (pad.IsInputDown(XInputCode::A)) {
			for (auto& dis : m_dis) {
				dis.lock()->Start();
			}
		}

		if (pad.IsInputDown(XInputCode::B)) {
			for (auto& dis : m_dis) {
				dis.lock()->Reset();
			}
		}
	}

	void WatanabeStage::CreateMapOutCollision(
		const Vec3& startPosition,
		const Vec3& forward,
		const float& width, const float& height, const float& depth)
	{
		const float halfHeight = height * 0.5f;
		const float halfDepth = depth * 0.5f;

		// 指定位置からhalfDepth分外側へ
		auto position = startPosition + (forward.GetNormalized() * halfDepth);
		// 位置を上に上げる
		position.y += halfHeight;

		auto object = Instantiate<GameObject>(position, Quat::Identity());
		auto collision = object->AddComponent<CollisionObb>();
		collision->SetFixed(true);

		auto objectTrans = object->GetComponent<Transform>();
		objectTrans->SetScale(Vec3(width, height, depth));
		objectTrans->SetForward(forward);

		{
			// 板ポリは厚さが無いためオフセットは不要
			auto position = startPosition;
			position.y += halfHeight;

			auto planeObj = Instantiate<GameObject>(position, Quat::Identity());
			auto objectTrans = planeObj->GetComponent<Transform>();
			objectTrans->SetScale(Vec3(width, height, 1));
			objectTrans->SetForward(forward);

			auto drawComp = planeObj->AddComponent<BarrierShader>();
			drawComp->SetSamplerState(SamplerState::LinearWrap);
			drawComp->SetEnabledTime(true);

			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			AdvMeshUtil::CreateBoardPoly(20, Vec2(width, height), vertices, indices);
			auto meshData = MeshResource::CreateMeshResource(vertices, indices, true);
			drawComp->SetOriginalMeshResource(meshData);
			drawComp->SetOriginalMeshUse(true);
			drawComp->SetEnabledDissolve(true);
			drawComp->SetDiffuse(Col4(1, 0, 0, 1));

			auto dissolveAnimator = planeObj->AddComponent<DissolveAnimator>();
			planeObj->SetAlphaActive(true);

			m_dis.push_back(dissolveAnimator);
		}
	}
}