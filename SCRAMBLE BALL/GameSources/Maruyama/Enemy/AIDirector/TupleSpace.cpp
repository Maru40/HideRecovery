
/*!
@file TupleSpace.cpp
@brief TupleSpaceのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/MaruAction.h"
#include "TupleSpace.h"

#include "Watanabe/DebugClass/Debug.h"

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
				const std::shared_ptr<I_Tupler>& requester, 
				const float value
			) :
				m_requester(requester),
				m_value(value)
			{}

			bool TupleRequestBase::operator== (const TupleRequestBase& other) {
				if (m_requester.lock() == other.GetRequester() &&
					m_value == other.GetValue())
				{
					return true;
				}

				return false;
			}

			//--------------------------------------------------------------------------------------
			/// ターゲットを見つけたことを知らせるタプル
			//--------------------------------------------------------------------------------------

			FindTarget::FindTarget(const std::shared_ptr<I_Tupler>& requester, const std::shared_ptr<GameObject>& target, const float value):
				TupleRequestBase(requester, value), m_target(target)
			{}

			bool FindTarget::operator== (const FindTarget& other) {
				if (GetRequester() == other.GetRequester() &&
					GetTarget() == other.GetTarget() &&
					GetValue() == other.GetValue())
				{
					return true;
				}

				return false;
			}

			//--------------------------------------------------------------------------------------
			/// ターゲットとバトルすることをリクエストするタプル
			//--------------------------------------------------------------------------------------

			ButtleTarget::ButtleTarget(
				const std::shared_ptr<I_Tupler>& requester,
				const std::shared_ptr<GameObject>& target,
				const float value
			):
				TupleRequestBase(requester, value),
				target(target)
			{}

			bool ButtleTarget::operator==(const ButtleTarget& other) {
				if (GetRequester() == other.GetRequester() &&
					GetTarget() == other.GetTarget() &&
					GetValue() == other.GetValue())
				{
					return true;
				}

				return false;
			}

			//--------------------------------------------------------------------------------------
			/// バトルに遷移することをリクエストするタプル
			//--------------------------------------------------------------------------------------

			ButtleTransition::ButtleTransition(
				const std::shared_ptr<I_Tupler>& requester,
				const std::shared_ptr<GameObject>& target,
				const float value
			):
				TupleRequestBase(requester, value),
				m_target(target)
			{}

			bool ButtleTransition::operator==(const ButtleTransition& other) {
				if (GetRequester() == other.GetRequester() &&
					GetTarget() == other.GetTarget() &&
					GetValue() == other.GetValue())
				{
					return true;
				}

				return false;
			}

			//--------------------------------------------------------------------------------------
			/// ダメージを受けたことを伝えるタプル
			//--------------------------------------------------------------------------------------

			Damaged::Damaged(
				const std::shared_ptr<I_Tupler>& requester,
				const DamageData& data,
				const float value
			):
				TupleRequestBase(requester, value),
				m_damageData(data)
			{}

			bool Damaged::operator ==(const Damaged& other) {
				if (GetRequester() == other.GetRequester() &&
					GetDamageData().attacker == other.GetDamageData().attacker &&
					GetValue() == other.GetValue())
				{
					return true;
				}

				return false;
			}


			//--------------------------------------------------------------------------------------
			/// タプルスペース本体
			//--------------------------------------------------------------------------------------

			bool TupleSpace::RemoveAllNotifys(const std::shared_ptr<I_Tupler>& tupler) {
				std::vector<std::function<bool()>> removeFuncs;

				//削除候補を検索
				for (auto& pair : m_notifysMap) {
					for (auto& notify : pair.second) {
						if (notify->GetRequester() == tupler) {
							removeFuncs.push_back([&, notify]() { return RemoveNotify(notify); });
						}
					}
				}

				//削除処理
				for (auto& removeFunc : removeFuncs) {
					bool isRemove = removeFunc();
				}

				return !removeFuncs.empty();	//emptyなら削除がされてないことを示す。
			}
		}
	}
}