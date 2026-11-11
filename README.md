# ChemFactory 🧪
### A Chemistry-Based Resource Management & Simulation Game

ChemFactory is a menu-driven simulation game developed as the final project for the **Fundamentals of Programming** course at the School of Mechanical Engineering, Iran University of Science and Technology (IUST), in Summer 2025.

Set in a post-apocalyptic world in the year 2345, the game puts players in the role of a chemist struggling to rebuild their life by managing a laboratory, synthesizing chemical compounds, trading resources, and surviving unexpected events.

## 🎯 Game Objective

The player has 50 in-game days to:
- Earn at least **$700**.
- Discover all **10 essential chemical compounds**.
- Manage limited resources and avoid bankruptcy.

Both financial and scientific objectives must be achieved before the deadline to win the game.

## ⚙️ Core Gameplay Systems

### 1. Laboratory Management
- View owned chemical elements and compounds.
- Explore available synthesis recipes.
- Manufacture compounds using required elements.
- Earn money and experience points (XP).
- Discover new compounds and unlock advanced recipes.

### 2. Trading & Economy
- Sell synthesized compounds to generate income.
- Purchase chemical elements from the in-game shop.
- Manage financial resources and transaction risks.
- Deal with unexpected losses during trading.

### 3. Player Progression
- Experience-based leveling system.
- Unlock elements and advanced compounds.
- Level-dependent shop items and synthesis recipes.
- Progress tracking throughout the game.

### 4. Mini-Games
Interactive mini-games provide opportunities to earn additional XP, with financial risks for unsuccessful attempts.

### 5. Random Events
A system of 12 random events introduces uncertainty into gameplay.

Events can:
- Increase or decrease the player's money.
- Grant or remove experience points.
- Damage laboratory equipment.
- Cause laboratory accidents.
- Remove owned compounds.
- Introduce unexpected expenses and rewards.

### 6. Shop & Protection System
The in-game shop offers:
- Chemical elements required for synthesis.
- Consumable protective items.
- Permanent upgrades that reduce the impact of selected negative events.

## 🧩 Technical Design

The project uses structured data models to organize game entities and player state, including:

- `Element`: Chemical element properties and availability.
- `Compound`: Synthesis requirements, rewards, and discovery status.
- `Player`: Money, experience, level, inventory, and progression.
- `ShopItem`: Item prices, level requirements, and protective effects.

The game logic is organized into dedicated functions responsible for laboratory operations, compound synthesis, trading, shop transactions, mini-games, random events, and game-state validation.

## 🏁 Game End Conditions

**Victory:**
- At least $700 in available money.
- All 10 chemical compounds discovered.
- Completion within 50 in-game days.

**Defeat:**
- Bankruptcy.
- Exceeding the 50-day deadline without meeting the victory requirements.
---

## 👨‍💻 Author

**Sepehr Pouladi**

Mechanical Engineering Student  
Iran University of Science and Technology (IUST)

**Contact & Social Links**

- 📧 Email: sepehrpooladi84@gmail.com
- 💻 GitHub: [SepehrPouladi](https://github.com/SepehrPouladi)
- 🔗 LinkedIn: [Sepehr Pouladi](https://www.linkedin.com/in/sepehrpooladi84/)

---

*Developed as an academic project for the Fundamentals of Programming course – Summer 2025.*
