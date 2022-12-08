
/*!
@file ImpactMap.cpp
@brief ImpactMap�̃N���X����
�S���F�ێR�T��
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
		///	Factory_�e���}�b�v
		//--------------------------------------------------------------------------------------

		//AreaNodeCount-------------------------------------------------------------------------

		Factory_ImpactMap_Parametor::AreaNodeCount::AreaNodeCount() :
			AreaNodeCount(5, 5)
		{}

		Factory_ImpactMap_Parametor::AreaNodeCount::AreaNodeCount(const int width, const int depth) :
			width(width), 
			depth(depth)
		{}

		//Factory�p�����[�^---------------------------------------------------------------------

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

		//Factory�{��---------------------------------------------------------------------------

		Factory_ImpactMap::Factory_ImpactMap(const Parametor& parametor) 
			: m_param(parametor)
		{}

		int Factory_ImpactMap::CalculateAreaIndex(const int widthCount, const int depthCount, const int numLoopDepth) {
			int maxDepth = (numLoopDepth / m_param.areaNodeCount.depth) + 1;
			int width = widthCount / m_param.areaNodeCount.width;
			int depth = depthCount / m_param.areaNodeCount.depth;

			return (width * maxDepth) + (depth);
		}

		//�p�����[�^����m�[�h�̐���
		void Factory_ImpactMap::CreateNodes(const std::shared_ptr<GraphAstar>& astar) {
			using AreaNodeCount = Parametor::AreaNodeCount;

			const maru::Rect& rect = m_param.rect;
			const Vec3& startPosition = rect.CalculateStartPosition();
			const float& intervalRange = m_param.intervalRange;

			//��ƂȂ鉡�̑傫���ƁA�c�̑傫��
			float numLoopWidth = rect.width / intervalRange;
			float numLoopDepth = rect.depth / intervalRange;

			//�c�Ɖ��Ɏw�肵���������m�[�h�𐶐��B
			for (int i = 0; i <= numLoopWidth; i++) {
				auto widthOffset = intervalRange * i;

				for (int j = 0; j <= numLoopDepth; j++) {
					auto depthOffset = intervalRange * j;
					auto offset = Vec3(widthOffset, m_param.createHeight, depthOffset);

					auto position = startPosition + offset;
					astar->AddNode(position, ImpactData(CalculateAreaIndex(i, j, (int)numLoopDepth))); //�m�[�h�̒ǉ�
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

			auto leftDepth = numLoopDepth * widthCount;	//��O�[

			int right = (index + 1);
			int left = (index - 1);
			int upper = index + (numLoopDepth);
			int bottom = index - numLoopDepth;

			int rightUpper = upper + 1;
			int rightBottom = bottom + 1;
			int leftUpper = upper - 1;
			int leftBottom = bottom - 1;

			int maxIndex = numLoopWidth * numLoopDepth;

			//�オ�ǉ��ł���Ȃ�
			if (upper < maxIndex) {
				graph->AddEdge(std::make_shared<AstarEdge>(index, upper));
			}

			//�����ǉ��ł���Ȃ�
			if (bottom >= 0) {
				graph->AddEdge(std::make_shared<AstarEdge>(index, bottom));
			}

			//�E���ǉ��ł���Ȃ�
			if (right < numLoopDepth * (widthCount + 1) && right < maxIndex) {
				graph->AddEdge(std::make_shared<AstarEdge>(index, right));
			}

			//�����ǉ��ł���Ȃ�(�����A���[���傫���Ȃ�)
			if (left >= leftDepth) {
				graph->AddEdge(std::make_shared<AstarEdge>(index, left));
			}

			//�E�オ�ǉ��ł���Ȃ�
			auto maxRightUpper = (leftDepth + numLoopDepth) + numLoopDepth;
			if (rightUpper < maxIndex && rightUpper < maxRightUpper) {
 				graph->AddEdge(std::make_shared<AstarEdge>(index, rightUpper));
			}

			//�E�����ǉ��ł���Ȃ�
			auto maxRightBottom = (leftDepth);
			if (rightBottom >= 0 && rightBottom < maxRightBottom) {
				graph->AddEdge(std::make_shared<AstarEdge>(index, rightBottom));
			}

			//���オ�ǉ��ł���Ȃ�
			auto minLeftUpper = leftDepth + numLoopDepth;
			if (leftUpper >= minLeftUpper && leftUpper < maxIndex) {
				graph->AddEdge(std::make_shared<AstarEdge>(index, leftUpper));
			}

			//�������ǉ��ł���Ȃ�
			auto minLeftBottom = leftDepth - numLoopDepth;
			if (leftBottom >= 0 && leftBottom >= minLeftBottom) {
				graph->AddEdge(std::make_shared<AstarEdge>(index, leftBottom));
			}
		}

		std::shared_ptr<GraphAstar> Factory_ImpactMap::CreateGraphAstar() {
			std::shared_ptr<GraphAstar> astar(new GraphAstar(std::make_shared<GraphAstar::GraphType>(true)));
			CreateNodes(astar);  //�m�[�h�̐���
			CreateEdges(astar);  //�G�b�W�̐���

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
			astar->ClearEdges();	//�G�b�W�̃N���A
			CreateEdges(astar);		//�G�b�W�̐���
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

			//��ƂȂ鉡�̑傫���ƁA�c�̑傫��
			int numLoopWidth = static_cast<int>(rect.width / intervalRange);
			int numLoopDepth = static_cast<int>(rect.depth / intervalRange);

			int width = (numLoopWidth / m_param.areaNodeCount.width) + 1;
			int depth = (numLoopDepth / m_param.areaNodeCount.depth) + 1;

			//�c�Ɖ��Ɏw�肵���������m�[�h�𐶐��B
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
		///	Factory_�e���}�b�v_�t���b�h�t�B���A���S���Y��(���ݍ쐬��) 
		//--------------------------------------------------------------------------------------

		void Factory_ImpactMap_FloodFill::AddImpactMap(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor) {
			
		}

		//--------------------------------------------------------------------------------------
		/// �e���}�b�v
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

		ImpactMap::~ImpactMap() {	//�f�X�g���N�^
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
			//���݂͂��̊֐�����قƂ�ǂ̃m�[�h�𐶐����Ă���B
			AddNodes(parametor);
			AddEdges(parametor);

			//����Area�p�̃m�[�h��G�b�W������(��Č�����)
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

		std::vector<Vec3> ImpactMap::GetRoutePositions(
			const Vec3& selfPosition, 
			const Vec3& targetPosition, 
			const int areaIndex,
			const int targetAreaIndex
		) {
			m_baseAstar->SearchAstarStart(selfPosition, targetPosition, areaIndex, targetAreaIndex);
			auto positions = m_baseAstar->GetRoutePositions();
			m_baseAstar->ResetAstar();
			return positions;
		}

		std::vector<Vec3> ImpactMap::GetRoutePositions(
			const std::shared_ptr<NavGraphNode>& selfNode,
			const Vec3& targetPosition,
			const int areaIndex,
			const int targetAreaIndex
		) {
			m_baseAstar->SearchAstarStart(selfNode, targetPosition, areaIndex, targetAreaIndex);
			auto positions = m_baseAstar->GetRoutePositions();
			m_baseAstar->ResetAstar();
			return positions;
		}

		std::vector<Vec3> ImpactMap::GetRoutePositions(
			const std::shared_ptr<NavGraphNode>& selfNode,
			const std::shared_ptr<NavGraphNode>& targetNode,
			const int areaIndex,
			const int targetAreaIndex
		) {
			m_baseAstar->SearchAstarStart(selfNode, targetNode, areaIndex, targetAreaIndex);
			auto positions = m_baseAstar->GetRoutePositions();
			m_baseAstar->ResetAstar();
			return positions;
		}

		//�����̎��E���̃m�[�h���擾
		std::vector<std::shared_ptr<NavGraphNode>> ImpactMap::GetEyeRangeNodes(const Vec3& selfPosition, const std::shared_ptr<I_Impacter>& impacter) {
			std::vector<std::shared_ptr<NavGraphNode>> nodes;

			//�������g�̃m�[�h���擾
			auto selfNode = impacter->GetImpacterData().selfNode.lock();
			if (!selfNode) {
				return nodes;
			}

			//���E�f�[�^�̎擾
			auto impacterTrans = impacter->GetImpacterObject()->GetComponent<Transform>();
			auto eye = impacter->GetEyeSearchRange();
			EyeSearchRangeParametor eyeParam = eye->GetParametor();

			//Impacter�̕K�v�ȃf�[�^�̎擾
			auto impacterPosition = impacterTrans->GetPosition();
			auto forward = impacterTrans->GetForward();

			auto graph = m_baseAstar->CreateCopyGraph();	//�O���t�̃R�s�[

			auto nextNode = selfNode;
			while (true) {
				std::vector<std::shared_ptr<NavGraphNode>> currentAddNodes;
				auto currentNode = nextNode;
				auto edges = graph->GetEdges(currentNode->GetIndex());
				for (auto edge : edges) {
					auto toNode = graph->GetNode(edge->GetTo());
					if (!toNode->IsActive()) {	//�A�N�e�B�u�łȂ��Ȃ�continue
						continue;
					}

					//�m�[�h�����E���Ȃ�
					if (eye->IsInEyeRange(toNode->GetPosition())) {
						nodes.push_back(m_baseAstar->GetGraph()->GetNode(toNode->GetIndex()));
						currentAddNodes.push_back(toNode);
						toNode->SetIsActive(false);	//�m�[�h���A�N�e�B�u�ɂ���B
					}
					
					auto currentPosition = currentNode->GetPosition();
					auto direct = currentPosition + forward;
					nextNode = UtilityAstar::CalculateTargetDirectNode(*m_baseAstar.get(), currentNode, direct);
				}

				//�ǉ�����x������Ȃ��Ȃ�
				if (currentAddNodes.size() == 0) {
					break;
				}
			}

			return nodes;
		}

		//------------------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
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

		//�f�o�b�O-------------------------------------------------------------------------------------------

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
			//�f�o�b�O�h���[�̒ǉ�
			return;
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