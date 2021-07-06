#pragma once
#include "Scene_Manager.h"
class CPrefab_Manager
{
	DECLARE_SINGLETON(CPrefab_Manager)
public:
	enum ID { ID_OBJECT, ID_ANIMATION, ID_PLACEMENT };
private:
	CPrefab_Manager();
	~CPrefab_Manager();
public:
	HRESULT Ready_Prefab_Manager();

	const OBJECTINFO* Get_ObjectPrefab(const wstring& _key) const;
	const ANIMATION* Get_AnimationPrefab(const wstring& _key) const;
	const PLACEMENT* Get_PlacementPrefab(const wstring& _key) const;
	 MULTIPLACE* Get_MultiPlacementPrefab(const wstring& _key) const;
	HRESULT SpawnObjectbyScene(const CScene_Manager::ID& _id);
	
private:
	HRESULT LoadObjectPrefab();
	HRESULT LoadAnimationPrefab();
	HRESULT LoadPlacementPrefab(const wstring& _path);
	HRESULT LoadMultiPlacementPrefab(const wstring& _path);
private:
	map<wstring, OBJECTINFO*> m_mapObjectPrefab;
	map<wstring, ANIMATION*> m_mapAnimationPrefab;
	map<wstring, PLACEMENT*> m_mapPlacementPrefab;
	map<wstring, MULTIPLACE*> m_mapMultPlaceiPrefab;

	const wstring m_wstrPlacementPath;
};

