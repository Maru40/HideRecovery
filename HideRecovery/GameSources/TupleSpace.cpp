
/*!
@file TupleSpace.cpp
@brief TupleSpaceのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TupleSpace.h"

namespace basecross {

	namespace Enemy {

		namespace Tuple {
			//--------------------------------------------------------------------------------------
			/// タプルのインターフェース
			//--------------------------------------------------------------------------------------

			bool I_Tuple::operator== (const I_Tuple& other) {
				return this == &other;
			}

			//--------------------------------------------------------------------------------------
			/// 希望行動の基底クラス
			//--------------------------------------------------------------------------------------

			TupleActionBase::TupleActionBase(
				const std::shared_ptr<GameObject>& acter, 
				const std::shared_ptr<GameObject>& requester, 
				const float value
			) :
				acter(acter),
				requester(requester),
				value(value)
			{}

			//--------------------------------------------------------------------------------------
			/// リクエストの基底クラス
			//--------------------------------------------------------------------------------------

			TupleRequestBase::TupleRequestBase(
				const std::shared_ptr<GameObject>& requester, 
				const float value
			) :
				requester(requester),
				value(value)
			{}

			bool TupleRequestBase::operator== (const TupleRequestBase& other) {
				if (this->requester.lock() == other.requester.lock() &&
					this->value == other.value)
				{
					return true;
				}

				return false;
			}

			//--------------------------------------------------------------------------------------
			/// ターゲットを見つけたことを知らせるタプル
			//--------------------------------------------------------------------------------------

			FindTarget::FindTarget(const std::shared_ptr<GameObject>& requester, const std::shared_ptr<GameObject>& target, const float value):
				TupleRequestBase(requester, value), target(target)
			{}

			bool FindTarget::operator== (const FindTarget& other) {
				if (this->requester.lock() == other.requester.lock() &&
					this->target.lock() == other.target.lock() &&
					this->value == other.value)
				{
					return true;
				}

				return false;
			}
		}
	}
}