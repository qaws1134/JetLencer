#include "stdafx.h"
#include "Prefab_Manager.h"
#include "Spawn_Manager.h"

IMPLEMENT_SINGLETON(CPrefab_Manager)
CPrefab_Manager::CPrefab_Manager() 
	:m_wstrPlacementPath(L"../Data/Placement%d.dat")
{
}


CPrefab_Manager::~CPrefab_Manager()
{
	for (auto & rPair : m_mapAnimationPrefab)
	{
		Safe_Delete(rPair.second);
	}
	m_mapAnimationPrefab.clear();
	for (auto & rPair : m_mapObjectPrefab)
	{
		Safe_Delete(rPair.second);
	}
	m_mapObjectPrefab.clear();
	for (auto & rPair : m_mapPlacementPrefab)
	{
		Safe_Delete(rPair.second);
	}
	m_mapPlacementPrefab.clear();
}

HRESULT CPrefab_Manager::Ready_Prefab_Manager()
{

	if (FAILED(LoadAnimationPrefab()))
		return E_FAIL;
	if (FAILED(LoadObjectPrefab()))
		return E_FAIL;
	if (FAILED(LoadMultiPlacementPrefab(L"../Data/EffectPlace.dat")))
		return E_FAIL;
	return S_OK;
}

HRESULT CPrefab_Manager::LoadObjectPrefab()
{
	HANDLE hFile = CreateFile(L"../Data/Object.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return E_FAIL;
	}
		
	for (auto& rPair : m_mapObjectPrefab)
	{
		Safe_Delete(rPair.second);
	}
	m_mapObjectPrefab.clear();

	DWORD dwbyte = 0;
	DWORD strLen;
	OBJECTINFO* pObject;
	TCHAR *pBuff;
	while (true)
	{

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);

		if (dwbyte == 0)
			break;

		pObject = new OBJECTINFO;

		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->wstrName = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->wstrObjectImage_ObjectKey = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->wstrObjectImage_Path = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->wstrDeathAnimImage_ObjectKey = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->wstrDeathAnimImage_StateKey = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->wstrIdleImage_ObjectKey = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->wstrIdleImage_StateKey = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->wstrCrashImage_ObjectKey = pBuff;
		Safe_Delete(pBuff);

		ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
		pBuff = new TCHAR[strLen]{};
		ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
		pObject->wstrCrashImage_StateKey = pBuff;
		Safe_Delete(pBuff);


		ReadFile(hFile, &pObject->fMaxHp, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->fAtk, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->fAtkRatio, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->fMoveSpeed, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->eObjId, sizeof(BYTE), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->bDestructable, sizeof(bool), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->eBulletType, sizeof(BYTE), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->fShotGunAngle, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pObject->iShotGunCount, sizeof(int), &dwbyte, nullptr);

		m_mapObjectPrefab.emplace(pObject->wstrName, pObject);
	}
	CloseHandle(hFile);
	return S_OK;
}

HRESULT CPrefab_Manager::LoadAnimationPrefab()
{

	HANDLE hFile = CreateFile(L"../Data/Animation.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;
	for (auto& rPair : m_mapAnimationPrefab)
		Safe_Delete(rPair.second);
	m_mapAnimationPrefab.clear();

	DWORD dwbyte = 0;
	DWORD dwStringSize = 0;
	ANIMATION* pAnimaInfo = 0;
	TCHAR* pBuf = nullptr;

	while (true)
	{
		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		if (0 == dwbyte)
			break;
		pBuf = new TCHAR[dwStringSize];
		pAnimaInfo = new ANIMATION;
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		pAnimaInfo->wstrObjectKey = pBuf;
		Safe_Delete(pBuf);


		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		pBuf = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		pAnimaInfo->wstrStateKey = pBuf;
		Safe_Delete(pBuf);

		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		pBuf = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		pAnimaInfo->wstrFilePath = pBuf;
		Safe_Delete(pBuf);


		ReadFile(hFile, &pAnimaInfo->fPlay_Speed, sizeof(float), &dwbyte, nullptr);
		ReadFile(hFile, &pAnimaInfo->iMax_Index, sizeof(int), &dwbyte, nullptr);
		ReadFile(hFile, &pAnimaInfo->bLoop, sizeof(bool), &dwbyte, nullptr);
		ReadFile(hFile, &pAnimaInfo->bIsSingle, sizeof(bool), &dwbyte, nullptr);

		m_mapAnimationPrefab.emplace(pAnimaInfo->wstrObjectKey + pAnimaInfo->wstrStateKey, pAnimaInfo);

		//===============이미지 Insert==============
		if (pAnimaInfo->bIsSingle)
		{
			if (pAnimaInfo->wstrObjectKey != L"")
			{
				if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, pAnimaInfo->wstrFilePath, pAnimaInfo->wstrObjectKey)))
				{
					ERR_MSG(L"싱글 텍스쳐 실패");
					return E_FAIL;
				}
			}
		}
		else
		{
			if (pAnimaInfo->wstrObjectKey != L"" && pAnimaInfo->wstrStateKey != L"")
			{
				if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::MULTI_TEX, pAnimaInfo->wstrFilePath, pAnimaInfo->wstrObjectKey, pAnimaInfo->wstrStateKey, pAnimaInfo->iMax_Index)))
				{
					ERR_MSG(L"멀티 텍스쳐 실패");
					return E_FAIL;
				}
			}
		}
	}
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CPrefab_Manager::LoadPlacementPrefab(const wstring & _path)
{
	HANDLE hFile = CreateFile(_path.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Placement 파일이 없습니다");
		return E_FAIL;
	}

	//for (auto& rPair : m_mapPlacementPrefab)
	//	Safe_Delete(rPair.second);
	//m_mapPlacementPrefab.clear();

	DWORD dwbyte = 0;
	DWORD dwStringSize = 0;

	TCHAR* pBuf = nullptr;
	PLACEMENT* pPlacement = nullptr;

	while (true)
	{
		pPlacement = new PLACEMENT;
		ReadFile(hFile, &pPlacement->eRenderID, sizeof(int), &dwbyte, nullptr);
		if (0 == dwbyte)
		{
			Safe_Delete(pPlacement);
			break;
		}
		ReadFile(hFile, &pPlacement->m_tMatInfo.mat[MATID::TRANS], sizeof(D3DXVECTOR3), &dwbyte, nullptr);
		ReadFile(hFile, &pPlacement->m_tMatInfo.mat[MATID::ROT], sizeof(D3DXVECTOR3), &dwbyte, nullptr);
		ReadFile(hFile, &pPlacement->m_tMatInfo.mat[MATID::SCALE], sizeof(D3DXVECTOR3), &dwbyte, nullptr);

		ReadFile(hFile, &pPlacement->m_bRender, sizeof(bool), &dwbyte, nullptr);


		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		pBuf = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		pPlacement->wstrObjName = pBuf;

		Safe_Delete(pBuf);


		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		pBuf = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		pPlacement->wstrName = pBuf;
		Safe_Delete(pBuf);

		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		pBuf = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		pPlacement->wstrObjectKey = pBuf;
		Safe_Delete(pBuf);

		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		pBuf = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		pPlacement->wstrFilePath = pBuf;
		Safe_Delete(pBuf);

		m_mapPlacementPrefab.emplace(pPlacement->wstrName, pPlacement);
		//===============이미지 Insert==============

		if (pPlacement->wstrObjectKey != L"")
		{
			if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, pPlacement->wstrFilePath, pPlacement->wstrName)))
			{
				ERR_MSG(L"싱글 텍스쳐 실패");
				return E_FAIL;
			}
		}
	}
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CPrefab_Manager::LoadMultiPlacementPrefab(const wstring & _path)
{

	HANDLE hFile = CreateFile(_path.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Placement 파일이 없습니다");
		return E_FAIL;
	}

	for (auto& rPair : m_mapMultPlaceiPrefab)
		Safe_Delete(rPair.second);
	m_mapMultPlaceiPrefab.clear();


	DWORD dwbyte = 0;
	DWORD dwStringSize = 0;

	TCHAR* pBuf = nullptr;
	size_t iIdx = 0;
	MULTIPLACE* pMulti = nullptr;
	PLACEMENT* pPlacement = nullptr;
	list<PLACEMENT*> listPlacement;


	while (true)
	{
		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		if (0 == dwbyte)
			break;

		pMulti = new MULTIPLACE;

		pBuf = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		pMulti->wstrObjectKey = pBuf;
		Safe_Delete(pBuf);

		ReadFile(hFile, &iIdx, sizeof(size_t), &dwbyte, nullptr);
		if (listPlacement.size() > 0)
		{
			auto& iter = listPlacement.begin();
			for (; iter != listPlacement.end(); )
			{
				if (iter == listPlacement.end())
					listPlacement.erase(iter);
				else
					iter++;
			}
			listPlacement.clear();

		}
		for (size_t i = 0; i < iIdx; i++)
		{
			pPlacement = new PLACEMENT;
			ReadFile(hFile, &pPlacement->eRenderID, sizeof(int), &dwbyte, nullptr);
			if (0 == dwbyte)
			{
				Safe_Delete(pPlacement);
				break;
			}
			ReadFile(hFile, &pPlacement->m_tMatInfo.mat[MATID::TRANS], sizeof(D3DXVECTOR3), &dwbyte, nullptr);
			ReadFile(hFile, &pPlacement->m_tMatInfo.mat[MATID::ROT], sizeof(D3DXVECTOR3), &dwbyte, nullptr);
			ReadFile(hFile, &pPlacement->m_tMatInfo.mat[MATID::SCALE], sizeof(D3DXVECTOR3), &dwbyte, nullptr);

			ReadFile(hFile, &pPlacement->m_bRender, sizeof(bool), &dwbyte, nullptr);


			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			pBuf = new TCHAR[dwStringSize];
			ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
			pPlacement->wstrObjName = pBuf;

			Safe_Delete(pBuf);


			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			pBuf = new TCHAR[dwStringSize];
			ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
			pPlacement->wstrName = pBuf;
			Safe_Delete(pBuf);

			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			pBuf = new TCHAR[dwStringSize];
			ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
			pPlacement->wstrObjectKey = pBuf;
			Safe_Delete(pBuf);

			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			pBuf = new TCHAR[dwStringSize];
			ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
			pPlacement->wstrFilePath = pBuf;
			Safe_Delete(pBuf);

			listPlacement.emplace_back(pPlacement);

			if(m_mapPlacementPrefab.find(pPlacement->wstrName) == m_mapPlacementPrefab.end())
				m_mapPlacementPrefab.emplace(pPlacement->wstrName, pPlacement);

			if (pPlacement->wstrObjectKey != L"")
			{
				if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, pPlacement->wstrFilePath, pPlacement->wstrObjectKey)))
				{
					ERR_MSG(L"싱글 텍스쳐 실패");
					Safe_Delete(pPlacement);
					return E_FAIL;
				}
			}
		}
		pMulti->listPlace = listPlacement;
		auto& iter_find = m_mapMultPlaceiPrefab.find(pMulti->wstrObjectKey);
		if (iter_find == m_mapMultPlaceiPrefab.end())
			m_mapMultPlaceiPrefab.emplace(pMulti->wstrObjectKey, pMulti);
		else
			Safe_Delete(pMulti);

	}
	CloseHandle(hFile);
	
	return S_OK;
}

const OBJECTINFO * CPrefab_Manager::Get_ObjectPrefab(const wstring & _key) const
{
	auto& iter_find = m_mapObjectPrefab.find(_key);
	if(iter_find == m_mapObjectPrefab.end())
		return nullptr;
	return iter_find->second;
}

const ANIMATION * CPrefab_Manager::Get_AnimationPrefab(const wstring & _key) const
{
	auto& iter_find = m_mapAnimationPrefab.find(_key);
	if (iter_find == m_mapAnimationPrefab.end())
		return nullptr;
	return iter_find->second;
}

const PLACEMENT * CPrefab_Manager::Get_PlacementPrefab(const wstring & _key) const
{
	auto& iter_find = m_mapPlacementPrefab.find(_key);
	if (iter_find == m_mapPlacementPrefab.end())
		return nullptr;
	return iter_find->second;
}

 MULTIPLACE * CPrefab_Manager::Get_MultiPlacementPrefab(const wstring & _key) const
{
	auto& iter_find = m_mapMultPlaceiPrefab.find(_key);
	if (iter_find == m_mapMultPlaceiPrefab.end())
		return nullptr;
	return iter_find->second;
}



HRESULT CPrefab_Manager::SpawnObjectbyScene(const CScene_Manager::ID & _id)
{
	TCHAR szFilePath[MAX_PATH] = L"";
	swprintf_s(szFilePath, m_wstrPlacementPath.c_str(), (int)_id);
	LoadPlacementPrefab(szFilePath);

	//placement 출력 
	PLACEMENT* pPlacement = nullptr;
	for (auto& rPair : m_mapPlacementPrefab)
	{
		pPlacement = rPair.second;
		auto& iter_find = m_mapObjectPrefab.find(pPlacement->wstrObjName);
		if (iter_find == m_mapObjectPrefab.end())
		{
			//ERR_MSG(L"키값 오류");
			CSpawn_Manager::Spawn(L"", pPlacement, nullptr);
		}
		else
		{
			CSpawn_Manager::Spawn(iter_find->second->wstrName, pPlacement, iter_find->second);
		}
	}

	OBJECTINFO* pObject = nullptr;
	for (auto& rPair : m_mapObjectPrefab)
	{
		pObject = rPair.second;
		auto& iter_find = m_mapObjectPrefab.find(pObject->wstrName);
		if (iter_find == m_mapObjectPrefab.end())
		{
			//ERR_MSG(L"키값 오류");
			CSpawn_Manager::Spawn(L"", nullptr, nullptr);
		}
		else
		{
			CSpawn_Manager::Spawn(iter_find->second->wstrName, nullptr, pObject);
		}
	}


	return S_OK;
}
