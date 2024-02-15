//==============================================================
//
//入力処理[input.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "input.h"

//静的メンバ変数宣言
LPDIRECTINPUT8 CInput::m_pInput = NULL;		//DirectInputオブジェクトへのポインタ

//==============================================================
//コンストラクタ
//==============================================================
CInput::CInput()
{
	//変数初期化
	m_pDevice = NULL;		//入力デバイスへのポインタ
}

//==============================================================
//デストラクタ
//==============================================================
CInput::~CInput()
{

}

//==============================================================
//初期化処理
//==============================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{//何も入力されてなかったら

		//DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//==============================================================
//終了処理
//==============================================================
void CInput::Uninit(void)
{
	//入力デバイスの破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();		//アクセス権を放棄
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	//DirectInputオブジェクトの破棄
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//==============================================================
//キーボードのコンストラクタ
//==============================================================
CInputKeyboard::CInputKeyboard()
{
	//変数初期化
	for (int nCntKey = 0; nCntKey < MAX_INPUT_KEY; nCntKey++)
	{
		m_aKeyStatePress[nCntKey] = NULL;		//キーボードのプレス情報
		m_aKeyStateTrigger[nCntKey] = NULL;		//キーボードのトリガー情報
		m_aKeyStateRelease[nCntKey] = NULL;		//キーボードのリリース情報
	}
}

//==============================================================
//キーボードのデストラクタ
//==============================================================
CInputKeyboard::~CInputKeyboard()
{

}

//==============================================================
//キーボードの初期化処理
//==============================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//初期化処理
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//入力デバイス（キーボード）の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//==============================================================
//キーボードの終了処理
//==============================================================
void CInputKeyboard::Uninit(void)
{
	//終了処理
	CInput::Uninit();
}

//==============================================================
//キーボードの更新処理
//==============================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[MAX_INPUT_KEY];		//キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < MAX_INPUT_KEY; nCntKey++)
		{
			m_aKeyStateRelease[nCntKey] = (m_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyStatePress[nCntKey];		//キーボードのリリース情報を保存
			m_aKeyStateTrigger[nCntKey] = (m_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];			//キーボードのトリガー情報を保存
			m_aKeyStatePress[nCntKey] = aKeyState[nCntKey];			//キーボードのプレス情報を保存
		}
	}
	else
	{
		m_pDevice->Acquire();						//キーボードへのアクセス権を獲得
	}
}

//==============================================================
//キーボードのプレス情報を取得
//==============================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyStatePress[nKey] & 0x80) ? true : false;
}

//==============================================================
//キーボードのトリガー情報を取得
//==============================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==============================================================
//キーボードのリリース情報を取得
//==============================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//==============================================================
//パッドのコンストラクタ
//==============================================================
CInputJoyPad::CInputJoyPad()
{
	//変数初期化
	for (int nCntKey = 0; nCntKey < NUM_PLAYER; nCntKey++)
	{
		m_aButtonStatePress[nCntKey] = {};		//ボタンのプレス情報
		m_aButtonStateTrigger[nCntKey] = {};	//ボタンのトリガー情報
		m_aButtonStateRelease[nCntKey] = {};	//ボタンのトリガー情報
	}
}

//==============================================================
//パッドのデストラクタ
//==============================================================
CInputJoyPad::~CInputJoyPad()
{

}

//==============================================================
//パッドの初期化処理
//==============================================================
HRESULT CInputJoyPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//初期化処理
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//xinputのステートの設定
	XInputEnable(true);

	for (int nCount = 0; nCount < NUM_PLAYER; nCount++)
	{
		//メモリーをクリアする
		memset(&m_aButtonStatePress[nCount], 0, sizeof(XINPUT_STATE));
		memset(&m_aButtonStateTrigger[nCount], 0, sizeof(XINPUT_STATE));
		memset(&m_aButtonStateRelease[nCount], 0, sizeof(XINPUT_STATE));
	}

	return S_OK;
}

//==============================================================
//パッドの終了処理
//==============================================================
void CInputJoyPad::Uninit(void)
{
	//xinputのステートの設定
	XInputEnable(false);

	//終了処理
	CInput::Uninit();
}

//==============================================================
//パッドの更新処理
//==============================================================
void CInputJoyPad::Update(void)
{
	XINPUT_STATE aGamepadState[NUM_PLAYER];
	static int nCntSpece;
	int nCntPad;

	for (nCntPad = 0; nCntPad < NUM_PLAYER; nCntPad++)
	{
		//入力デバイスからデータを取得
		if (XInputGetState(nCntPad, &aGamepadState[nCntPad]) == ERROR_SUCCESS)
		{
			m_aButtonStateTrigger[nCntPad].Gamepad.wButtons =
				~m_aButtonStatePress[nCntPad].Gamepad.wButtons
				& aGamepadState[nCntPad].Gamepad.wButtons;			//トリガー

			m_aButtonStateRelease[nCntPad].Gamepad.wButtons =
				((m_aButtonStatePress[nCntPad].Gamepad.wButtons
					^ aGamepadState[nCntPad].Gamepad.wButtons)
					& m_aButtonStatePress[nCntPad].Gamepad.wButtons);	//リリース

			m_aButtonStatePress[nCntPad] = aGamepadState[nCntPad];		//プレス
		}
	}
}

//==============================================================
//パッドのプレス情報を取得
//==============================================================
bool CInputJoyPad::GetPress(int Key, int nPlayer)
{
	return (m_aButtonStatePress[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//==============================================================
//パッドRTのプレス情報を取得
//==============================================================
float CInputJoyPad::GetPressRT(int nPlayer)
{
	float per = (float)m_aButtonStatePress[nPlayer].Gamepad.bRightTrigger / 255.0f;

	return (per);
}

//==============================================================
//パッドLTのプレス情報を取得
//==============================================================
float CInputJoyPad::GetPressLT(int nPlayer)
{
	float per = (float)m_aButtonStatePress[nPlayer].Gamepad.bLeftTrigger / 255.0f;

	return (per);
}

//==============================================================
//ゲームパッドRXのプレス情報を取得
//==============================================================
D3DXVECTOR3 CInputJoyPad::GetPressRX(int nPlayer)
{
	D3DXVECTOR3 Data;
	float fMan = 7000.0f / (float)(SHRT_MAX);
	float fMin = 7000.0f / (float)(SHRT_MIN);

	Data = D3DXVECTOR3((float)m_aButtonStatePress[nPlayer].Gamepad.sThumbRX, (float)m_aButtonStatePress[nPlayer].Gamepad.sThumbRY, 0.0f) / SHRT_MAX;

	if (fMan >= Data.x && fMin <= Data.x)
	{
		Data = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	return(Data);
}

//==============================================================
//ゲームパッドLXのプレス情報を取得
//==============================================================
D3DXVECTOR3 CInputJoyPad::GetPressLX(int nPlayer)
{
	D3DXVECTOR3 Data;
	float fMan = 7000.0f / (float)(SHRT_MAX);
	float fMin = 7000.0f / (float)(SHRT_MIN);

	Data = D3DXVECTOR3((float)m_aButtonStatePress[nPlayer].Gamepad.sThumbLX, (float)m_aButtonStatePress[nPlayer].Gamepad.sThumbLY, 0.0f) / SHRT_MAX;

	if (fMan >= Data.x && fMin <= Data.x)
	{
		Data = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	return(Data);
}

//==============================================================
//ゲームパッドトリガー情報を取得
//==============================================================
bool CInputJoyPad::GetTrigger(BUTTON Key, int nPlayer)
{
	return (m_aButtonStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//==============================================================
//ゲームパッドリリース情報を取得
//==============================================================
bool CInputJoyPad::GetRelease(BUTTON Key, int nPlayer)
{
	return (m_aButtonStateRelease[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}