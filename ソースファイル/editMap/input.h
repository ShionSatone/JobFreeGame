//==============================================================
//
//入力処理[input.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _INPUT_H_		//このマクロ定義がされていなかったら
#define _INPUT_H_		//2重インクルード防止のマクロを定義する

#include "main.h"
#include "Xinput.h"

#pragma comment(lib, "xinput.lib")

//マクロ定義
#define MAX_INPUT_KEY			(256)		//キーの最大数
#define NUM_PLAYER		(4)			//プレイヤーの数

//入力クラスの定義
class CInput
{
public:
	CInput();		//コンストラクタ
	virtual ~CInput();		//デストラクタ

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化処理
	virtual void Uninit(void);			//終了処理
	virtual void Update(void) = 0;		//更新処理

protected:
	static LPDIRECTINPUT8 m_pInput;		//DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevice;		//入力デバイスへのポインタ
};

//キーボードクラスの定義
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();		//コンストラクタ
	~CInputKeyboard();		//デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化処理
	void Uninit(void);			//終了処理
	void Update(void);			//更新処理

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);

private:
	BYTE m_aKeyStatePress[MAX_INPUT_KEY];			//キーボードのプレス情報
	BYTE m_aKeyStateTrigger[MAX_INPUT_KEY];	//キーボードのトリガー情報
	BYTE m_aKeyStateRelease[MAX_INPUT_KEY];	//キーボードのリリース情報
};

//パッドクラスの定義
class CInputJoyPad : public CInput
{
public:

	//ボタン配置の列挙
	typedef enum
	{
		BUTTON_UP = 0,		//十字キー上
		BUTTON_DOWN,		//十字キー下
		BUTTON_LEFT,		//十字キー左
		BUTTON_RIGHT,		//十字キー右
		BUTTON_START,		//スタートボタン
		BUTTON_BACK,		//バックボタン
		BUTTON_LSTICK,		//左スティック押し込み
		BUTTON_RSTICK,		//右スティック押し込み
		BUTTON_LB,			//LB
		BUTTON_RB,			//RB
		BUTTON_01,			//
		BUTTON_02,			//
		BUTTON_A,			//Aボタン
		BUTTON_B,			//Bボタン
		BUTTON_X,			//Xボタン
		BUTTON_Y,			//Yボタン
		BUTTON00,

		BUTTON_MAX
	} BUTTON;

	CInputJoyPad();		//コンストラクタ
	~CInputJoyPad();	//デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化処理
	void Uninit(void);			//終了処理
	void Update(void);			//更新処理

	bool GetPress(int Key, int nPlayer);
	float GetPressRT(int nPlayer);
	float GetPressLT(int nPlayer);
	D3DXVECTOR3 GetPressRX(int nPlayer);
	D3DXVECTOR3 GetPressLX(int nPlayer);

	bool GetTrigger(BUTTON Key, int nPlayer);
	bool GetRelease(BUTTON Key, int nPlayer);

private:
	XINPUT_STATE m_aButtonStatePress[NUM_PLAYER];			//ボタンのプレス情報
	XINPUT_STATE m_aButtonStateTrigger[NUM_PLAYER];			//ボタンのトリガー情報
	XINPUT_STATE m_aButtonStateRelease[NUM_PLAYER];			//ボタンのトリガー情報
};

#endif