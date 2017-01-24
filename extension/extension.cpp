#include "extension.h"
#include <CDetour/detours.h>

CInspectControl g_InspectControl;
SMEXT_LINK(&g_InspectControl);

IGameConfig *g_pGameConf = NULL;

CDetour *g_GetInspectActivity = NULL;

IForward *g_pGetInspectActivity = NULL;

DETOUR_DECL_MEMBER1(GetInspectActivity, int, int, TFWeaponInspectStage)
{
	int index = gamehelpers->EntityToBCompatRef((CBaseEntity*)this);
	int iActualReturn = DETOUR_MEMBER_CALL(GetInspectActivity)(TFWeaponInspectStage);

//	g_pSM->LogMessage(myself, "GetInspectActivity Weapon: %d Stage: %d return: %d", index, TFWeaponInspectStage, iActualReturn);
	
	if (g_pGetInspectActivity != NULL && g_pGetInspectActivity->GetFunctionCount() > 0)
	{
		g_pGetInspectActivity->PushCell(index);
		g_pGetInspectActivity->PushCell(TFWeaponInspectStage);
		g_pGetInspectActivity->PushCell(iActualReturn);
		cell_t returnValue = 0;
		g_pGetInspectActivity->PushCellByRef(&returnValue);

		cell_t result = Pl_Continue;
		g_pGetInspectActivity->Execute(&result);
		
		if (result != Pl_Continue)
		{
			return (returnValue);
		}
	}
	
	return iActualReturn;
}

bool CInspectControl::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
	if (!gameconfs->LoadGameConfigFile("tf2.inspectcontrol", &g_pGameConf, error, maxlength)) return false;
	
	CDetourManager::Init(g_pSM->GetScriptingEngine(), g_pGameConf);
	
	g_GetInspectActivity = DETOUR_CREATE_MEMBER(GetInspectActivity, "CTFWeaponBase::GetInspectActivity");
	if(g_GetInspectActivity != NULL)
	{
		g_GetInspectActivity->EnableDetour();
		g_pSM->LogMessage(myself, "CTFWeaponBase::GetInspectActivity detour enabled.");
	}

	g_pGetInspectActivity = forwards->CreateForward("IC_OnGetInspectActivity", ET_Event, 4, NULL, Param_Cell, Param_Cell, Param_Cell, Param_CellByRef);
	
	return true;
}

void CInspectControl::SDK_OnUnload()
{
	gameconfs->CloseGameConfigFile(g_pGameConf);

	if(g_GetInspectActivity != NULL) g_GetInspectActivity ->Destroy();

	forwards->ReleaseForward(g_pGetInspectActivity);
}