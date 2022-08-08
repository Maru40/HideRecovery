
/*!
@file NumbersCtrl.cpp
@brief NumbersCtrlクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "NumbersCtrl.h"
#include "DebugNumber.h"
#include "NumberObject.h"

#include "Mathf.h"

#include "DebugObject.h"

namespace basecross {

	void NumbersCtrl::CreateNumberObjs() {
		for (int i = 0; i < m_maxDigit; i++) {
			ex_weak_ptr<NumberObject> numberObj = GetStage()->Instantiate<NumberObject>(Vec3(0.0f), Quat::Identity(), GetGameObject());
			if (auto number = numberObj->GetComponent<maru::Number>()) {
				number->SetTexture(m_texture);
			}

			m_numberObjs.push_back(numberObj);
		}
	}

	void NumbersCtrl::ChangeDrawNumbers() {
		int index = 0;
		bool isReview = true;  //審査するかどうか
		for (auto& numberObj : m_numberObjs) {
			if (index == m_numberObjs.size() - 1) { //一番下の桁だったら処理をしない
				break;
			}

			auto numberComp = numberObj->GetComponent<maru::Number>();
			auto num = numberComp->GetNumber();

			//一度0以外がでたらそれ以降は表示する。
			if (num == 0 && isReview) {
				numberObj->SetDrawActive(false);
			}
			else {
				numberObj->SetDrawActive(true);
				isReview = false;
			}

			index++;
		}
	}

	void NumbersCtrl::SetNumbersPosition() {
		Vec3 scale = transform->GetScale() / static_cast<float>(m_maxDigit);
		Vec3 basePosition(0.0f);
		Vec3 offset(scale.x, 0.0f, 0.0f);

		int index = 0;
		for (auto& numberObj : m_numberObjs) {
			if (!numberObj->GetDrawActive()) {  //表示されていなかったら
				continue;
			}

			auto createPosition = basePosition + (offset * (float)index++);
			auto numberTrans = numberObj->GetComponent<Transform>();
			numberTrans->SetPosition(createPosition);
			numberTrans->SetScale(scale);
		}
	}

	void NumbersCtrl::OnCreate() {
		CreateNumberObjs();

		SetValue(m_value);
	}

	void NumbersCtrl::OnDestroy() {
		for (auto& number : m_numberObjs) {
			if (number) {
				GetStage()->RemoveGameObject<GameObject>(number.GetShard());
			}
		}

		m_numberObjs.clear();
	}

	void NumbersCtrl::ChangeIntegerNumber(const int value) {
		int place = static_cast<int>(pow(10, m_numberObjs.size() - 1));

		for (auto& numberObj : m_numberObjs) {
			if (!numberObj) {  //nullだったら処理を飛ばす。
				continue;
			}

			int integerValue = static_cast<int>(m_value);
			int num = integerValue / place % 10;  //取り出したい桁を取得
			place /= 10;

			auto numberComp = numberObj->GetComponent<maru::Number>();
			numberComp->SetNumber(num);
		}
	}

	void NumbersCtrl::ChangeFloatNumber(const float value) {

	}

	//--------------------------------------------------------------------------------------
	///	アクセッサ
	//--------------------------------------------------------------------------------------

	void NumbersCtrl::SetValue(const float value) {
		//数字のセット
		int place = static_cast<int>(pow(10, m_numberObjs.size() - 1));
		float maxValue = static_cast<float>(place * 10) - 1;

		m_value = maru::Mathf::Clamp(value, 0.0f, maxValue); //最大数チェック

		ChangeIntegerNumber(static_cast<int>(m_value));
		ChangeFloatNumber(m_value);

		ChangeDrawNumbers();
		SetNumbersPosition();
	}

	float NumbersCtrl::GetValue() const noexcept {
		return m_value;
	}

	void NumbersCtrl::SetColor(const Col4& color) {
		for (auto& numberObject : m_numberObjs) {
			numberObject->GetComponent<maru::Number::DrawComp>()->SetDiffuse(color);
		}
	}

	void NumbersCtrl::SetTexture(const wstring& texture) {
		m_texture = texture;

		for (auto& numberObject : m_numberObjs) {
			numberObject->GetComponent<maru::Number>()->SetTexture(texture);
		}
	}
}

//endbasecross