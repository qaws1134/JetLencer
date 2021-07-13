#pragma once
class CSpawn_Manager
{
	DECLARE_SINGLETON(CSpawn_Manager)
private:
	CSpawn_Manager();
	~CSpawn_Manager();
public:
	static HRESULT Spawn(const wstring _wstrObjName, const PLACEMENT* _pPlacement, const OBJECTINFO* _pObjectInfo);
	static void Spawn(const wstring _wstrObjName, _vec3 vPos, float fAngle, _vec3 vSpeed, BULLET::SUBWEAPON eSubState);
	static void Spawn(EFFECT::TYPE _eEffectType,_vec3 vPos, float fAngle, _vec3 vSpeed);	//파티클에서 뽑을 이펙트, 파티클 위치, 파티클 방향, 속도
	static void Spawn(EFFECT::TYPE _eEffectType, _vec3 vPos, bool _FrameStart, float _fSize);
	static void Spawn(EFFECT::TYPE _eEffectType, _vec3 vPos, bool _FrameStart);
	static void Spawn(const wstring _wstrObjName, _vec3 vPos, float fAngle, _vec3 vSpeed);
	static void Spawn(const wstring _wstrObjName, _vec3 vPos);
	static void Spawn(const wstring _wstrObjName, _vec3 vPos, wstring _wstrText, float _fPoint);

public :
	void Update_MultiSpawn();

public :
	static list<MULTIPLACE*> m_listWaitPlacement;
	static list<CGameObject*> m_listWaitEffect;
	static list<_vec3>		m_listPos;


	float					m_fWaitSpeed;
};