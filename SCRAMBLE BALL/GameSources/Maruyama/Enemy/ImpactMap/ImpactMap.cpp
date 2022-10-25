
/*!
@file ImpactMap.cpp
@brief ImpactMapのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Enemy/Astar/NavGraphNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"
#include "Maruyama/Enemy/Astar/SparseGraph.h"
#include "Maruyama/Enemy/Astar/AstarGraph.h"
#include "Maruyama/Enemy/Astar/GraphAstar.h"
#include "ImpactMap.h"

#include "Maruyama/Enemy/Astar/AstarDebug/AstarEdgeDraw.h"
#include "Maruyama/Enemy/Astar/AstarDebug/AstarNodeDraw.h"

#include "Patch/PlayerInputer.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"

#include "Maruyama/Enemy/Astar/UtilityAstar.h"
#include "Maruyama/Interface/I_Impact.h"
#include "Maruyama/Enemy/Astar/Heuristic.h"

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

		bool Rect::IsInRect(const Vec3& position) {
			auto rectStartPosition = CalculateStartPosition();

			if (position.x > rectStartPosition.x &&
				position.x < rectStartPosition.x + this->width &&
				position.z > rectStartPosition.z &&
				position.z < rectStartPosition.z + this->depth
			) {
				return true;
			}

			return false;
		}

		//--------------------------------------------------------------------------------------
		///	Factory_影響マップ
		//--------------------------------------------------------------------------------------

		//AreaNodeCount-------------------------------------------------------------------------

		Factory_ImpactMap_Parametor::AreaNodeCount::AreaNodeCount() :
			AreaNodeCount(5, 5)
		{}

		Factory_ImpactMap_Parametor::AreaNodeCount::AreaNodeCount(const int width, const int depth) :
			width(width), 
			depth(depth)
		{}

		//Factoryパラメータ---------------------------------------------------------------------

		Factory_ImpactMap_Parametor::Factory_ImpactMap_Parametor()
			: Factory_ImpactMap_Parametor(Rect(), 5.0f)
		{}

		Factory_ImpactMap_Parametor::Factory_ImpactMap_Parametor(const Rect& rect, const float intervalRange)
			: Factory_ImpactMap_Parametor(rect, intervalRange, AreaNodeCount())
		{}

		Factory_ImpactMap_Parametor::Factory_ImpactMap_Parametor(const Rect& rect, const float intervalRange, const AreaNodeCount areaNodeCount) :
			rect(rect), 
			intervalRange(intervalRange), 
			areaNodeCount(areaNodeCount), 
			createHeight(0.0f)
		{}

		//Factory本体---------------------------------------------------------------------------

		Factory_ImpactMap::Factory_ImpactMap(const Parametor& parametor) 
			: m_param(parametor)
		{}

		int Factory_ImpactMap::CalculateAreaIndex(const int widthCount, const int depthCount, const int numLoopDepth) {
			int maxDepth = (numLoopDepth / m_param.areaNodeCount.depth) + 1;
			int width = widthCount / m_param.areaNodeCount.width;
			int depth = depthCount / m_param.areaNodeCount.depth;

			return (width * maxDepth) + (depth);
		}

		//パラメータからノードの生成
		void Factory_ImpactMap::CreateNodes(const std::shared_ptr<GraphAstar>& astar) {
			using AreaNodeCount = Parametor::AreaNodeCount;

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
					astar->AddNode(position, ImpactData(CalculateAreaIndex(i, j, (int)numLoopDepth))); //ノードの追加
				}
			}
		}

		void Factory_ImpactMap::CreateEdges(const std::shared_ptr<GraphAstar>& astar) {
			astar->AddEdges();
		}

		void Factory_ImpactMap::CreateAreaEdges(
			const std::shared_ptr<GraphAstar>& astar,
			const int widthCount,
			const int depthCount,
			const int numLoopWidth,
			const int numLoopDepth
		) {
			//int maxDepth = numLoopDepth / m_param.areaNodeCount.depth;

			auto graph = astar->GetReWiritingAreaGraph();
			int index = (widthCount * numLoopDepth) + depthCount;

			auto leftDepth = numLoopDepth * widthCount;	//手前端

			int right = (index + 1);
			int left = (index - 1);
			int upper = index + (numLoopDepth);
			int bottom = index - numLoopDepth;

			int rightUpper = upper + 1;
			int rightBottom = bottom + 1;
			int leftUpper = upper - 1;
			int leftBottom = bottom - 1;

			int maxIndex = numLoopWidth * numLoopDepth;

			//上が追加できるなら
			if (upper < maxIndex) {
				graph->AddEdge(std::make_shared<AstarEdge>(index, upper));
			}

			//下が追加できるなら
			if (bottom >= 0) {
				graph->AddEdge(std::make_shared<AstarEdge>(index, bottom));
			}

			//右が追加できるなら
			if (right < numLoopDepth * (widthCount + 1) && right < maxIndex) {
				graph->AddEdge(std::make_shared<AstarEdge>(index, right));
			}

			//左が追加できるなら(左が、左端より大きいなら)
			if (left >= leftDepth) {
				graph->AddEdge(std::make_shared<AstarEdge>(index, left));
			}

			//右上が追加できるなら
			auto maxRightUpper = (leftDepth + numLoopDepth) + numLoopDepth;
			if (rightUpper < maxIndex && rightUpper < maxRightUpper) {
 				graph->AddEdge(std::make_shared<AstarEdge>(index, rightUpper));
			}

			//右下が追加できるなら
			auto maxRightBottom = (leftDepth);
			if (rightBottom >= 0 && rightBottom < maxRightBottom) {
				graph->AddEdge(std::make_shared<AstarEdge>(index, rightBottom));
			}

			//左上が追加できるなら
			auto minLeftUpper = leftDepth + numLoopDepth;
			if (leftUpper >= minLeftUpper && leftUpper < maxIndex) {
				graph->AddEdge(std::make_shared<AstarEdge>(index, leftUpper));
			}

			//左下が追加できるなら
			auto minLeftBottom = leftDepth - numLoopDepth;
			if (leftBottom >= 0 && leftBottom >= minLeftBottom) {
				graph->AddEdge(std::make_shared<AstarEdge>(index, leftBottom));
			}
		}

		std::shared_ptr<GraphAstar> Factory_ImpactMap::CreateGraphAstar() {
			std::shared_ptr<GraphAstar> astar(new GraphAstar(std::make_shared<GraphAstar::GraphType>(true)));
			CreateNodes(astar);  //ノードの生成
			CreateEdges(astar);  //エッジの生成

			return astar;
		}

		std::shared_ptr<GraphAstar> Factory_ImpactMap::CreateGraphAstar(const Parametor& parametor) {
			m_param = parametor;
			return CreateGraphAstar();
		}

		void Factory_ImpactMap::AddNodes(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor) {
			m_param = parametor;
			CreateNodes(astar);
		}

		void Factory_ImpactMap::AddEdges(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor) {
			m_param = parametor;
			astar->ClearEdges();	//エッジのクリア
			CreateEdges(astar);		//エッジの生成
		}

		void Factory_ImpactMap::AddAreaNodes(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor) {
			m_param = parametor;
			astar->CreateAreaAstarGraph();
		}

		void Factory_ImpactMap::AddAreaEdges(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor) {
			m_param = parametor;
			auto graph = astar->GetReWiritingAreaGraph();

			using AreaNodeCount = Parametor::AreaNodeCount;

			const maru::Rect& rect = m_param.rect;
			const Vec3& startPosition = rect.CalculateStartPosition();
			const float& intervalRange = m_param.intervalRange;

			//基準となる横の大きさと、縦の大きさ
			int numLoopWidth = static_cast<int>(rect.width / intervalRange);
			int numLoopDepth = static_cast<int>(rect.depth / intervalRange);

			int width = (numLoopWidth / m_param.areaNodeCount.width) + 1;
			int depth = (numLoopDepth / m_param.areaNodeCount.depth) + 1;

			//縦と横に指定した数だけノードを生成。
			for (int i = 0; i < width; i++) {
				for (int j = 0; j < depth; j++) {
					CreateAreaEdges(astar, i, j, width, depth);
				}
			}

			auto nodes = astar->GetAreaGraph()->GetNodes();
			auto edges = astar->GetAreaGraph()->GetEdgesMap();
		}

		void Factory_ImpactMap::AddNodesEdges(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor) {
			m_param = parametor;
			AddNodes(astar, parametor);
			AddEdges(astar, parametor);
		}

		//--------------------------------------------------------------------------------------
		///	Factory_影響マップ_フラッドフィルアルゴリズム(現在作成中) 
		//--------------------------------------------------------------------------------------

		void Factory_ImpactMap_FloodFill::AddImpactMap(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor) {
			
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
			m_baseAstar(new GraphAstar(std::make_shared<GraphAstar::GraphType>(true)))
		{
			CreateImpactData();
		}

		ImpactMap::~ImpactMap() {	//デストラクタ
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
			m_baseAstar = astar;
		}

		void ImpactMap::CreateImpactData(const Factory_Parametor& parametor) {
			SetFactoryParametor(parametor);
			CreateImpactData();
		}

		void ImpactMap::AddNodes(const Factory_Parametor& parametor) {
			auto factory = Factory_ImpactMap(parametor);
			factory.AddNodes(m_baseAstar, parametor);
		}

		void ImpactMap::AddEdges(const Factory_Parametor& parametor) {
			auto factory = Factory_ImpactMap(parametor);
			factory.AddEdges(m_baseAstar, parametor);
		}

		void ImpactMap::AddAreaNodes(const Factory_Parametor& parametor) {
			auto factory = Factory_ImpactMap(parametor);
			factory.AddAreaNodes(m_baseAstar, parametor);
		}

		void ImpactMap::AddAreaEdges(const Factory_Parametor& parametor) {
			auto factory = Factory_ImpactMap(parametor);
			factory.AddAreaEdges(m_baseAstar, parametor);
		}

		void ImpactMap::AddImpactData(const Factory_Parametor& parametor) {
			//現在はこの関数からほとんどのノードを生成している。
			AddNodes(parametor);
			AddEdges(parametor);

			//仮でArea用のノードやエッジも制作(代案検討中)
			AddAreaNodes(parametor);
			AddAreaEdges(parametor);
		}

		void ImpactMap::ClearNodes() {
			m_baseAstar->ClearNodes();
		}

		void ImpactMap::ClearEdges() {
			m_baseAstar->ClearEdges();
		}

		std::vector<Vec3> ImpactMap::GetRoutePositons(const Vec3& selfPosition, const Vec3& targetPosition) {
			m_baseAstar->SearchAstarStart(selfPosition, targetPosition);
			auto positions = m_baseAstar->GetRoutePositions();
			m_baseAstar->ResetAstar();
			return positions;
		}

		std::vector<Vec3> ImpactMap::GetRoutePositions(const Vec3& selfPosition, const Vec3& targetPosition, const int areaIndex) {
			m_baseAstar->SearchAstarStart(selfPosition, targetPosition, areaIndex);
			auto positions = m_baseAstar->GetRoutePositions();
			m_baseAstar->ResetAstar();
			return positions;
		}

		//自分の視界内のノードを取得
		std::vector<std::shared_ptr<NavGraphNode>> ImpactMap::GetEyeRangeNodes(const Vec3& selfPosition, const std::shared_ptr<I_Impacter>& impacter) {
			std::vector<std::shared_ptr<NavGraphNode>> nodes;

			//自分自身のノードを取得
			auto selfNode = impacter->GetImpacterData().selfNode.lock();
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

			auto graph = m_baseAstar->CreateCopyGraph();	//グラフのコピー

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
						nodes.push_back(m_baseAstar->GetGraph()->GetNode(toNode->GetIndex()));
						currentAddNodes.push_back(toNode);
						toNode->SetIsActive(false);	//ノードを非アクティブにする。
					}
					
					auto currentPosition = currentNode->GetPosition();
					auto direct = currentPosition + forward;
					nextNode = UtilityAstar::CalculateTargetDirectNode(*m_baseAstar.get(), currentNode, direct);
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
			return m_baseAstar;
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
			auto graph = m_baseAstar->GetGraph();
			m_nodeDraw = GetStage()->AddGameObject<GameObject>()->AddComponent<AstarNodeDraw>(graph);
			m_edgeDraw = GetStage()->AddGameObject<GameObject>()->AddComponent<AstarEdgeDraw>(graph);
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