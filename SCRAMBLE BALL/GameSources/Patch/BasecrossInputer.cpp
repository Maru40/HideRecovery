#include"BasecrossInputer.h"

namespace basecross
{
	const std::shared_ptr<itbs::Input::InputDevice> BasecrossInputer::GetInputDevice()
	{
		return App::GetApp()->GetMyInputDevice();
	}
}