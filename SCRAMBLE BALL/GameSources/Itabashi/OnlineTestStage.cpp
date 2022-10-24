#include "stdafx.h"
#include "OnlineTestStage.h"
#include "OnlineManager.h"
#include "OnlineTransformSynchronization.h"

namespace basecross
{
	class TestController : public Component
	{
		std::weak_ptr<Transform> m_transform;
		int m_onlinePlayerNumber = 0;

	public:
		TestController(const std::shared_ptr<GameObject>& owner) :
			Component(owner)
		{

		}

		void OnCreate() override
		{
			m_transform = GetGameObject()->GetComponent<Transform>();
		}

		void OnUpdate() override
		{
			auto transform = m_transform.lock();
			int localNumber = Online::OnlineManager::GetLocalPlayer().getNumber();

			if (!transform || m_onlinePlayerNumber == 0 || m_onlinePlayerNumber != localNumber)
			{
				return;
			}

			auto position = transform->GetPosition();

			if (App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputPush(itbs::Input::KeyCode::LeftArrow))
			{
				position.x -= 0.025f;
				transform->SetPosition(position);
			}

			if (App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputPush(itbs::Input::KeyCode::RightArrow))
			{
				position.x += 0.025f;
				transform->SetPosition(position);
			}

			if (App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputPush(itbs::Input::KeyCode::UpArrow))
			{
				position.z += 0.025f;
				transform->SetPosition(position);
			}

			if (App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputPush(itbs::Input::KeyCode::DownArrow))
			{
				position.z -= 0.025f;
				transform->SetPosition(position);
			}
		}

		void SetOnlinePlayerNumber(int playerNumber)
		{
			m_onlinePlayerNumber = playerNumber;
		}
	};

	class TestOnlineManager : public Online::OnlineComponent
	{
	public:
		TestOnlineManager(const std::shared_ptr<GameObject>& owner) : OnlineComponent(owner) {}

		void OnUpdate() override {}
		void OnDraw() override {}

		void OnConnected() override
		{
			Online::OnlineManager::JoinRandomOrCreateRoom(ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(6));
		}
	};

	void OnlineTestStage::OnCreate()
	{
		const Vec3 eye(0.0f, 5.0f, -5.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();

		Col4 cols[4] =
		{
			Col4(1.0f,0,0,1.0f),
			Col4(0,1,0,1.0f),
			Col4(0,0,1,1.0f),
			Col4(1,1,0,1)
		};

		auto manageObject = Instantiate<GameObject>();
		manageObject->AddComponent<TestOnlineManager>();

		for (int i = 0; i < 4; ++i)
		{
			auto testObject = Instantiate<GameObject>();

			auto drawer = testObject->AddComponent<PNTStaticDraw>();
			drawer->SetMeshResource(L"DEFAULT_CUBE");
			drawer->SetDiffuse(cols[i]);

			auto testComponent = testObject->AddComponent<TestController>();
			testComponent->SetOnlinePlayerNumber(i + 1);
			auto onlineTransform = testObject->AddComponent<Online::OnlineTransformSynchronization>();
			onlineTransform->SetOnlinePlayerNumber(i + 1);
		}

		bool connect = Online::OnlineManager::Connect();
		int i = 0;
	}
}