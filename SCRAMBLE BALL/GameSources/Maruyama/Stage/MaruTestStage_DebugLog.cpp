#include "stdafx.h"

#include "MaruTestStage_DebugLog.h"

#include "Watanabe/DebugClass/Debug.h"

#include "Maruyama/Thread/ThreadPool.h"

#include "Patch/PlayerInputer.h"

#include "Maruyama/Enemy/ImpactMap/Cell.h"
#include "Maruyama/Enemy/ImpactMap/FieldCellMap.h"

#include "Maruyama/Enemy/ImpactMap/Factory_WayPointMap_FloodFill.h"

#include "Maruyama/Patch/Graphs/SparseGraph_Ex.h"

#include "Maruyama/Enemy/Astar/AstarNode.h"
#include "Maruyama/StateMachine/NodeBase.h"
#include "Maruyama/StateMachine/EdgeBase.h"
#include "Maruyama/Enemy/Astar/AstarEdge_Ex.h"

#include "Maruyama/Utility/UtilityObstacle.h"
#include "Maruyama/Utility/Utility.h"
#include "Watanabe/StageObject/Block.h"

#include "Maruyama/Thread/ThreadPool.h"

#include "Maruyama/Enemy/AIDirector/AIDirector_Ex.h"

namespace basecross {

	//�f�o�b�O�ϐ�
	std::shared_ptr<maru::SparseGraph<maru::AstarNode, maru::AstarEdge>> m_debugGraph;
	int test_debugCount = 0;
	
	void MaruTestStage_DebugLog::CreateViewLight() {
		//const Vec3 eye(0.0f, 30.0f, -0.000001f);
		const Vec3 eye(0.0f, 252.4f, -0.000001f);
		const Vec3 at(0, 0.0f, 0);
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

	struct Test {

	};

	int say_ok(const int number) {
		return 0;
	}

	void MaruTestStage_DebugLog::OnCreate() {
		CreateViewLight();
		auto log = AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"MaruTestStage_DebugLog");

		//�X���b�h�v�[���̃f�o�b�O
		AddGameObject<Tester::TesterThreadObject>();

		//�Z���f�o�b�O�\��
		//AddGameObject<GameObject>()->AddComponent<maru::FieldCellMap>();

		//Vec3 positions[] = {
		//	Vec3(-2.5f, 0.0f, 0.0f),
		//};

		//for (auto& position : positions) {
		//	auto object = Instantiate<GameObject>(position, Quat::Identity());
		//	object->AddComponent<PNTStaticDraw>()->SetMeshResource(L"DEFAULT_CUBE");
		//	object->AddComponent<CollisionObb>();
		//	object->GetComponent<Transform>()->SetScale(Vec3(1.0f, 5.0f, 15.0f));
		//	object->AddTag(maru::UtilityObstacle::DEFAULT_OBSTACLE_TAGS[0]);
		//}

		//�e�X�gAIDirector�̐���
		AddGameObject<GameObject>()->AddComponent<Enemy::AIDirector_Ex>();

		//�|�C���^�̃A�h���X�m�F
		//GameObject* objPtr = new GameObject(GetThis<Stage>());
		//Debug::GetInstance()->Log((int)(uintptr_t)objPtr);
		//delete(objPtr);
		//Debug::GetInstance()->Log((int)(uintptr_t)objPtr);

		//�e�X�g�}�b�v�̐���
		//CreateTestMap();	
	}

	void MaruTestStage_DebugLog::OnUpdate() {
		if (PlayerInputer::GetInstance()->IsBDown()) {
			
		}

		if (m_debugGraph) {
			return;
			for (auto& node : m_debugGraph->GetNodes()) {
				node.second->OnDebugDraw();
			}

			for (auto& pair : m_debugGraph->GetEdgesMap()) {
				for (auto& edge : pair.second) {
					edge->OnDebugDraw();
				}
			}
		}
	}

	void MaruTestStage_DebugLog::CreateTestMap() {
		// Map�̓ǂݍ���
		CreateMap(L"StageS2.csv");

		//�t���b�h�t�B���A���S���Y���e�X�g
		auto graph = std::make_shared<maru::SparseGraph<maru::AstarNode, maru::AstarEdge>>();
		auto floodFill = std::make_shared<maru::Factory_WayPointMap_FloodFill>(GetThis<Stage>());
		auto factoryParam = maru::Factory_WayPointMap_FloodFill::Parametor();
		auto& rect = factoryParam.rect;
		factoryParam.intervalRange = 5.0f;	//�C���^�[�o�[�������W��1�ł��������\�B
		constexpr float Width = 100.0f;
		constexpr float Depth = 200.0f;
		rect.width = Width - factoryParam.intervalRange;
		rect.depth = Depth - factoryParam.intervalRange;
		floodFill->AddWayPointMap(graph, factoryParam);
		Debug::GetInstance()->Log((int)graph->GetNodes().size());
		Debug::GetInstance()->Log((int)graph->GetNumAllEdges());

		m_debugGraph = graph;
	}

}