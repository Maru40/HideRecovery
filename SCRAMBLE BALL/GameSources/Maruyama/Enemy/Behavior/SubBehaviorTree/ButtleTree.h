/*!
@file HidePlacePatrolTree.h
@brief HidePlacePatrolTree�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Behavior/SubBehaviorTree/SubBehaviorTreeBase.h"

namespace basecross {

	namespace maru {
		namespace Behavior {

			namespace Decorator {
				struct IsInEyeTarget_Parametor;
			}

			namespace Task {
				struct NearSeekMove_Parametor;
			}

			namespace SubBehavior {

				//--------------------------------------------------------------------------------------
				/// �o�g���p�̃r�w�C�r�A�c���[�̃^�C�v
				//--------------------------------------------------------------------------------------
				enum class ButtleTree_BihaviorType {
					FirstSelecter,		//�����Z���N�^�[

					AttackSelecter,		//�U��(�V�[�P���X(�ړ����K�v�Ȃ���Δ�΂�))
						AttackMoveSelecter,	//�U�����̈ړ��Z���N�^�[
							NearMoveSelecter,	//�߂Â��^�X�N(�C�Â���ĂȂ�������A�����l�߂�������)
								NearSeekMoveTask,	//�����I�ɋ߂Â�
								NearAstarMoveTask,	//Astar���g���ċ߂Â�
							WanparoundSelecter,	//��荞��(�������ł������Ă�����)
								RightSideMoveTask,	//�E�T�C�h
								LeftSideMoveTask,	//���T�C�h 
								BackSideMoveTask,	//�����
						ShotTask,			//����

					EvadeSelecter,	//�ޔ��Z���N�^�[
						InducementStaySelecter,	//���~�߃Z���N�^�[
							InducementAttack,	//���~�ߍU��
							//������(���������Ƃ�ɁA�U�����������Ȃ铮��)
						EvadeMoveTask,	//�ޔ��ړ�


				};

				//--------------------------------------------------------------------------------------
				/// �o�g���p�̃r�w�C�r�A�c���[�̃p�����[�^�̃f�R���[�^�Q
				//--------------------------------------------------------------------------------------
				struct ButtleTree_DecoratorParametor {
					Decorator::IsInEyeTarget_Parametor* shot_isInEyeParamPtr;	//���E�͈͐���f�R���[�^

					ButtleTree_DecoratorParametor();

					virtual ~ButtleTree_DecoratorParametor();
				};

				//--------------------------------------------------------------------------------------
				/// �o�g���p�̃r�w�C�r�A�c���[�̃p�����[�^�̃^�X�N�Q
				//--------------------------------------------------------------------------------------
				struct ButtleTree_TaskParametor {
					Task::NearSeekMove_Parametor* nearSeekParamPtr;

					ButtleTree_TaskParametor();

					virtual ~ButtleTree_TaskParametor();
				};

				//--------------------------------------------------------------------------------------
				/// �o�g���p�̃r�w�C�r�A�c���[�̃p�����[�^
				//--------------------------------------------------------------------------------------
				struct ButtleTree_Parametor {
					ButtleTree_TaskParametor taskParam;
					ButtleTree_DecoratorParametor decoratorParam;

					ButtleTree_Parametor();

					virtual ~ButtleTree_Parametor();
				};

				//--------------------------------------------------------------------------------------
				/// �o�g���p�̃r�w�C�r�A�c���[
				//--------------------------------------------------------------------------------------
				class ButtleTree : public SubBehaviorTreeBase<ButtleTree_BihaviorType>
				{
				public:
					using NodeType = ButtleTree_BihaviorType;
					using Parametor = ButtleTree_Parametor;

				private:
					Parametor m_param;

				public:
					ButtleTree(
						const std::shared_ptr<GameObject>& owner
					);

					virtual ~ButtleTree() = default;

				protected:
					void CreateNode() override;

					void CreateEdge() override;

					/// <summary>
					/// �U���n�̃G�b�W����
					/// </summary>
					void CreateAttackEdge();

					/// <summary>
					/// �����n�̃G�b�W����
					/// </summary>
					void CreateEvadeEdge();

					void CreateDecorator() override;

					void InitializeParametor();

				};

			}
		}
	}
}


//����
//��������ɓG��HP�Ǝ�����HP�ɍ��킹�āA
//����
//������
//��荞��

//�����̑I������ꂻ���B