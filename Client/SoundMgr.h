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

		PLAYER_ROLL,	//��
		PLAYER_EVADE,	//����


		PLAYER_BULLET,//
		PLAYER_BEAM,//
		PLAYER_BEAM_CHARGE,//
		PLAYER_BOTTLE,//
		PLAYER_ROCKET,//
		PLAYER_GUNDRON,//
		PLAYER_MULTIHOMMING,//
		PLAYER_HIT, // //�־� �����Ҹ�

		DANGER,	//
		DANGER_CRIT,
		//�� �Ѿ��� ���� ����������
		ENEMY_NORMAL,//
		ENEMY_ROCKET,
		SERPENT_OUT_WARTER,//
		SERPENT_IN_WARTER,//

		SERPENT_LASER,	//�����ϴ� ������ �Ҹ� -> �Ҹ� �� ����
		SERPENT_BULLET, // //�Ҿ��Ҿ� �Ҹ�
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
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem; 

};

