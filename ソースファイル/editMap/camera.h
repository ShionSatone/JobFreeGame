//==============================================================
//
//カメラ処理[camera.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _CAMERA_H_		//このマクロ定義がされていなかったら
#define _CAMERA_H_		//2重インクルード防止のマクロを定義する

#include"main.h"

//カメラクラスの定義
class CCamera
{
public:
	CCamera();		//コンストラクタ
	~CCamera();		//デストラクタ

	HRESULT Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void SetCamera(void);	//設定処理

	void SetPositionVY(float posY) { m_posV.y = posY; }		//視点位置Y設定
	void SetPositionRY(float posY) { m_posR.y = posY; }		//注視点位置Y設定
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }		//向き設定
	D3DXVECTOR3 GetRotation(void) { return m_rot; }			//向き取得
	bool GetMode(void) { return m_bProject; }				//投影方法取得

	void Control(void);		//移動処理
	void RotCorrection(void);	//向きの補正処理

private:
	static bool m_bStart;		//実行してるかどうか

	D3DXMATRIX m_mtxView;			//ビューマトリックス
	D3DXMATRIX m_mtxProjection;		//プロジェクションマトリックス

	D3DXVECTOR3 m_posV;			//視点
	D3DXVECTOR3 m_posR;			//注視点
	D3DXVECTOR3 m_vecU;			//上方向ベクトル
	D3DXVECTOR3 m_rot;			//向き

	D3DXVECTOR3 m_posVDest;		//目的の視点
	D3DXVECTOR3 m_posRDest;		//目的の注視点

	D3DXVECTOR3 m_moveV;		//視点の移動量
	D3DXVECTOR3 m_moveR;		//注視点の移動量
	float m_fRotMove;			//現在の方向(角度)
	float m_fRotDest;			//目標の方向(角度)
	float m_fRotDiff;			//目標の方向までの差分

	bool m_bProject;				//投影方法の切り替え
};

//カメラの構造体
//typedef struct
//{
//	
//	
//	float fLength;				//長さ
//	D3DVIEWPORT9 viewport;		//ビューポート
//} Camera;

//プロトタイプ宣言

//void SetCamera(int nIdex);
//Camera *GetCamera(void);

#endif
