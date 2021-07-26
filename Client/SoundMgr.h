#pragma once
class CSoundMgr
{
public:
	static CSoundMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr; 

		return m_pInstance; 
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance; 
			m_pInstance = nullptr; 
		}
	}
public:
	enum CHANNELID
	{
		BGM,
		BGM_MENU,
		BGM_LOADOUT,
		BGM_STAGE_SEL,
		BGM_STAGE1,
		BGM_STAGE2,
		BGM_STAGE_BOSS,
	

		PLAYER_MOVE,//
		PLAYER_AFTERBURN_BEGIN,//
		PLAYER_AFTERBURN,	//

		PLAYER_OVERHEAT,

		PLAYER_ROLL,	//쇽
		PLAYER_EVADE,	//슈육


		PLAYER_BULLET,//
		PLAYER_BEAM,//
		PLAYER_BEAM_CHARGE,//
		PLAYER_BOTTLE,//
		PLAYER_ROCKET,//
		PLAYER_GUNDRON,//
		PLAYER_MULTIHOMMING,//
		PLAYER_HIT, // //최앵 작은소리

		DANGER,	//
		DANGER_CRIT,
		//적 총알은 안쪽 범위에서만
		ENEMY_NORMAL,//
		ENEMY_ROCKET,
		SERPENT_OUT_WARTER,//
		SERPENT_IN_WARTER,//

		SERPENT_LASER,	//차지하다 공허한 소리 -> 소리 다 끄기
		SERPENT_BULLET, // //촤악촤악 소리
		SERPENT_DEAD,
		EFFECT_BOOM1,//
		EFFECT_BOOM2,
		EFFECT_PARTICLE_BOOM1,
		UI_MOVE,//
		UI_SELECT,//

		STAGE_COUNT,
		STAGE_VICTORY,

		MAXCHANNEL
	};
private:
	CSoundMgr();
	~CSoundMgr();

public:
	HRESULT Initialize();

	void Release(); 
public:
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();

private:
	void LoadSoundFile(); 

private:
	static CSoundMgr* m_pInstance; 
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem; 

};

