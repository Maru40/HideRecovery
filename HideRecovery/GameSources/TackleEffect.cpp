/*!
@file TackleEffect.cpp
@brief TackleEffectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EffectBase.h"
#include "TackleEffect.h"

#include "AuraBase.h"
#include "PlayerInputer.h"

namespace basecross {

	namespace Effect {

		Tackle::Tackle(const std::shared_ptr<GameObject>& objPtr)
			:Base(objPtr)
		{}

		void Tackle::OnCreate() {
			std::vector<CreateAuraData> datas = {
				CreateDefaultAura(),
				CreateMoveAura(),
			};

			CreateAuraObjects(datas);
			SetEffectsDrawActive(false);
		}

		CreateAuraData Tackle::CreateDefaultAura() {
			auto param = Aura_Parametor();

			param.sides = 30;        //頂点の数(多ければ綺麗なメッシュ)
			param.height = 0.6f;     //高さ
			param.topRadius = 0.0f;  //上の半径
			param.topColor = Col4(0.0f, 0.5f, 0.0f, 0.5f); //上の色
			param.bottomRadius = 2.5f;  //下の半径
			param.bottomColor = Col4(0.0f, 0.5f, 0.0f, 0.0f); //下の色
			param.texture = L"SquareSprite_TX";  //テクスチャネーム
			param.updateParam.speed = 0.0f; //UVスピード
			param.updateParam.direct = Vec2(0.0f, -1.0f); //UVMove方向

			//オフセットデータ
			OffsetData offsetData;
			offsetData.position = Vec3(0.0f, 0.5f, 1.0f);
			offsetData.rotation = Vec3(XM_PIDIV2, 0.0f, 0.0f);
			offsetData.scale = Vec3(1.0f, 1.0f, 1.0f);

			return CreateAuraData(param, offsetData);
		}

		CreateAuraData Tackle::CreateMoveAura() {
			auto param = Aura_Parametor();

			param.sides = 30;        //頂点の数(多ければ綺麗なメッシュ)
			param.height = 0.6f;     //高さ
			param.topRadius = 0.0f;  //上の半径
			param.topColor = Col4(0.0f, 0.5f, 0.0f, 1.0f); //上の色
			param.bottomRadius = 2.5f;  //下の半径
			param.bottomColor = Col4(0.0f, 0.5f, 0.0f, 0.0f); //下の色
			param.texture = L"LineH_TX";  //テクスチャネーム
			param.updateParam.speed = 1.0f; //UVスピード
			param.updateParam.direct = Vec2(0.0f, -1.0f); //UVMove方向

			//オフセットデータ
			OffsetData offsetData;
			offsetData.position = Vec3(0.0f, 0.5f, 1.0f);
			offsetData.rotation = Vec3(XM_PIDIV2, 0.0f, 0.0f);
			offsetData.scale = Vec3(1.0f, 1.0f, 1.0f);

			return CreateAuraData(param, offsetData);
		}

	}
}