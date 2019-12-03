Weapons Module

This is primarily designed for projectile/balistic weapons the system is versatile

Adding new weapons

Step 1)

step 2)

step 3) boom


notes:

when adding weapons to the module please note the additional lines needed to be added to the various files in the game

// Add to playerdata datablock
maxInv[Rifle] = value; 
maxInv[RifleClip] = value;


// Add to loadout function this will vary depending on Teams/Classes/GameMode
%player.setInventory(Ryder, 1);
%player.setInventory(RyderClip, %player.maxInventory(RyderClip));
%player.setInventory(RyderAmmo, %player.maxInventory(RyderAmmo));    // Start the gun loaded
%player.addToWeaponCycle(Ryder);

// Add to default.keybinds.cs
//GameCoreMap.bindCmd(keyboard, "2", "commandToServer('use',\"Rifle\");", "");












