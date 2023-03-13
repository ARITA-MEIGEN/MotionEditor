//=================================================
// Content     (ゲームの設定)(Line.h)
// Author     : 有田明玄
//=================================================
#ifndef _LINE_H_		//このマクロ定義がされてなかったら
#define _LINE_H_		//二重インクルード防止のマクロ定義
#include"main.h"
#include"Object.h"

//モデルの構造体
class CLine :public CObject
{
public:
	explicit CLine(int nPriority = 3);
	~CLine();

	//プロトタイプ宣言
	HRESULT Init(void)override;
	void Uninit(void)override;
	void Update(void)override;
	void Draw(void)override;
	static CLine* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetAll(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 max, D3DXVECTOR3 min);

private:
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_width;		//幅
	D3DXVECTOR3 m_rot;			//向き
	D3DXCOLOR	m_col;			//色
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	bool bUse;					//使用しているかどうか
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファ

};

#endif


