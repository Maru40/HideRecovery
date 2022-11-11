/*!
@file HidePlacePatrolTree.h
@brief HidePlacePatrolTreeなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Behavior/SubBehaviorTree/SubBehaviorTreeBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	
	namespace Enemy {
		namespace Tuple {
			class I_Tupler;
		}
	}

	namespace maru {
		namespace Behavior {

			namespace Decorator {
				struct IsInEyeTarget_Parametor;
			}

			namespace Task {
				struct NearSeekMove_Parametor;
				struct NearAstarMove_Parametor;
			}

			namespace SubBehavior {

				//--------------------------------------------------------------------------------------
				/// バトル用のビヘイビアツリーのタイプ
				//--------------------------------------------------------------------------------------
				enum class ButtleTree_BihaviorType {
					FirstSelecter,		//初期セレクター

					AttackSelecter,		//攻撃(シーケンス(移動が必要なければ飛ばす))
						AttackMoveSelecter,	//攻撃中の移動セレクター
							NearMoveSelecter,	//近づくタスク(気づかれてなかったら、距離詰めたいかも)
								NearSeekMoveTask,	//直線的に近づく
								NearAstarMoveTask,	//Astarを使って近づく
							WanparoundSelecter,	//回り込む(見方が打ち合っていたら)
								RightSideMoveTask,	//右サイド
								LeftSideMoveTask,	//左サイド 
								BackSideMoveTask,	//裏取り
						ShotTask,			//撃つ

					EvadeSelecter,	//退避セレクター
						InducementStaySelecter,	//足止めセレクター
							InducementAttack,	//足止め攻撃
							//逃げる(自分をおとりに、攻撃させたくなる動き)
						EvadeMoveTask,	//退避移動


				};

				//--------------------------------------------------------------------------------------
				/// バトル用のビヘイビアツリーのパラメータのデコレータ群
				//--------------------------------------------------------------------------------------
				struct ButtleTree_DecoratorParametor {
					Decorator::IsInEyeTarget_Parametor* nearSeek_isInEyeParamPtr;	//近づくタスクの視界デコレータ
					Decorator::IsInEyeTarget_Parametor* nearAstar_isInEyeParamPtr;	//Astarで近づくタスクの視界デコレータ
					Decorator::IsInEyeTarget_Parametor* shot_isInEyeParamPtr;		//視界範囲制御デコレータ

					ButtleTree_DecoratorParametor();

					virtual ~ButtleTree_DecoratorParametor();
				};

				//--------------------------------------------------------------------------------------
				/// バトル用のビヘイビアツリーのパラメータのタスク群
				//--------------------------------------------------------------------------------------
				struct ButtleTree_TaskParametor {
					Task::NearSeekMove_Parametor* nearSeekParamPtr;
					Task::NearAstarMove_Parametor* nearAstarParamPtr;

					ButtleTree_TaskParametor();

					virtual ~ButtleTree_TaskParametor();
				};

				//--------------------------------------------------------------------------------------
				/// バトル用のビヘイビアツリーのパラメータ
				//--------------------------------------------------------------------------------------
				struct ButtleTree_Parametor {
					ButtleTree_TaskParametor taskParam;
					ButtleTree_DecoratorParametor decoratorParam;

					ButtleTree_Parametor();

					virtual ~ButtleTree_Parametor();
				};

				//--------------------------------------------------------------------------------------
				/// バトル用のビヘイビアツリー
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
					/// 攻撃系のエッジ生成
					/// </summary>
					void CreateAttackEdge();

					/// <summary>
					/// 逃走系のエッジ生成
					/// </summary>
					void CreateEvadeEdge();

					void CreateDecorator() override;

					void InitializeParametor();

					/// <summary>
					/// ショットタスク中にターゲット指定できないオブジェクトを取得
					/// </summary>
					/// <returns></returns>
					std::shared_ptr<GameObject> GetShotOutTarget();

					/// <summary>
					/// 同じリクエスタのタプルを削除
					/// </summary>
					template<class T>
					void RemoveTuples(std::vector<std::shared_ptr<T>>& tuples, std::shared_ptr<Enemy::Tuple::I_Tupler>& requester) {
						auto iter = tuples.begin();
						while (iter != tuples.end()) {
							if ((*iter)->GetRequester() == requester) {
								iter = tuples.erase(iter);
								continue;
							}
							iter++;
						}
					}

				};

			}
		}
	}
}


//メモ
//逃げた後に敵のHPと自分のHPに合わせて、
//反撃
//逃げる
//回り込む

//これらの選択が取れそう。