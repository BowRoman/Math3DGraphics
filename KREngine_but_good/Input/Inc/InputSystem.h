#ifndef INCLUDED_INPUT_INPUTSYSTEM_H
#define INCLUDED_INPUT_INPUTSYSTEM_H

namespace Input {

class InputSystem
{
	InputSystem(const InputSystem&) = delete;
	InputSystem& operator=(const InputSystem&) = delete;

public:
	static void StaticInitialize(HWND window);
	static void StaticTerminate();
	static InputSystem* Get();

public:
	InputSystem();
	~InputSystem();

	void Initialize(HWND window);
	void Terminate();

	void Update();

	bool IsKeyDown(uint32_t key) const;
	bool IsKeyPressed(uint32_t key) const;

	bool IsMouseDown(uint32_t button) const;
	bool IsMousePressed(uint32_t button) const;

	int GetMouseScreenX() const;
	int GetMouseScreenY() const;

	bool IsMouseLeftEdge() const;
	bool IsMouseRightEdge() const;
	bool IsMouseTopEdge() const;
	bool IsMouseBottomEdge() const;

	int GetMouseMoveX() const;
	int GetMouseMoveY() const;
	int GetMouseMoveZ() const;

	bool IsGamePadButtonDown(uint32_t button) const;
	bool IsGamePadButtonPressed(uint32_t button) const;

	bool IsDPadUp() const;
	bool IsDPadDown() const;
	bool IsDPadLeft() const;
	bool IsDPadRight() const;

	float GetLeftAnalogX() const;
	float GetLeftAnalogY() const;
	float GetRightAnalogX() const;
	float GetRightAnalogY() const;

private:
	friend BOOL CALLBACK EnumGamePadCallback(const DIDEVICEINSTANCE* pDIDeviceInstance, VOID* pContext);

	void UpdateKeyboard();
	void UpdateMouse();
	void UpdateGamePad();

	IDirectInput8* mDirectInput;
	IDirectInputDevice8* mKeyboardDevice;
	IDirectInputDevice8* mMouseDevice;
	IDirectInputDevice8* mGamePadDevice;

	static const uint32_t kKeyBufferSize = 256;
	uint8_t mCurrKeyBuffer[kKeyBufferSize];
	uint8_t mPrevKeyBuffer[kKeyBufferSize];

	DIMOUSESTATE mCurrMouseState;
	DIMOUSESTATE mPrevMouseState;

	DIJOYSTATE mCurrGamePadState;
	DIJOYSTATE mPrevGamePadState;

	int mMouseX;
	int mMouseY;

	int mScreenWidth;
	int mScreenHeight;

	bool mInitialized;
};

} // namespace Input

#endif // #ifndef INCLUDED_INPUT_INPUTSYSTEM_H