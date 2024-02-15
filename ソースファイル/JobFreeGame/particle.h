//==============================================================
//
//パーティクルの処理[effect.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _PARTICLE_H_		//このマクロ定義がされていなかったら
#define _PARTICLE_H_		//2重インクルード防止のマクロを定義する

#include "object.h"

//パーティクルクラス
class CParticle : public CObject
{
public:

	CParticle();	//コンストラクタ
	~CParticle();	//デストラクタ

	static CParticle *Create(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);		//生成処理

	HRESULT Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理

	void Set(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);		//設定処理
	//void GenerateNone(void);		//何もなし
	void GenerateWater(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);		//水
	void GenerateIce(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);			//氷
	void GenerateVapor(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);		//蒸気
	void GenerateExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);	//爆発
	void GenerateLife(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);	//回復
	void GenerateJump(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);	//ジャンプ
	void GenerateMove(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius);	//歩き

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_move;		//移動量
	D3DXCOLOR m_col;		//色
	float m_fRadius;		//半径（大きさ）
	float m_fAngle;			//角度
	float m_fMove;			//移動量
	int m_nLife;			//パーティクルが消えるまでの時間
	int m_type;	//パーティクルの種類

};

#endif