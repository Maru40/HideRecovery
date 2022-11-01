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

			namespace SubBehavior {

				//--------------------------------------------------------------------------------------
				/// �o�g���p�̃r�w�C�r�A�c���[
				//--------------------------------------------------------------------------------------
				enum class ButtleTree_BihaviorType {
					FirstSelecter,		//�����Z���N�^�[

					AttackSelecter,		//�U��(�V�[�P���X(�ړ����K�v�Ȃ���Δ�΂�))
						AttackMoveSelecter,	//�U�����̈ړ��Z���N�^�[
							NearSeekMoveTask,	//�߂Â��^�X�N(�C�Â���ĂȂ�������A�����l�߂�������)
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
				/// �o�g���p�̃r�w�C�r�A�c���[
				//--------------------------------------------------------------------------------------
				class ButtleTree : public SubBehaviorTreeBase<ButtleTree_BihaviorType>
				{
				public:
					using NodeType = ButtleTree_BihaviorType;

				private:


				public:
					ButtleTree(const std::shared_ptr<GameObject>& owner);

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