
/*!
@file ImpactMap.cpp
@brief ImpactMapのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "SingletonComponent.h"
#include "NavGraphNode.h"
#include "AstarEdge.h"
#include "SparseGraph.h"
#include "GraphAstar.h"
#include "ImpactMap.h"

#include "AstarNodeDraw.h"
#include "AstarEdgeDraw.h"

#include "PlayerInputer.h"

#include "EyeSearchRange.h"

#include "UtilityAstar.h"
#include "I_Impact.h"
#include "Heuristic.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	四角形データ(生成する範囲に利用)
		//--------------------------------------------------------------------------------------

		Rect::Rect()
			: Rect(Vec3(0.0f), 0.0f, 0.0f)
		{}

		Rect::Rect(const Vec3& centerPosition, const float& width, const float& depth)
			: centerPosition(centerPosition), width(width), depth(depth)
		{}

		Vec3 Rect::CalculateStartPosition() const {
			auto position = centerPosition;
			auto scale = Vec3(width, 0.0f, depth);
			auto halfScale = scale * 0.5f;
			float x = position.x - halfScale.x;
			float y = position.y;
			float z = position.z - halfScale.z;
			Vec3 startPosition(x, y, z);

			return startPosition;
		}

		//--------------------------------------------------------------------------------------
		///	Factory_影響マップ
		//--------------------------------------------------------------------------------------

		//Factoryパラメータ---------------------------------------------------------------------

		Factory_ImpactMap_Parametor::Factory_ImpactMap_Parametor()
			: Factory_ImpactMap_Parametor(Rect(), 5.0f)
		{}
		Factory_ImpactMap_Parametor::Factory_ImpactMap_Parametor(const Rect& rect, const float& intervalRange)
			: rect(rect), intervalRange(intervalRange), createHeight(0.0f)
		{}

		//Factory本体---------------------------------------------------------------------------

		Factory_ImpactMap::Factory_ImpactMap(const Parametor& parametor) 
			: m_param(parametor)
		{}

		void Factory_ImpactMap::CreateNode(const std::shared_ptr<GraphAstar>& astar) {
			const maru::Rect& rect = m_param.rect;
			const Vec3& startPosition = rect.CalculateStartPosition();
			const float& intervalRange = m_param.intervalRange;

			//基準となる横の大きさと、縦の大きさ
			float numLoopWidth = rect.width / intervalRange;
			float numLoopDepth = rect.depth / intervalRange;

			//縦と横に指定した数だけノードを生成。
			for (int i = 0; i <= numLoopWidth; i++) {
				auto widthOffset = intervalRange * i;

				for (int j = 0; j <= numLoopDepth; j++) {
					auto depthOffset = intervalRange * j;
					auto offset = Vec3(widthOffset, m_param.createHeight, depthOffset);

					auto position = startPosition + offset;
					astar->AddNode(position); //ノードの追加
				}
			}
		}

		void Factory_ImpactMap::CreateEdge(const std::shared_ptr<GraphAstar>& astar) {
			astar->AddEdges();
		}

		std::shared_ptr<GraphAstar> Factory_ImpactMap::CreateGraphAstar() {
			std::shared_ptr<GraphAstar> astar(new GraphAstar(std::make_shared<SparseGraph<NavGraphNode, AstarEdge>>(true)));
			CreateNode(astar);  //ノードの生成
			CreateEdge(astar);  //エッジの生成

			return astar;
		}

		std::shared_ptr<GraphAstar> Factory_ImpactMap::CreateGraphAstar(const Parametor& parametor) {
			m_param = parametor;
			return CreateGraphAstar();
		}

		void Factory_ImpactMap::AddNodes(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor) {
			m_param = parametor;
			CreateNode(astar);
		}

		void Factory_ImpactMap::AddEdges(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor) {
			m_param = parametor;
			astar->ClearEdges(); //エッジのクリア
			CreateEdge(astar);   //エッジの生成
		}

		void Factory_ImpactMap::AddNodesEdges(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor) {
			m_param = parametor;
			AddNodes(astar, parametor);
			AddEdges(astar, parametor);
		}


		//--------------------------------------------------------------------------------------
		/// 影響マップ
		//--------------------------------------------------------------------------------------

		ImpactMap::ImpactMap(const std::shared_ptr<Stage>& stage)
			:ImpactMap(stage, Factory_Parametor())
		{}

		ImpactMap::ImpactMap(const std::shared_ptr<Stage>& stage, const Factory_Parametor& parametor) :
			m_stage(stage),
			m_param(parametor), 
			m_astar(new GraphAstar(std::make_shared<SparseGraph<NavGraphNode, AstarEdge>>(true)))
		{
			CreateImpactData();
		}

		ImpactMap::~ImpactMap() {
			if (auto stage = GetStage()) {	
				if (m_nodeDraw) {
					stage->RemoveGameObject<GameObject>(m_nodeDraw->GetGameObject());
				}

				if (m_edgeDraw) {
					stage->RemoveGameObject<GameObject>(m_edgeDraw->GetGameObject());
				}
			}
		}

		void ImpactMap::CreateImpactData() {
			auto factory = Factory_ImpactMap(m_param);
			auto astar = factory.CreateGraphAstar();
			m_astar = astar;
		}

		void ImpactMap::CreateImpactData(const Factory_Parametor& parametor) {
			SetFactoryParametor(parametor);
			CreateImpactData();
		}

		void ImpactMap::AddNodes(const Factory_Parametor& parametor) {
			auto factory = Factory_ImpactMap(parametor);
			factory.AddNodes(m_astar, parametor);
		}

		void ImpactMap::AddEdges(const Factory_Parametor& parametor) {
			auto factory = Factory_ImpactMap(parametor);
			factory.AddEdges(m_astar, parametor);
		}

		void ImpactMap::AddImpactData(const Factory_Parametor& parametor) {
			AddNodes(parametor);
			AddEdges(parametor);
		}

		void ImpactMap::ClearNodes() {
			m_astar->ClearNodes();
		}

		void ImpactMap::ClearEdges() {
			m_astar->ClearEdges();
		}

		std::vector<Vec3> ImpactMap::GetRoutePositons(const Vec3& selfPosition, const Vec3& targetPosition) {
			m_astar->SearchAstarStart(selfPosition, targetPosition);
			auto positions = m_astar->GetRoutePositions();
			m_astar->ResetAstar();
			return positions;
		}

		//自分の視界内のノードを取得
		std::vector<std::shared_ptr<NavGraphNode>> ImpactMap::GetEyeRangeNodes(const Vec3& selfPosition, const std::shared_ptr<I_Impacter>& impacter) {
			std::vector<std::shared_ptr<NavGraphNode>> nodes;

			//自分自身のノードを取得
			auto selfNode = UtilityAstar::SearchNearNode(*m_astar.get(), selfPosition);
			if (!selfNode) {
				return nodes;
			}

			//視界データの取得
			auto impacterTrans = impacter->GetImpacterObject()->GetComponent<Transform>();
			auto eye = impacter->GetEyeSearchRange();
			EyeSearchRangeParametor eyeParam = eye->GetParametor();

			//Impacterの必要なデータの取得
			auto impacterPosition = impacterTrans->GetPosition();
			auto forward = impacterTrans->GetForward();

			auto graph = m_astar->CreateCopyGraph();	//グラフのコピー

			auto nextNode = selfNode;
			while (true) {
				std::vector<std::shared_ptr<NavGraphNode>> currentAddNodes;
				auto currentNode = nextNode;
				auto edges = graph->GetEdges(currentNode->GetIndex());
				for (auto edge : edges) {
					auto toNode = graph->GetNode(edge->GetTo());
					if (!toNode->IsActive()) {	//アクティブでないならcontinue
						continue;
					}

					//ノードが視界内なら
					if (eye->IsInEyeRange(toNode->GetPosition())) {
						nodes.push_back(toNode);
						currentAddNodes.push_back(toNode);
						toNode->SetIsActive(false);	//ノードを非アクティブにする。
					}
					
					auto currentPosition = currentNode->GetPosition();
					auto direct = currentPosition + forward;
					nextNode = UtilityAstar::CalculateTargetDirectNode(*m_astar.get(), currentNode, direct);
				}

				//追加が一度もされないなら
				if (currentAddNodes.size() == 0) {
					break;
				}
			}

			return nodes;
		}

		//------------------------------------------------------------------------------------------------
		/// アクセッサ
		//------------------------------------------------------------------------------------------------

		void ImpactMap::SetFactoryParametor(const Factory_Parametor& parametor) noexcept {
			m_param = parametor;
		}

		ImpactMap::Factory_Parametor ImpactMap::GetFactoryParametor() const noexcept {
			return m_param;
		}

		std::shared_ptr<GraphAstar> ImpactMap::GetGraphAstar() const noexcept {
			return m_astar;
		}


		//デバッグ-------------------------------------------------------------------------------------------

		void ImpactMap::DebugInput(const std::function<bool()>& isInput) {
			if (!m_nodeDraw || !m_edgeDraw) {
				return;
			}

			if (isInput()) {
				m_nodeDraw->SetDebugDraw(!m_nodeDraw->GetDebugDraw());
				m_edgeDraw->SetDebugDraw(!m_edgeDraw->GetDebugDraw());
			}
		}

		void ImpactMap::CreateDebugDraw(const bool isDraw) {
			//デバッグドローの追加
			//return;
			m_nodeDraw = GetStage()->AddGameObject<GameObject>()->AddComponent<AstarNodeDraw>(m_astar);
			m_edgeDraw = GetStage()->AddGameObject<GameObject>()->AddComponent<AstarEdgeDraw>(m_astar);
			SetIsDebugDraw(isDraw);
		}

		void ImpactMap::SetIsDebugDraw(const bool isDraw) {
			m_nodeDraw->SetDebugDraw(isDraw);
			m_edgeDraw->SetDebugDraw(isDraw);
		}

		void ImpactMap::ChangeTargetNodeColor(const int& nodeIndex, const Col4& color) {
			if (m_nodeDraw) {
				m_nodeDraw->SetColor(nodeIndex, color);
			}	
		}

	}
}

//endbasecross