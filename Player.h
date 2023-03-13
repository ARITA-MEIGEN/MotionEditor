//=============================================================================
//
// Xファイル読み込み
// Author : 有田明玄
//
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_

//インクルード
#include"main.h"
#include"Object.h"
#include"ObjectX.h"
#include"Model.h"

//前方宣言
class CShadow;
class CModel;
class CCollision;

//マクロ定義
#define MAX_MOTION		(20)	//モーションの数
#define MAX_KEY			(60)	//キーの総数
#define MAX_FRAME		(120)	//フレームの最大数
#define NUM_PARTS		(14)	//パーツの数
#define HOLD_TIME		(30)	//長押しして処理が通るまでの時間
#define SINGLE_STA		(0.01f)	//単体で動かす量
#define HOLD_STA		(0.05f)	//長押しで動かす量
#define MAX_COLLISION	(20)	//1つのパーツにつき設定できる判定の数
#define	MAX_POS			(50.0f)	//imguiで動かせる最大数


class CPlayer :public CObject
{
public:
	//キー要素
	struct KEY
	{
		float fPosX;
		float fPosY;
		float fPosZ;

		float fRotX;
		float fRotY;
		float fRotZ;

		D3DXVECTOR3 fTempPos;
		D3DXVECTOR3 fTempRot;

	};

	//キー情報
	struct KEY_SET 
	{
		KEY aKey[NUM_PARTS];					//モデルの数分座標を持つ
		int nFrame;								//再生時間
		CCollision* Collision[MAX_COLLISION];	//当たり判定
		int nNumCollision;						//1つのキーの当たり判定の数
		CCollision* HurtCol[MAX_COLLISION];		//やられ判定
		int nNumHurtCol;						//1つのキーの当たり判定の数
	};

	struct MOTION_SET
	{
		KEY_SET	aModelKey[MAX_KEY];		//キーの総数分持つ
		int		nNumKey;				//キーの総数(ファイルで読み込む)
		bool	bLoop;					//ループするかどうか
		bool	bControl;				//操作可能なモーションかどうか
		int		nCancelFrame;			//技のキャンセル猶予
		bool	bAtk;					//攻撃モーションかどうか(再生後にニュートラルに戻すかどうか)
		int		nHitStopTimer;			//ヒットストップの時間

	};

	enum PLAYER_MOTION
	{
		//地上
		PM_ST_NEUTRAL,		//ニュートラル
		PM_ST_MOVE,			//移動(しゃがみだけ無し)
		PM_ST_DASH,			//前ダッシュ
		PM_ST_BACKSTEP,		//バックステップ
		PM_ST_GUARD,		//ガード
		PM_ST_HIT,			//被弾
		PM_ST_LATTACK,		//弱攻撃
		PM_ST_MATTACK,		//中攻撃
		PM_ST_HATTACK,		//強攻撃
		PM_ST_HURT,			//やられモーション

		//空中
		PM_JP_NEUTRAL,		//垂直ジャンプ
		PM_JP_MOVELEFT	,	//移動(しゃがみだけ無し)
		PM_JP_MOVERIGHT,	//移動(しゃがみだけ無し)
		PM_JP_DASH,			//前ダッシュ
		PM_JP_GUARD,		//ガード
		PM_JP_HIT,			//被弾
		PM_JP_LATTACK,		//弱攻撃
		PM_JP_MATTACK,		//中攻撃
		PM_JP_HATTACK,		//強攻撃
		PM_JP_HURT,			//やられモーション

		//しゃがみ
		PM_CR_NEUTRAL,		//ニュートラル
		PM_CR_MOVE,			//移動(しゃがみだけ無し)
		PM_CR_GUARD,		//ガード
		PM_CR_HIT,			//被弾
		PM_CR_LATTACK,		//弱攻撃
		PM_CR_MATTACK,		//中攻撃
		PM_CR_HATTACK,		//強攻撃
		PM_CR_HURT,			//やられモーション

		//必殺技
		PM_236L,			//弱波動
		PM_236M,			//中波動
		PM_236H,			//強波動

		PM_214L,			//弱竜巻
		PM_214M,			//中竜巻
		PM_214H,			//強竜巻

		PM_623L,			//弱昇竜
		PM_623M,			//中昇竜
		PM_623H,			//強昇竜

		PM_ST_DIE,			//死
		PM_CR_DIE,			//死
		PM_JP_DIE,			//死

		PM_DOWN,			//死亡&ダウン時モーション
		PM_STANDUP,			//起き上がり
		PM_FORWARDSLOW,		//前投げ
		PM_BACKSLOW,		//後ろ投げ
		PM_MAX
	};

	explicit CPlayer(int nPriority = 3);
	~CPlayer();
	//プロトタイプ宣言
	HRESULT			Init()override;
	void			Uninit(void)override;
	void			Update(void)override;
	void			Draw(void)override;
	bool			ControlPlayer(void);			//プレイヤーの操作
	static CPlayer*	Create();						//プレイヤー生成
	D3DXMATRIX		GetMtx();						//マトリックスの取得
	void			ReadMotion();					//モーション読み込み
	void			WriteMotion(int nowmotion);		//モーション書き出し
	void			rolling();						//回転のテスト
	void			MotionManager();				//状態に合わせてモーション再生する
	void			PlayFirstMotion();				//前と状態が違う場合のみ最初のモーションを設定する
	void			EditMode();						//前と状態が違う場合のみ最初のモーションを設定する
	void			Normalize(int number);			//角度の正規化
	void			PlayerStateDraw();				//プレイヤーの情報の編集と表示(IMGUI)
	void			DrawCollision();				//プレイヤーの情報の編集と表示(IMGUI)
	void			SetFrame();						//フレーム応じてモーションの位置を表示する
	void			SaveAxis();						//軸の判定を保存
	void			SaveCollision();				//当たり判定の保存
	void			SetHitBox();					//やられ判定の設定


private:
	CModel*			m_apModel[NUM_PLAYERPARTS];		//モデルのインスタンス
	MOTION_SET		m_apMotion[PM_MAX];			//モーションの数だけ生成モーションの数->キーの総数->モデルの数
	D3DXMATRIX		m_mtxWorld;						//ワールドマトリックス
	D3DXVECTOR3		m_pos;							//位置
	D3DXVECTOR3		m_rot;							//向き
	D3DXVECTOR3		m_move;							//移動量
	D3DXVECTOR3		m_posold;						//前回の位置
	//CShadow*		m_Shadow;						//影
	D3DXVECTOR3		m_rotDest;						//目的の角度の保存
	int				m_nNumKey;						//キーの総数
	D3DXMATRIX		m_mtxRot;						//回転マトリックス(保存用)
	D3DXQUATERNION	m_quat;							//クォータニオン
	D3DXVECTOR3		m_vecAxis;						//回転軸
	float			m_fRolling;						//回転量　(回転角)
	int				m_nNumModel;
	char			m_nModelpass[255];
	D3DXVECTOR3		m_movepos;
	D3DXVECTOR3		m_moverot;
	PLAYER_MOTION	m_pMotion;						//モーション
	PLAYER_MOTION	m_pMotionOld;					//ひとつ前のモーション
	int				m_nowmotion;					//現在のモーション
	int				m_nKEYData;						//現在データを表示するパーツ
	bool			m_bPlay;						//再生モード
	int				m_nEditModel;					//編集するモデル
	int				m_nHold;						//長押し時間
	int				m_nHoldold;						//長押し時間
	D3DXVECTOR3		m_TempPos;						//編集用の仮座標
	D3DXVECTOR3		m_TempRot;						//編集用の仮座標
	D3DXVECTOR3		m_ColiPos;						//編集用の仮座標
	D3DXVECTOR3		m_siz;
	int				m_nSelectCollison;				//選択している当たり判定の番号
	int				m_nSelectHurt;					//選択しているやられ判定の番号
	CCollision* 	m_AxisBox;						//押し出し判定(プレイヤーの軸)
	static int		m_snPlayernumber;				//プレイヤーの番号
	int				m_nPlayernumber;				//プレイヤーの番号
	KEY_SET			m_CopyKey;						//キーのコピー
	CCollision*		m_CopyCollision[MAX_COLLISION];	//やられ判定のコピー
	int				m_CopyColNumber;				//コピーするやられ判定の番号


	//使わねえ奴

	////テンプレート関数宣言
	//template <typename T, typename U>
	//T State(T tar,T Tri ,T Hol, U Key,U KeyDown)
	//{//数値の上下を設定する関数
	//	if (CApplication::GetInputKeyboard()->GetTrigger(Key))
	//	{
	//		tar += Tri;
	//	}
	//	else if (CApplication::GetInputKeyboard()->GetPress(Key))
	//	{
	//		m_nHold++;
	//		if (m_nHold >= HOLD_TIME)
	//		{
	//			tar += Hol;
	//		}
	//	}
	//	else if (CApplication::GetInputKeyboard()->GetTrigger(KeyDown))
	//	{
	//		tar -= Tri;
	//	}
	//	else if (CApplication::GetInputKeyboard()->GetPress(KeyDown))
	//	{
	//		m_nHold++;
	//		if (m_nHold >= HOLD_TIME)
	//		{
	//			tar -= Hol;
	//		}
	//	}
	//	return tar;
	//}
};

#endif
