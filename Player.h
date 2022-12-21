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
	};

	struct MOTION_SET
	{
		KEY_SET	aModelKey[MAX_KEY];		//キーの総数分持つ
		int		nNumKey;				//キーの総数(ファイルで読み込む)
		bool	bLoop;					//ループするかどうか
	};

	enum PLAYER_STATE
	{
		P_STAND,
		P_CROUCH,
		P_MOVE,
		P_LATTACK_STAND,
		P_LATTACK_CROUCH,
		P_LATTACK_JUMP,
		P_MATTACK_STAND,
		P_MATTACK_CROUCH,
		P_MATTACK_JUMP,
		P_HATTACK_STAND,
		P_HATTACK_CROUCH,
		P_HATTACK_JUMP,
		P_MAX
	};

	explicit CPlayer(int nPriority = 3);
	~CPlayer();
	//プロトタイプ宣言
	HRESULT			Init()override;
	void			Uninit(void)override;
	void			Update(void)override;
	void			Draw(void)override;
	bool			ControlPlayer(void);			//プレイヤーの操作
	static CPlayer*	Create();
	D3DXMATRIX		GetMtx();
	void			ReadMotion();					//モーション読み込み
	void			WriteMotion(int nowmotion);		//モーション書き出し
	void			MotionPlayer(int nNumber);		//モーションの再生　引数は再生するモーションの番号
	void			rolling();
	void			MotionManager();				//状態に合わせてモーション再生する
	void			PlayFirstMotion();				//前と状態が違う場合のみ最初のモーションを設定する
	void			EditMode();						//前と状態が違う場合のみ最初のモーションを設定する
	void			Normalize(int number);			//角度の正規化
	void			PlayerStateDraw();				//プレイヤーの情報の編集と表示(IMGUI)
	void			DrawCollision();				//プレイヤーの情報の編集と表示(IMGUI)
	void			SetFrame();						//フレーム応じてモーションの位置を表示する



private:
	CModel*			m_apModel[NUM_PLAYERPARTS];		//モデルのインスタンス
	MOTION_SET		m_apMotion[MAX_MOTION];			//モーションの数だけ生成モーションの数->キーの総数->モデルの数
	D3DXMATRIX		m_mtxWorld;						//ワールドマトリックス
	D3DXVECTOR3		m_pos;							//位置
	D3DXVECTOR3		m_rot;							//向き
	D3DXVECTOR3		m_move;							//移動量
	D3DXVECTOR3		m_posold;						//前回の位置
	//CShadow*		m_Shadow;						//影
	D3DXVECTOR3		m_rotDest;						//目的の角度の保存
	int				m_MotionCnt;					//モーションカウンター
	int				m_nNumKey;						//キーの総数
	int				m_nCurrentKey;					//現在のキー番号
	D3DXMATRIX		m_mtxRot;						//回転マトリックス(保存用)
	D3DXQUATERNION	m_quat;							//クォータニオン
	D3DXVECTOR3		m_vecAxis;						//回転軸
	float			m_fRolling;						//回転量　(回転角)
	int				m_nNumModel;
	char			m_nModelpass[255];
	D3DXVECTOR3		m_movepos;
	D3DXVECTOR3		m_moverot;
	PLAYER_STATE	m_pState;
	PLAYER_STATE	m_pStateOld;					//ひとつ前のモーション
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
	int				m_nSelectCollison;
	CCollision* 	m_AxisBox;						//押し出し判定(プレイヤーの軸)



	//テンプレート関数宣言
	template <typename T, typename U>
	T State(T tar,T Tri ,T Hol, U Key,U KeyDown)
	{//数値の上下を設定する関数
		if (CApplication::GetInputKeyboard()->GetTrigger(Key))
		{
			tar += Tri;
		}
		else if (CApplication::GetInputKeyboard()->GetPress(Key))
		{
			m_nHold++;
			if (m_nHold >= HOLD_TIME)
			{
				tar += Hol;
			}
		}
		else if (CApplication::GetInputKeyboard()->GetTrigger(KeyDown))
		{
			tar -= Tri;
		}
		else if (CApplication::GetInputKeyboard()->GetPress(KeyDown))
		{
			m_nHold++;
			if (m_nHold >= HOLD_TIME)
			{
				tar -= Hol;
			}
		}

		return tar;
	}
};

#endif
