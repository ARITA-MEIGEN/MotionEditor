//=================================================
// Content     (ゲームの設定)(line.cpp)
// Author     : 有田明玄
//=================================================
//===========================
//インクルード
//===========================
#include"line.h"
#include"camera.h"
#include"shadow.h"
#include"Application.h"
#include"renderer.h"

//マクロ定義
#define NUM_BUFF	(8)		//1つの判定につき必要なバッファの数
#define NUM_LINE	(4)		//1つの段のバッファの数

CLine::CLine(int nPriority)
{
}

CLine::~CLine()
{
}

//===========================
//初期化処理
//===========================
HRESULT CLine::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_BUFF,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//情報の初期化
	bUse = false;

	//頂点座標へのポインタ
	VERTEX_3D*pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_BUFF; i++)	//上段
	{
		//頂点座標の設定
		//法線ベクトルの設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//色の設定
		pVtx[i].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, m_width.z / 2);
	pVtx[1].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, m_width.z / 2);
	pVtx[2].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, -m_width.z / 2);
	pVtx[3].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, -m_width.z / 2);

	pVtx[4].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, m_width.z / 2);
	pVtx[5].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, m_width.z / 2);
	pVtx[6].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
	pVtx[7].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, -m_width.z / 2);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================
//終了処理
//==============================
void CLine::Uninit(void)
{
	//バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}
//==============================
//更新処理
//==============================
void CLine::Update(void)
{
	//頂点座標へのポインタ
	VERTEX_3D*pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, m_width.z / 2);
	pVtx[1].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, m_width.z / 2);
	pVtx[2].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, -m_width.z / 2);
	pVtx[3].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, -m_width.z / 2);

	pVtx[4].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, m_width.z / 2);
	pVtx[5].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, m_width.z / 2);
	pVtx[6].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
	pVtx[7].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, -m_width.z / 2);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//==============================
//描画処理
//==============================
void CLine::Draw(void)
{
	if (bUse == true)
	{
		LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
		pDevice = CApplication::GetRenderer()->GetDevice();

		D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//ポリゴンの描画       
		pDevice->DrawPrimitive(D3DPT_LINELIST,	//ポリゴンの形(線)
			0,							//頂点の開始場所
			NUM_LINE*3);						//プリミティブの数
	}
}

//=====================
//生成
//=====================
CLine* CLine::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CLine*pLine = new CLine();

	if (pLine != nullptr)
	{
		pLine->Init();
		pLine->m_pos = pos;
		pLine->m_rot = pos;

	}

	return pLine;
}

