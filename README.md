# TF2 Inspect Controller

Allows you to override what activity the game will use when inspecting a weapon.

### Include

```
/**
 * Called whenever the player inspects a weapon.
 *
 * @param weapon				Weapon being inspected index.
 * @param stage					Weapon inspect stage.
 * @param originalactivity		The activity the game wants to use.
 * @param newactivity			Your new activity to use.
 *
 * @return 						Return Plugin_Continue to use the game result. Return anything else to use the value set in result.
 */
forward Action IC_OnGetInspectActivity(int weapon, int stage, int originalactivity, int &newactivity);
```

```SourcePawn
#include <InspectControl>

public Action IC_OnGetInspectActivity(int weapon, int stage, int originalactivity, int &newactivity)
{
	PrintToChatAll("weapon %i stage %i originalactivity %i", weapon, stage, originalactivity);
    
	return Plugin_Continue;
}
```
or
```SourcePawn
#include <InspectControl>

public Action IC_OnGetInspectActivity(int weapon, int stage, int originalactivity, int &newactivity)
{
	newactivity = 1700;
    
	return Plugin_Changed;
}
```
