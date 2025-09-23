# Action Library Architecture Examples

This folder contains example architectures for implementing predefined actions in your dice combat system.

## Current System
Your current system uses a simple `DiceFace` structure:
```cpp
struct DiceFace {
    Action action;  // ATTACK, HEAL, BLOCK, EMPTY
    Target target;  // HERO, MONSTER, ALL_MONSTERS, etc.
    int value;      // damage/heal amount
};
```

## Architecture Options

### Option 1: Simple Named Actions (Easiest)
Expand your current enum with named actions:
```cpp
enum Action {
    ATTACK, HEAL, BLOCK, EMPTY,
    // Named actions
    SWORD_SLASH, FIREBALL, CLEAVE, HEAL_POTION
};
```

### Option 2: Action Library System (Recommended)
Create a library of predefined actions that characters can reference by name.
See `ActionLibrary.hpp` and `ActionLibrary.cpp` for full implementation.

### Option 3: JSON-Based Actions
Define actions in external JSON files for easy modding.
See `example_actions.json` for structure.

### Option 4: Template System
Use templates for common action patterns.
See `example_templates.hpp` for implementation.

## Benefits of Action Library Approach
- **Reusability**: Same actions can be used across multiple characters
- **Consistency**: All "Fireball" actions work the same way
- **Easy Balancing**: Change action values in one place
- **Expandability**: Easy to add new actions without touching character code
- **Readability**: Character builds are more descriptive

## Integration
To integrate any of these systems:
1. Choose your preferred approach
2. Copy the relevant example files to your `incl/` and `src/` directories
3. Update your `Makefile` to include new files
4. Modify `CharacterFactory.cpp` to use the new system

Your current code remains fully functional - these are architectural enhancements!