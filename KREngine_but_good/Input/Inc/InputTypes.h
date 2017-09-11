#ifndef INCLUDED_INPUT_INPUTTYPES_H
#define INCLUDED_INPUT_INPUTTYPES_H

namespace Keys
{
	// Keyboard roll 1
	const uint32_t ESCAPE		= 0x01;
	const uint32_t F1			= 0x3B;
	const uint32_t F2			= 0x3C;
	const uint32_t F3			= 0x3D;
	const uint32_t F4			= 0x3E;
	const uint32_t F5			= 0x3F;
	const uint32_t F6			= 0x40;
	const uint32_t F7			= 0x41;
	const uint32_t F8			= 0x42;
	const uint32_t F9			= 0x43;
	const uint32_t F10			= 0x44;
	const uint32_t F11			= 0x57;
	const uint32_t F12			= 0x58;

	// Keyboard roll 2
	const uint32_t GRAVE		= 0x29;
	const uint32_t ONE			= 0x02;
	const uint32_t TWO			= 0x03;
	const uint32_t THREE		= 0x04;
	const uint32_t FOUR			= 0x05;
	const uint32_t FIVE			= 0x06;
	const uint32_t SIX			= 0x07;
	const uint32_t SEVEN		= 0x08;
	const uint32_t EIGHT		= 0x09;
	const uint32_t NINE			= 0x0A;
	const uint32_t ZERO			= 0x0B;
	const uint32_t MINUS		= 0x0C;
	const uint32_t EQUALS		= 0x0D;
	const uint32_t BACKSPACE	= 0x0E;

	// Keyboard roll 3
	const uint32_t TAB			= 0x0F;
	const uint32_t Q			= 0x10;
	const uint32_t W			= 0x11;
	const uint32_t E			= 0x12;
	const uint32_t R			= 0x13;
	const uint32_t T			= 0x14;
	const uint32_t Y			= 0x15;
	const uint32_t U			= 0x16;
	const uint32_t I			= 0x17;
	const uint32_t O			= 0x18;
	const uint32_t P			= 0x19;
	const uint32_t LBRACKET		= 0x1A;
	const uint32_t RBRACKET		= 0x1B;
	const uint32_t BACKSLASH	= 0x2B;

	// Keyboard roll 4
	const uint32_t A			= 0x1E;
	const uint32_t S			= 0x1F;
	const uint32_t D			= 0x20;
	const uint32_t F			= 0x21;
	const uint32_t G			= 0x22;
	const uint32_t H			= 0x23;
	const uint32_t J			= 0x24;
	const uint32_t K			= 0x25;
	const uint32_t L			= 0x26;
	const uint32_t SEMICOLON	= 0x27;
	const uint32_t APOSTROPHE	= 0x28;
	const uint32_t ENTER		= 0x1C;

	// Keyboard roll 5
	const uint32_t Z			= 0x2C;
	const uint32_t X			= 0x2D;
	const uint32_t C			= 0x2E;
	const uint32_t V			= 0x2F;
	const uint32_t B			= 0x30;
	const uint32_t N			= 0x31;
	const uint32_t M			= 0x32;
	const uint32_t COMMA		= 0x33;
	const uint32_t PERIOD		= 0x34;
	const uint32_t SLASH		= 0x35;

	// Lock keys
	const uint32_t CAPSLOCK		= 0x3A;
	const uint32_t NUMLOCK		= 0x45;
	const uint32_t SCROLLLOCK	= 0x46;

	// Numpad
	const uint32_t NUMPAD1		= 0x4F;
	const uint32_t NUMPAD2		= 0x50;
	const uint32_t NUMPAD3		= 0x51;
	const uint32_t NUMPAD4		= 0x4B;
	const uint32_t NUMPAD5		= 0x4C;
	const uint32_t NUMPAD6		= 0x4D;
	const uint32_t NUMPAD7		= 0x47;
	const uint32_t NUMPAD8		= 0x48;
	const uint32_t NUMPAD9		= 0x49;
	const uint32_t NUMPAD0		= 0x52;
	const uint32_t NUM_ADD		= 0x4E;
	const uint32_t NUM_SUB		= 0x4A;
	const uint32_t NUM_MUL		= 0x37;
	const uint32_t NUM_DIV		= 0xB5;
	const uint32_t NUM_ENTER	= 0x9C;
	const uint32_t NUM_DECIMAL	= 0x53;

	// Navigation keys
	const uint32_t INS			= 0xD2;
	const uint32_t DEL			= 0xD3;
	const uint32_t HOME			= 0xC7;
	const uint32_t END			= 0xCF;
	const uint32_t PGUP			= 0xC9;
	const uint32_t PGDN			= 0xD1;

	// Support keys
	const uint32_t LSHIFT		= 0x2A;
	const uint32_t RSHIFT		= 0x36;
	const uint32_t LCONTROL		= 0x1D;
	const uint32_t RCONTROL		= 0x9D;
	const uint32_t LALT			= 0x38;
	const uint32_t RALT			= 0xB8;
	const uint32_t LWIN			= 0xDB;
	const uint32_t RWIN			= 0xDC;
	const uint32_t SPACE		= 0x39;

	// Arrow keys
	const uint32_t UP			= 0xC8;
	const uint32_t DOWN			= 0xD0;
	const uint32_t LEFT			= 0xCB;
	const uint32_t RIGHT		= 0xCD;
}

namespace Mouse
{
	const uint32_t LBUTTON		= 0;
	const uint32_t RBUTTON		= 1;
	const uint32_t MBUTTON		= 2;
}

namespace GamePad
{
	const uint32_t BUTTON1		= 0;
	const uint32_t BUTTON2		= 1;
	const uint32_t BUTTON3		= 2;
	const uint32_t BUTTON4		= 3;
	const uint32_t BUTTON5		= 4;
	const uint32_t BUTTON6		= 5;
	const uint32_t BUTTON7		= 6;
	const uint32_t BUTTON8		= 7;
	const uint32_t BUTTON9		= 8;
	const uint32_t BUTTON10		= 9;
}

#endif // #ifndef INCLUDED_INPUT_INPUTTYPES_H