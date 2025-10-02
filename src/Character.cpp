#include "Character.hpp"
#include "Display.hpp"

// ===== CONSTRUCTOR & DESTRUCTOR =====
Character::Character(const std::string& name, int health, const std::vector<DiceFace>& customFaces, const std::string& team)
: name(name), team(team), max_health(health), health(health), incoming_damage(0), shield(0), round_death(-1), die(customFaces) {
    // Validation checks
    if (customFaces.size() != 6) {
        std::cerr << "ERROR: Character '" << name << "' created with " << customFaces.size()
                  << " faces instead of 6!" << std::endl;
    }

    if (health <= 0) {
        std::cerr << "ERROR: Character '" << name << "' created with invalid health: " << health << std::endl;
    }

    // Validate dice faces
    for (size_t i = 0; i < customFaces.size(); ++i) {
        const auto& face = customFaces[i];
        if (face.value < 0) {
            std::cerr << "WARNING: Character '" << name << "' face " << i
                      << " has negative value: " << face.value << std::endl;
        }
        if (face.action == ATTACK && face.value == 0) {
            std::cerr << "WARNING: Character '" << name << "' has ATTACK with 0 damage on face " << i << std::endl;
        }
        if (face.action == HEAL && face.value == 0) {
            std::cerr << "WARNING: Character '" << name << "' has HEAL with 0 healing on face " << i << std::endl;
        }
        if (face.action == BLOCK && face.value == 0) {
            std::cerr << "WARNING: Character '" << name << "' has BLOCK with 0 shield on face " << i << std::endl;
        }
    }
}


// ===== CORE GAME METHODS =====
DiceFace Character::roll() {
    DiceFace result = die.rollFace();
    return result;
}

ActionResult Character::takeDamage(int damage) {
		if (dodged) {
			ActionResult result = {0, 0, 0, 0, 0, health, shield, health, shield, false, true};
			// Don't reset dodge here - it should persist through the turn
			return result;
		}

    int actualDamage = damage;
    ActionResult result = {0, 0, 0, 0, 0, health, shield, health, shield, false, false};

    if (shield > 0) {
        int blocked = std::min(shield, actualDamage);
        result.newShield -= blocked;
        shield -= blocked;
        actualDamage -= blocked;
        result.damageBlocked = blocked;
    }

    if (health == 0) {
        actualDamage = 0;
    }

    if (actualDamage > 0) {
        health -= actualDamage;
        if (health < 0)
            health = 0;
        result.damageDealt = actualDamage;
        result.newHealth = health;
    }
    return result;
}

ActionResult Character::heal(int amount) {
    int appliedHealing = amount;
    ActionResult result = {0, 0, 0, 0, 0, health, shield, health, shield, false, false};

    if (health == max_health) {
        appliedHealing = 0; // No healing needed
    } else if (health + amount > max_health) {
        appliedHealing = max_health - health; // Heal to full health
        incoming_damage = 0; // Clear incoming damage when healed to full
    }

    health += appliedHealing;
    result.healingAttempted = amount;
    result.healingApplied = appliedHealing;
    result.newHealth = health;
    incoming_damage -= appliedHealing;
    if (incoming_damage < 0)
			incoming_damage = 0;

    return result;
}

ActionResult Character::addShield(int amount) {
    ActionResult result = {0, 0, 0, 0, 0, health, shield, health, shield, false, false};

    shield += amount;
    result.shieldAdded = amount;
    result.newShield = shield;

    return result;
}

ActionResult Character::dodge() {
		ActionResult result = {0, 0, 0, 0, 0, health, shield, health, shield, false, false};
		dodged = true;
		return result;
}

ActionResult Character::stun(int actorHP) {
		if (dodged) {
			ActionResult result = {0, 0, 0, 0, 0, health, shield, health, shield, false, true};
			// Don't reset dodge here - it should persist through the turn
			return result;
		}

		// Stun connects when actor HP >= target HP
		if (actorHP >= health) {
			ActionResult result = {0, 0, 0, 0, 0, health, shield, health, shield, true, false};
			stunned = true;
			return result;
		} else {
			// Stun failed - actor HP too low
			ActionResult result = {0, 0, 0, 0, 0, health, shield, health, shield, false, false};
			return result;
		}
}

bool Character::isAlive() const {
    return health > 0;
}

// ===== GETTERS =====
const std::string& Character::getName() const {
    return name;
}

int Character::getHealth() const {
    return health;
}

int Character::getMaxHealth() const {
    return max_health;
}

int Character::getShield() const {
    return shield;
}

const std::string& Character::getTeam() const {
    return team;
}

int Character::getIncomingDamage() const {
    return incoming_damage;
}

int Character::getRoundOfDeath() const {
    return round_death;
}

bool Character::isStunned() const {
    return stunned;
}

bool Character::isDodging() const {
    return dodged;
}

// ===== SETTERS =====
int Character::setIncomingDamage(int damage) {
    incoming_damage = damage;
    return incoming_damage;
}

void Character::setRoundOfDeath(int round) {
    round_death = round;
}

void Character::resetShield() {
    shield = 0;
}

void Character::resetDodge() {
    dodged = false;
}

void Character::resetStun() {
    stunned = false;
}

void Character::setMaxHealth(int newMaxHealth) {
		max_health = newMaxHealth;
}

void Character::setHealth(int newHealth) {
		health = newHealth;
		if (health < 0)
			health = 0;
		if (health > max_health)
			health = max_health;
}

void Character::setShield(int newShield) {
		shield = newShield;
		if (shield < 0)
			shield = 0;
}

// ===== DISPLAY METHODS =====
void Character::displayDie() const {
    Display::displayFaces(die.getFaces());
}