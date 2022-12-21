//=================================================
// Content     (ゲームの設定)(Line.h)
// Author     : 有田明玄
//=================================================
#ifndef _LINE_H_		//このマクロ定義がされてなかったら
#define _LINE_H_		//二重インクルード防止のマクロ定義
#include"main.h"
#include"Object.h"

//モデルの構造体
class CCollision :public CObject
{
public:
	explicit CCollision(int nPriority = 3);
	~CCollision();

	//プロトタイプ宣言
	HRESULT Init(void)override;
	void Uninit(void)override;
	void Update(void)override;
	void Draw(void)override;
	static CCollision* Create(D3DXVECTOR3 pos);
	void SetSiz(D3DXVECTOR3 size) { m_width = size; };
	D3DXVECTOR3 GetSiz() {return m_width; };
	void SetPos(D3DXVECTOR3(pos)) { m_pos = pos; };
	D3DXVECTOR3 GetPos( ) { return m_pos; };
	void SwichUse() { bUse = !bUse; };	//オンオフの切り替え
	void SetUse(bool use) { bUse = use; };	//オンオフの切り替え
	
private:
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_width;		//幅
	D3DXVECTOR3 m_rot;			//向き
	D3DXCOLOR	m_col;			//色
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	bool bUse;					//使用しているかどうか
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//インデックスバッファ

};

#endif



