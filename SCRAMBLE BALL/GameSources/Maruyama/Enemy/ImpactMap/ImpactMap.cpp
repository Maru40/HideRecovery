
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
		///	�l�p�`�f�[�^(��������͈͂ɗ��p)
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

		int Factory_ImpactMap::CalculateAreaIndex(const int widthCount, const int depthCount) {
			int width = widthCount / m_param.areaNodeCount.width;
			int depth = depthCount / m_param.areaNodeCount.depth;

			return width + (depth * 2);
		}

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
					astar->AddNode(position, ImpactData(CalculateAreaIndex(i, j))); //�m�[�h�̒ǉ�
				}
			}
		}

		void Factory_ImpactMap::CreateEdges(const std::shared_ptr<GraphAstar>& astar) {
			astar->AddEdges();
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
			astar->ClearEdges(); //�G�b�W�̃N���A
			CreateEdges(astar);   //�G�b�W�̐���
		}

		void Factory_ImpactMap::AddNodesEdges(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor) {
			m_param = parametor;
			AddNodes(astar, parametor);
			AddEdges(astar, parametor);
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

		void ImpactMap::AddImpactData(const Factory_Parametor& parametor) {
			//���݂͂��̊֐�����قƂ�ǂ̃m�[�h�𐶐����Ă���B
			AddNodes(parametor);
			AddEdges(parametor);
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
			//return;
			m_nodeDraw = GetStage()->AddGameObject<GameObject>()->AddComponent<AstarNodeDraw>(m_baseAstar);
			m_edgeDraw = GetStage()->AddGameObject<GameObject>()->AddComponent<AstarEdgeDraw>(m_baseAstar);
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