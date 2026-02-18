#include <libdl/utils.h>
#include <libdl/moby.h>
#include <libdl/stdio.h>
#include "shared.h"

extern SurvivalBakedConfig_t bakedConfig;

char gambitsRandomWeapon[GAME_MAX_LOCALS] = {0};

//--------------------------------------------------------------------------
void gambitsUpYourArsenalOnRoundComplete(int roundNumber)
{
  char buf[64];
  int i;
  for (i = 0; i < GAME_MAX_LOCALS; ++i) {
    int weaponId = weaponSlotToId(randRangeInt(WEAPON_SLOT_VIPERS, WEAPON_SLOT_COUNT-1));
    gambitsRandomWeapon[i] = weaponId;

    Player* player = playerGetFromSlot(i);
    if (!playerIsValid(player)) continue;

    // give max ammo
    if (player->GadgetBox->Gadgets[weaponId].Level < 0)
      playerGiveWeapon(player->GadgetBox, weaponId, 0, 1);
    else
      player->GadgetBox->Gadgets[weaponId].Ammo = playerGetWeaponMaxAmmo(player->GadgetBox, weaponId);

    snprintf(buf, sizeof(buf), "New Weapon \x0E%s\x08", uiMsgString(weaponGetDef(weaponId, 0)->basicQSTag));
    pushSnack(i, buf, TPS * 3);
  }
}

//--------------------------------------------------------------------------
void gambitsUpYourArsenalTick(void)
{
  int i;

  for (i = 0; i < GAME_MAX_LOCALS; ++i) {
    if (gambitsRandomWeapon[i]) {
      mapLocalPlayerEnforceSingleWeaponRestriction(i, gambitsRandomWeapon[i], 0);
    }
  }
}

//--------------------------------------------------------------------------
void gambitsUpYourArsenalInit(void)
{
  // initialize random weapons
  gambitsUpYourArsenalOnRoundComplete(0);
}
