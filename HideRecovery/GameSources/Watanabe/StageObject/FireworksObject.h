#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"
#include "../Interface/I_Performable.h"

namespace basecross {
	class FireworksObject :public StageObjectBase, public I_Performable {
	public:
		FireworksObject(const shared_ptr<Stage>& stage);

		void OnCreate()override;

		void Start()override;
		void Reset()override;
	};
}
