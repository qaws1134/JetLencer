#pragma once
class CSpawn_Manager
{
	DECLARE_SINGLETON(CSpawn_Manager)
private:
	CSpawn_Manager();
	~CSpawn_Manager();
public:
	static HRESULT Spawn(const wstring _wstrObjName, const PLACEMENT* _pPlacement, const OBJECTINFO* _pObjectInfo);
	static void Spawn(const wstring _wstrObjName, _vec3 vPos, float fAngle, _vec3 vSpeed);
	static void Spawn(const wstring _wstrObjName, _vec3 vPos, bool _FrameStart, EFFECT::TYPE _eEffectType);
	static void Spawn(const wstring _wstrObjName, _vec3 vPos);
public :
	void Update_Spawn();


public :
	static list<MULTIPLACE*> m_listWaitPlacement;
	static list<_vec3>		m_listPos;


	float					m_fWaitSpeed;
};