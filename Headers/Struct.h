#pragma once
typedef struct tagTEXINFO
{
	LPDIRECT3DTEXTURE9 pTexture; 

	D3DXIMAGE_INFO tImageInfo; 
}TEXINFO;
typedef struct tagTile
{
	D3DXVECTOR3 vPos; 
	D3DXVECTOR3 vSize; 
	BYTE byDrawID; 
	BYTE byOption; 
}TILE;

typedef struct tagUnitInfo
{
	
#ifdef _AFX
	CString wstrName; 
#else
	wstring wstrName; 
#endif // _AFX
	int		iAtt; 
	int		iDef; 
	BYTE	byJob; 
	BYTE	byItem; 
}UNITINFO;

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vSize;
	//D3DXVECTOR2; 
	//D3DXVECTOR4; 

}INFO;

typedef struct tagOBJECTINFO
{
#ifdef _AFX
	CString cstrName;
	CString cstrObjectImage_ObjectKey;
	CString cstrObjectImage_Path;
	CString cstrDeathAnimImage_ObjectKey;
	CString cstrDeathAnimImage_StateKey;

	CString cstrIdleImage_ObjectKey;
	CString cstrIdleImage_StateKey;

	CString cstrCrashImage_ObjectKey;
	CString cstrCrashImage_StateKey;

#else
	wstring wstrName;
	wstring wstrObjectImage_ObjectKey;
	wstring wstrObjectImage_Path;

	wstring wstrDeathAnimImage_ObjectKey;
	wstring wstrDeathAnimImage_StateKey;

	wstring wstrIdleImage_ObjectKey;
	wstring wstrIdleImage_StateKey;

	wstring wstrCrashImage_ObjectKey;
	wstring wstrCrashImage_StateKey;
#endif
	float	fMaxHp;
	float	fAtk;
	float	fAtkRatio;
	float	fMoveSpeed;
	BYTE	eObjId;
//Bullet
	bool	bDestructable;
	BYTE	eBulletType;
//샷건일때
	float	fShotGunAngle;
	int		iShotGunCount;

}OBJECTINFO;



typedef struct tagAnimationInfo
{
#ifdef _AFX
	CString wstrObjectKey;
	CString wstrStateKey;
	CString	wstrFilePath;
#else
	wstring wstrObjectKey;
	wstring wstrStateKey;
	wstring wstrFilePath;
#endif
	float	fPlay_Speed;
	int		iMax_Index;
	bool	bLoop;
	bool	bIsSingle;

}ANIMATION;


typedef struct tagMatrixInfo
{
	D3DXVECTOR3 mat[MATID::END];
}MATRIXINFO;



typedef struct tagPlacementInfo
{	
	tagPlacementInfo() :m_bRender(false){}
	RENDERID::ID eRenderID;
	MATRIXINFO m_tMatInfo;	//매트릭스를 조정할 백터를 저장
	bool m_bRender;			//그려질지 말지를 결정
	
#ifdef _AFX
	CString wstrPrefabName;
	CString wstrName;
	CString wstrObjectKey;
	CString	wstrFilePath;
#else
	wstring wstrName;
	wstring wstrFilePath;

	wstring wstrObjectKey;
	wstring wstrObjName;
#endif
}PLACEMENT;

//하나의 objplaceinfo 는 여러개의 프리팹을 저장
// mat의 정보를 가져옴 ->대신 trans는 CenterPos 로 사용함
// translation 대신 draw에서 중점을 각 trans로 잡아줌

typedef struct tagMultiPlaceInfo
{
#ifdef _AFX
	CString wstrObjectKey;	//키값 
#else
	wstring wstrObjectKey;
	tagMultiPlaceInfo() {}
	tagMultiPlaceInfo(const tagMultiPlaceInfo& multi)
	{
		wstrObjectKey = multi.wstrObjectKey;
		for (auto& iter : multi.listPlace)
			listPlace.emplace_back(new PLACEMENT(*iter));
	}
#endif

	list<PLACEMENT*> listPlace; // placement의 프리팹키와 매트릭스 정보가 필요함
					
}MULTIPLACE;


typedef struct tagFrame
{
	float fStartFrame;
	float fMaxFrame;
	float fFrameSpeed;

	wstring wstrObjKey;
	wstring wstrStateKey;
}FRAME;

typedef struct tagColor
{
	int iAlpha;
	int iRed;
	int iGreen;
	int iBlue;
	
}MATCOLOR;


typedef struct tagCombat
{
	int iHp;
	int iAtk;
}COMBAT;