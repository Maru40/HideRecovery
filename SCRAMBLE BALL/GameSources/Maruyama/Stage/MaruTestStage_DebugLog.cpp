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

namespace basecross {

	//デバッグ変数
	std::shared_ptr<maru::SparseGraph<maru::AstarNode, maru::AstarEdge>> m_debugGraph;
	
	void MaruTestStage_DebugLog::CreateViewLight() {
		const Vec3 eye(0.0f, 30.0f, -0.000001f);
		const Vec3 at(0, 0.0f, 0);
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
	}

	void MaruTestStage_DebugLog::OnCreate() {
		CreateViewLight();
		auto log = AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"MaruTestStage_DebugLog");

		//スレッドプールのデバッグ
		AddGameObject<Tester::TesterThreadObject>();

		//セルデバッグ表示
		//AddGameObject<GameObject>()->AddComponent<maru::FieldCellMap>();

		//フラッドフィルアルゴリズムテスト
		auto graph = std::make_shared<maru::SparseGraph<maru::AstarNode, maru::AstarEdge>>();
		auto floodFill = std::make_shared<maru::Factory_WayPointMap_FloodFill>(GetThis<Stage>());
		auto factoryParam = maru::Factory_WayPointMap_FloodFill::Parametor();
		auto& rect = factoryParam.rect;
		rect.width = 10.0f;
		rect.depth = 10.0f;
		floodFill->AddWayPointMap(graph, factoryParam);
		Debug::GetInstance()->Log((int)graph->GetNodes().size());

		m_debugGraph = graph;
	}

	void MaruTestStage_DebugLog::OnUpdate() {
		if (PlayerInputer::GetInstance()->IsBDown()) {
			
		}

		if (m_debugGraph) {
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

}