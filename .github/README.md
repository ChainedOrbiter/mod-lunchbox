# ![logo](https://raw.githubusercontent.com/azerothcore/azerothcore.github.io/master/images/logo-github.png) AzerothCore
## mod-lunchbox
### This is a module for [AzerothCore](http://www.azerothcore.org)

# Module info

- Name: Lunchbox
- Author: ChainedOrbiter
- Module:
    + Repository: [https://github.com/ChainedOrbiter/mod-lunchbox](https://github.com/ChainedOrbiter/mod-group-xp)
- License: MIT License

# Module integration

- Includes configuration (.conf)?: Yes, copied by CMake
- Includes SQL patches?: No
- Core hooks used:
    + UnitScript: OnAuraApply
    + PlayerScript: OnLogin
    + WorldScript: OnAfterConfigLoad

# Description
This module allows changing the amount of health or mana regained when eating and drinking, respectively.

#### Features:
- Set regen multiplier for food & drink separately
- Config option for debugging regen amount

### How to install
1. Simply place the module under the `modules` folder of your AzerothCore source folder.
2. Re-run cmake and build AzerothCore
3. Done

## Configuration
The module uses decimal value for multiplicative regen gain. Default values are below.

```ini
Lunchbox.Enable = 1
Lunchbox.DrinkMultiplier = 1.0
Lunchbox.FoodMultiplier = 1.0

Lunchbox.DebugChatEnabled = 0
```

### Reloading config
The module is engineered to respect the ``.reload config``, so any value changes made in the ``.conf`` file will be updated directly. This can be verified with
the debug chat enabled.


## Credits
* AzerothCore: [repository](https://github.com/azerothcore) - [website](http://azerothcore.org/) - [discord chat community](https://discord.gg/PaqQRkd)
