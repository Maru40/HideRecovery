#include"PlayerInputer.h"

namespace basecross
{
	int PlayerInputer::m_rotationSensitivityLevel = 5;
	int PlayerInputer::m_mouseSensitivityLevel = 5;

	std::shared_ptr<PlayerInputer> PlayerInputer::m_playerInputer;

	PlayerInputer::PlayerInputer() :
		m_inputDevice(App::GetApp()->GetMyInputDevice())
	{
	}

	Vec2 PlayerInputer::GetMoveDirection()
	{
		static const auto& inputDevice = App::GetApp()->GetMyInputDevice();
		static const auto& keyBoard = inputDevice->GetKeyBoard();
		static const auto& xinput = inputDevice->GetXInputGamePad();

		Vec2 direction;

		if (keyBoard.IsInputPush(KeyCode::D))
		{
			direction.x += 1;
		}

		if (keyBoard.IsInputPush(KeyCode::A))
		{
			direction.x -= 1;
		}

		if (keyBoard.IsInputPush(KeyCode::W))
		{
			direction.y += 1;
		}

		if (keyBoard.IsInputPush(KeyCode::S))
		{
			direction.y -= 1;
		}


		direction.x += xinput.GetLeftThumb().x;
		direction.y += xinput.GetLeftThumb().y;

		direction.x = MyMath::Clamp(direction.x, -1.0f, 1.0f);

		direction.y = MyMath::Clamp(direction.y, -1.0f, 1.0f);

		if (direction.length() > 1.0f)
		{
			direction.normalize();
		}

		return direction;
	}

	Vec2 PlayerInputer::GetCameraRotation()
	{
		static const auto& inputDevice = App::GetApp()->GetMyInputDevice();
		static const auto& keyBoard = inputDevice->GetKeyBoard();
		static const auto& xinput = inputDevice->GetXInputGamePad();

		Vec2 rotation;

		rotation.x += xinput.GetRightThumb().x;
		rotation.y += xinput.GetRightThumb().y;

		auto mouseMovePoint = inputDevice->GetMouseState().GetMouseMove();
		auto mouseMove = Vec2((float)mouseMovePoint.x, (float)mouseMovePoint.y);

		rotation += mouseMove / MOUSE_SENSITIVITY_DEFAULT_VALUE * 
			(m_mouseSensitivityLevel / static_cast<float>(MAX_SENSITIVITY_LEVEL));

		rotation *= m_rotationSensitivityLevel / static_cast<float>(MAX_SENSITIVITY_LEVEL);

		rotation *= XMConvertToRadians(12);

		return rotation;

	}

	Vec2 PlayerInputer::GetRSticVec() {
		static const auto& inputDevice = App::GetApp()->GetMyInputDevice();
		static const auto& keyBoard = inputDevice->GetKeyBoard();
		static const auto& xinput = inputDevice->GetXInputGamePad();

		Vec2 rotation;

		rotation.x += xinput.GetRightThumb().x;
		rotation.y += xinput.GetRightThumb().y;

		return rotation;
	}

	Vec3 PlayerInputer::GetRSticVec3() {
		static const auto& inputDevice = App::GetApp()->GetMyInputDevice();
		static const auto& keyBoard = inputDevice->GetKeyBoard();
		static const auto& xinput = inputDevice->GetXInputGamePad();

		Vec3 rotation(0.0f);

		rotation.x += xinput.GetRightThumb().x;
		rotation.z += xinput.GetRightThumb().y;

		return rotation;
	}

	void PlayerInputer::SetMouseSensitivityLevel(const int level)
	{
		m_mouseSensitivityLevel = level;
	}

	void PlayerInputer::SetRotationSensitivityLevel(const int level)
	{
		m_rotationSensitivityLevel = level;
	}

	int PlayerInputer::GetMouseSensitivityLevel()
	{
		return m_mouseSensitivityLevel;
	}

	int PlayerInputer::GetRotationSensitivityLevel()
	{
		return m_rotationSensitivityLevel;
	}

	bool PlayerInputer::IsDecision()
	{
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputDown(KeyCode::F) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputDown(XInputCode::A);
	}

	bool PlayerInputer::IsCancel()
	{
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputDown(KeyCode::X) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputDown(XInputCode::B);
	}

	bool PlayerInputer::IsChangeStance()
	{
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputDown(KeyCode::C) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputDown(XInputCode::RightThumb);
	}

	bool PlayerInputer::IsMenuDown()
	{
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputDown(KeyCode::Tab) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputDown(XInputCode::Start);
	}

	bool PlayerInputer::IsDashPush()
	{
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputPush(KeyCode::LeftShift) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputPush(XInputCode::LeftShoulder);
	}

	bool PlayerInputer::IsUpChoices()
	{
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputDown(KeyCode::UpArrow) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputDown(XInputCode::Up);
	}

	bool PlayerInputer::IsDownChoices()
	{
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputDown(KeyCode::DownArrow) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputDown(XInputCode::Down);
	}

	bool PlayerInputer::IsDash() {
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputPush(KeyCode::LeftShift); //|| 
			//App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputDown(XInputCode::LeftShoulder);
	}

	

	bool PlayerInputer::IsJetPumpPush() {
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputPush(KeyCode::Enter);
	}

	bool PlayerInputer::IsJetPumpUp() {
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputUp(KeyCode::Enter);
	}

	bool PlayerInputer::IsChangeTargetLookCamera() {
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputDown(KeyCode::F) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputDown(XInputCode::LeftShoulder);
	}

	bool PlayerInputer::IsX() {
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputPush(KeyCode::X) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputPush(XInputCode::X);
	}

	bool PlayerInputer::IsY() {
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputPush(KeyCode::Y) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputPush(XInputCode::Y);
	}

	bool PlayerInputer::IsXDown() {
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputDown(KeyCode::X) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputDown(XInputCode::X);
	}

	bool PlayerInputer::IsYDown() {
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputDown(KeyCode::Y) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputDown(XInputCode::Y);
	}

	bool PlayerInputer::IsBDown() {
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputDown(KeyCode::B) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputDown(XInputCode::B);
	}

	bool PlayerInputer::IsXUp() {
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputUp(KeyCode::X) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputUp(XInputCode::X);
	}

	bool PlayerInputer::IsYUp() {
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputUp(KeyCode::Y) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputUp(XInputCode::Y);
	}

	bool PlayerInputer::IsUpUp() {
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputUp(KeyCode::UpArrow) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputUp(XInputCode::Up);
	}

	bool PlayerInputer::IsDownUp() {
		return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputUp(KeyCode::DownArrow) ||
			App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputUp(XInputCode::Down);
	}

	bool PlayerInputer::IsRBDown() {
		return App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputDown(XInputCode::RightShoulder);
	}

	bool PlayerInputer::IsRBUp() {
		return App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputUp(XInputCode::RightShoulder);
	}

	bool PlayerInputer::IsLBDown() {
		return App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputDown(XInputCode::LeftShoulder);
	}

	//bool PlayerInputer::IsDebugSlimePressRadChange() {
	//	return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputUp(KeyCode::P);
	//}

	//bool PlayerInputer::IsDebugHitStopModeChange() {
	//	return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputUp(KeyCode::H) ||
	//		App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputUp(XInputCode::LeftThumb);
	//}

	//bool PlayerInputer::IsDebugAstarDraw() {
	//	return App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputUp(KeyCode::L); //||       //キーボード
	//		//App::GetApp()->GetMyInputDevice()->GetXInputGamePad().IsInputUp(XInputCode::Back); //コントローラー
	//}

	std::shared_ptr<PlayerInputer> PlayerInputer::GetInstance()
	{
		if (!m_playerInputer)
		{
			m_playerInputer = std::make_shared<PlayerInputer>();
		}

		return m_playerInputer;
	}

	bool PlayerInputer::IsDesitionDown() const
	{
		return m_inputDevice->GetKeyBoard().IsInputDown(KeyCode::F) ||
			m_inputDevice->GetXInputGamePad().IsInputDown(XInputCode::A);
	}

	bool PlayerInputer::IsCancelDown() const
	{
		return m_inputDevice->GetKeyBoard().IsInputDown(KeyCode::X) ||
			m_inputDevice->GetXInputGamePad().IsInputDown(XInputCode::B);
	}

	bool PlayerInputer::IsUpDown() const
	{
		return m_inputDevice->GetKeyBoard().IsInputDown(KeyCode::UpArrow) ||
			m_inputDevice->GetXInputGamePad().IsInputDown(XInputCode::Up);
	}

	bool PlayerInputer::IsUp() const
	{
		return m_inputDevice->GetKeyBoard().IsInputPush(KeyCode::UpArrow) ||
			m_inputDevice->GetXInputGamePad().IsInputPush(XInputCode::Up);
	}

	bool PlayerInputer::IsDownDown() const
	{
		return m_inputDevice->GetKeyBoard().IsInputDown(KeyCode::DownArrow) ||
			m_inputDevice->GetXInputGamePad().IsInputDown(XInputCode::Down);
	}

	bool PlayerInputer::IsDown() const
	{
		return m_inputDevice->GetKeyBoard().IsInputPush(KeyCode::DownArrow) ||
			m_inputDevice->GetXInputGamePad().IsInputPush(XInputCode::Down);
	}

	bool PlayerInputer::IsLeftDown() const
	{
		return m_inputDevice->GetKeyBoard().IsInputDown(KeyCode::LeftArrow) ||
			m_inputDevice->GetXInputGamePad().IsInputDown(XInputCode::left);
	}

	bool PlayerInputer::IsLeft() const
	{
		return m_inputDevice->GetKeyBoard().IsInputPush(KeyCode::LeftArrow) ||
			m_inputDevice->GetXInputGamePad().IsInputPush(XInputCode::left);
	}

	bool PlayerInputer::IsRightDown() const
	{
		return m_inputDevice->GetKeyBoard().IsInputDown(KeyCode::RightArrow) ||
			m_inputDevice->GetXInputGamePad().IsInputDown(XInputCode::Right);
	}

	bool PlayerInputer::IsRight() const
	{
		return m_inputDevice->GetKeyBoard().IsInputPush(KeyCode::RightArrow) ||
			m_inputDevice->GetXInputGamePad().IsInputPush(XInputCode::Right);
	}
}