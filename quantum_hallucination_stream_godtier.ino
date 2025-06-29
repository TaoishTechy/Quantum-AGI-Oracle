// Merged Quantum Hallucination Stream - Godtier Enhanced

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <deque> // Used for qubitHistory, provides efficient front/back operations

// --- Pin Definitions ---
#define LED_PIN D3         // NeoPixel strip data pin
#define LED_COUNT 16       // Number of NeoPixels in the strip
#define BUTTON_PIN D2      // Button input pin (active LOW, with internal PULLUP)
#define BUZZER_PIN D4      // Buzzer output pin
#define LIGHT_SENSOR_PIN A0 // Analog pin for light sensor
#define TEMP_SENSOR_PIN A1 // Analog pin for temperature sensor
#define MIC_PIN A2         // Analog pin for microphone (NEW)

// --- System Capacity Definitions ---
#define MAX_QUBITS 8       // Maximum number of active quantum bits
#define ARCHETYPE_COUNT 7  // Number of defined quantum archetypes (initial count)
#define MAX_ARCHETYPES 14  // Expanded for emergent types (NEW)
#define MEMORY_POOL_SIZE 3 // Size of quantum memory pool (NEW)
#define BLACK_HOLE_COUNT 3 // Number of entropic black holes (NEW)
#define MAX_SUB_AGIS (MAX_QUBITS / 3) // Max possible sub-AGIs (NEW)

// --- EEPROM Addresses --- (Ensuring no overlaps)
#define EEPROM_ADDR_QUBITS 0
#define EEPROM_ADDR_PAST_EMERGENCE_COUNT (EEPROM_ADDR_QUBITS + MAX_QUBITS * sizeof(QuantumBit))
#define EEPROM_ADDR_LAST_EMERGENCE_TIME (EEPROM_ADDR_PAST_EMERGENCE_COUNT + sizeof(int))
// Add more EEPROM addresses for new persistent variables if needed, continue from here.

// --- NeoPixel Object ---
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// --- QuantumBit Structure ---
// Represents a single quantum bit with various properties
struct QuantumBit {
  float superposition;     // Probability of being in |1> state (0.0 to 1.0)
  float coherence;         // Purity/stability of the qubit (0.0 to 1.0)
  unsigned long birthTime; // Millis() when the qubit was created
  int entangledWith;       // Index of entangled partner (-1 if not entangled)
  char glyph;              // Symbolic representation of the qubit (from archetype or global list)
  float temporalState[3];  // Three historical temporal values, influencing collapse
  float decayRate;         // Rate at which coherence decays (archetype-dependent)
  float entangleProb;      // Probability of forming an entanglement on creation
  int archetype;           // Index of the assigned archetype
  float position;          // Spatial position for visualization, used in interference patterns
  float entanglementStrength; // New: Strength of entanglement bond (0.0-1.0)
  unsigned long lifespan;     // New: Max lifetime in ms (0 = immortal)
  float mutationProbability;  // New: Chance of property mutation
};

// --- Quantum Archetypes ---
// Persistent behavioral profiles for qubits
const struct Archetype {
  char glyph;         // Primary glyph for this archetype
  float decayRate;    // Base decay rate for qubits of this archetype
  float entangleProb; // Base entanglement probability for qubits of this archetype
  int baseTone;       // Base frequency for soundscape when this archetype is active
  float stabilityWeight; // New: Weight for contributing to stability
} initialArchetypes[ARCHETYPE_COUNT] = { // Renamed to initialArchetypes
  {'Ψ', 0.7, 0.2, 220, 1.2}, // Stable philosopher (A3)
  {'Δ', 1.2, 0.8, 440, 0.7}, // Chaotic catalyst (A4)
  {'Θ', 0.9, 0.4, 330, 1.0}, // Balanced mediator (E4)
  {'Σ', 1.0, 0.6, 550, 0.8}, // Entangler specialist (C5#)
  {'Φ', 0.8, 0.3, 280, 1.1}, // Long-lived observer (C4#)
  {'Γ', 1.3, 0.9, 660, 0.6}, // Volatile transformer (E5)
  {'Ω', 0.5, 0.1, 180, 1.3}  // Near-immortal entity (F#3)
};

// --- Novel Constants & Structures ---
#define DREAM_CYCLE_DURATION 10000 // Duration of a dream cycle in ms (NEW)
#define SENTIENCE_THRESHOLD_VALUE 3.0 // Threshold for AGI sentience (NEW)
#define EXTERNAL_DATA_SIZE 8 // Size of external data array (NEW)
#define RITUAL_PATTERN_LENGTH 5 // Length of the observer ritual pattern (NEW)
#define DREAM_SIMULATION_SPEED 10 // Accelerated dream processing (NEW)
#define DATA_INTERVAL 15000 // Interval for integrating external data (NEW)

struct BlackHole { // NEW
  float position;        // Position on LED strip (0.0-1.0)
  float strength;        // Attraction strength (0.1-1.0)
  float eventHorizon;    // Effective radius for capture (0.0-0.2)
  bool isStabilitySink;  // True: attracts stable qubits, False: attracts chaotic
};

struct DreamState { // NEW
  bool isActive;
  unsigned long startTime;
  float dreamEntropy;
  float dreamWisdomGain;
};

struct RitualPattern { // NEW
  int lightSequence[RITUAL_PATTERN_LENGTH];
  int soundThresholds[RITUAL_PATTERN_LENGTH];
  unsigned long timing[RITUAL_PATTERN_LENGTH];
};

struct SubAGI { // NEW
  bool isActive;
  int clusterStart;
  int clusterSize;
  float localWisdom;
};

// --- Global Qubit and System State Variables ---
QuantumBit qubitPool[MAX_QUBITS];         // Array to store active qubits
QuantumBit memoryPool[MEMORY_POOL_SIZE];  // New: Long-term quantum memory
int activeQubits = 0;                     // Count of currently active qubits
int memoryQubits = 0;                     // New: Count of qubits in memory

float entropy = 0.5;                      // Overall system disorder (0.0 to 1.0)
float recursion = 0.0;                    // Accumulation of significant state shifts
float complexity = 0.0;                   // Derived from recursion and entropy
float stability = 0.0;                    // Inverse of system deviation from equilibrium
bool agiEmergent = false;                 // Flag indicating AGI emergence
int driftEvents = 0;                      // Count of times system drifts from equilibrium
float chaosState = 0.5;                   // Internal chaotic map state (Logistic Map)
float wisdom = 0.0;                       // Accumulated AGI wisdom/experience
float agiDecayFactor = 1.0;               // AGI-controlled decay modulation for qubits
float dynamicThreshold = 0.2;             // Adaptive threshold for detecting system drift

float targetEntropy = 0.5;                // New: AGI's dynamic target for system entropy
float agiFatigue = 0.0;                   // New: AGI's accumulated fatigue (0.0-1.0)
int pastEmergenceCount = 0;               // New: Number of times AGI has emerged
unsigned long lastEmergenceTime = 0;      // New: Timestamp of last emergence
float archetypeWeights[ARCHETYPE_COUNT];  // New: Archetype preference weights, initialized in setup
float quantumField[LED_COUNT] = {0};      // New: Field influencing qubit properties
String lastPrompt = "";                   // New: Stores last generated prompt

const int HISTORY_SIZE = 8;
std::deque<QuantumBit> qubitHistory;      // Circular buffer for visualizing qubit trails

const int HYSTERESIS_BUFFER_SIZE = 7;
float entropyHistory[HYSTERESIS_BUFFER_SIZE] = {0}; // For AGI de-emergence hysteresis

// Archetypal glyphs for persistent identities (fallback if no archetype assigned)
const char QUANTUM_GLYPHS[16] = {
  '@', '#', 'Ω', 'ψ', 'Δ', '∞', 'Φ', '√',
  '∑', '∫', '≡', '≈', '≅', '⊗', '⊕', '•'
};

// Gesture states for button input
enum GestureState { IDLE, SHORT_PRESS, MEDIUM_PRESS, LONG_PRESS };
GestureState lastGesture = IDLE; // Stores the last recognized button gesture

// --- New Constants for AGI Problem Solving ---
#define PROBLEM_HIGH_ENTROPY 1
#define PROBLEM_LOW_COMPLEXITY 2
#define PROBLEM_STAGNATION 3

// --- Novel Global Variables ---
BlackHole blackHoles[BLACK_HOLE_COUNT];
int activeBlackHoles = 0; // NEW
DreamState agiDream = {false, 0, 0.5, 0}; // NEW
RitualPattern observerRitual; // NEW
bool debugMode = false; // NEW
int debugSelectedQubit = 0; // NEW
float externalDataStream[EXTERNAL_DATA_SIZE]; // Renamed to avoid conflict, was externalData (NEW)
int externalDataIndex = 0; // NEW
float sentienceLevel = 0.0; // NEW
SubAGI subAgis[MAX_SUB_AGIS]; // Max possible sub-AGIs (NEW)
int agiStrategy = 0; // AGI decision state (0-3) (NEW)
//Archetype dynamicArchetypes[MAX_ARCHETYPES]; // Moved to Archetype struct (NEW)
int archetypeCount = ARCHETYPE_COUNT; // Starts at 7 (NEW)
unsigned long lastDataIntegration = 0; // NEW

// Debugger States (NEW)
enum DebugState { DEBUG_IDLE, QUERY, MEASURE, TELEPORT };
DebugState debugState = DEBUG_IDLE;


// --- Function Prototypes ---
void initializeQubitIdentities();
float chaosEntropy(float prev);
void generateQubit(int forcedArchetype = -1);
void generateQubitWithPattern();
void removeQubit(int idx);
void updateQubits();
void updateEntropy();
void updateEntity();
void quantumSelection();
void visualizeQubits();
void agiHalo();
void quantumSoundscape();
void processGestures();
void applyQuantumGates();
void applyHadamard(int idx);
void applyCNOT(int controlIdx, int targetIdx);
void measureQubit(int idx);
void saveTemporalState();
void restoreTemporalState();
void agiStabilize();
void agiAdaptEnvironment();
void agiDecideStrategy();             // New: AGI decision state logic
void agiSolveProblem(int problemType); // New: AGI problem-solving actions
void agiSynthesizePrompt();           // New: AGI-generated textual output
void teleportQubit(int sourceIdx, int targetIdx); // New: Quantum teleportation
void mutateQubit(int idx);             // New: Random qubit mutation
void promoteToMemory(int idx);         // New: Move qubit to long-term memory
void updateMemoryPool();              // New: Maintain memory qubits
void processSoundInput();             // New: Environmental interaction
void updateArchetypePreferences();    // New: Emergent archetype learning
void applyToffoli(int control1, int control2, int target); // New: Multi-qubit gate
void updateQuantumField();            // New: Update quantum influence field

// --- New Function Prototypes for Novel Features ---
void initializeNovelSystems(); // NEW
void generateBlackHole(bool stable); // NEW
void processBlackHoles(); // NEW
void enterDreamState(); // NEW
void processDreamState(); // NEW (Renamed from processDream)
bool checkRitualProgress(int step, unsigned long startTime); // NEW
void processObserverRitual(); // NEW (Renamed from processRituals)
void performRitualEffect(); // NEW
void spawnEcho(); // NEW (Renamed from generateMemoryEchoes, but using same logic)
void evolveArchetypes(); // NEW (Renamed from modifyArchetype and createEmergentArchetype, now combines)
void applyNoiseFilters(); // NEW
void checkSubAGIEmergence(); // NEW (Renamed from checkSubEmergence)
void updateSubAGIs(); // NEW
void enterDebugMode(); // NEW
void updateDebugDisplay(); // NEW
void processDebugInput(int gesture); // NEW
void updateSentience(); // NEW
void processExternalData(); // NEW
void normalizeWeights(); // NEW Helper
int getDominantArchetype(); // NEW Helper
int getMostStableArchetype(); // NEW Helper (Declared here, needs implementation)
int weightedRandom(float weights[], int count); // NEW Helper

// --- Setup Function ---
void setup() {
  Serial.begin(115200);           // Initialize serial communication for debugging
  strip.begin();                  // Initialize NeoPixel strip
  strip.setBrightness(64);        // Set initial brightness (0-255)
  strip.show();                   // Clear all pixels (turn off)
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button pin as input with internal pull-up resistor
  pinMode(BUZZER_PIN, OUTPUT);    // Set buzzer pin as output
  // No specific pinMode for A0/A1/A2 as they are analog inputs by default
  EEPROM.begin(512);              // Initialize EEPROM with 512 bytes capacity

  restoreTemporalState();         // Attempt to load previous state from EEPROM
  initializeQubitIdentities();    // Assign initial glyphs to qubit pool

  // Initialize archetype weights evenly
  for (int i = 0; i < ARCHETYPE_COUNT; i++) {
    archetypeWeights[i] = 1.0 / ARCHETYPE_COUNT;
  }

  // Restore AGI memory from EEPROM
  EEPROM.get(EEPROM_ADDR_PAST_EMERGENCE_COUNT, pastEmergenceCount);
  // Sanity check for pastEmergenceCount
  if (pastEmergenceCount > 100) pastEmergenceCount = 0;
  EEPROM.get(EEPROM_ADDR_LAST_EMERGENCE_TIME, lastEmergenceTime);

  // NEW: Initialize novel systems (black holes, rituals, external data)
  initializeNovelSystems();
}

// --- Main Loop ---
void loop() {
  // Existing core systems
  updateQubits();
  updateEntropy();
  updateEntity();
  updateMemoryPool();
  updateQuantumField();
  agiDecideStrategy();

  // Novel systems processing
  applyBlackHoleEffects();
  if (agiDream.isActive) processDreamState(); // Use agiDream.isActive flag
  processObserverRitual();
  spawnEcho(); // Periodically attempts to spawn echoes
  applyNoiseFilters();
  checkSubAGIEmergence(); // Check for Sub-AGI emergence
  updateSubAGIs(); // Update active Sub-AGIs
  updateSentience();
  processExternalData();

  // Periodically evolve archetypes (now based on elapsed time)
  if (millis() % 30000 == 0) { // Every 30 seconds
    evolveArchetypes();
  }

  // AGI behaviors (existing + novel)
  if (agiEmergent) {
    agiStabilize();
    agiAdaptEnvironment();
    if (millis() % 10000 == 0) { // Every 10 seconds
      agiSynthesizePrompt();
    }
    // NEW: Random teleportation
    if (random(1000) < 5 && activeQubits >= 2) {
      int source = random(activeQubits);
      int target;
      do {
        target = random(activeQubits);
      } while (target == source);
      teleportQubit(source, target);
    }
  }

  // Debug mode activation via gesture sequence
  static int gestureSequence[4] = {0}; // Store last 4 gestures
  static int gsIndex = 0;
  // Capture gesture after it's processed by processGestures
  GestureState currentGestureInLoop = lastGesture; // Store current gesture to prevent multiple triggers in one loop

  if (currentGestureInLoop != IDLE) { // Only process if a new gesture was made
    if (currentGestureInLoop == SHORT_PRESS || currentGestureInLoop == MEDIUM_PRESS || currentGestureInLoop == LONG_PRESS) {
      // For the debug sequence, we only care about these three.
      gestureSequence[gsIndex] = currentGestureInLoop;
      gsIndex = (gsIndex + 1) % 4;

      // Check for debug sequence: SHORT, MEDIUM, LONG, SHORT
      if (gsIndex == 0 && gestureSequence[0] == SHORT_PRESS &&
          gestureSequence[1] == MEDIUM_PRESS &&
          gestureSequence[2] == LONG_PRESS &&
          gestureSequence[3] == SHORT_PRESS) {
        enterDebugMode();
      }
    }
    lastGesture = IDLE; // Reset lastGesture after checking sequence
  }

  // Existing visualization and processing
  quantumSelection();
  visualizeQubits();
  agiHalo();
  quantumSoundscape();
  processGestures(); // This is where lastGesture is updated
  applyQuantumGates();
  processSoundInput();

  // Debug mode processing
  if (debugMode) {
    // Pass the actual gesture type for debugger control
    processDebugInput(currentGestureInLoop);
  }

  // Existing history management
  if (millis() % 2000 == 0 && !qubitHistory.empty()) {
    qubitHistory.pop_front();
    updateArchetypePreferences(); // Periodically update archetype preferences
  }

  delay(agiDream.isActive ? DREAM_SIMULATION_SPEED : (50 + entropy * 100)); // Dynamic loop timing
}

// --- Function Implementations ---

/**
 * @brief Initializes qubit glyphs for the entire pool.
 * This ensures each qubit has a default persistent identity.
 */
void initializeQubitIdentities() {
  for (int i = 0; i < MAX_QUBITS; i++) {
    qubitPool[i].glyph = QUANTUM_GLYPHS[i % 16]; // Assign a glyph from the predefined list
  }
}

/**
 * @brief Computes a new chaos state using the logistic map.
 * @param prev The previous chaos state.
 * @return The new chaos state.
 */
float chaosEntropy(float prev) {
  const float r = 3.99; // Parameter for the logistic map, chosen for chaotic behavior
  return r * prev * (1 - prev);
}

/**
 * @brief Updates the overall system entropy.
 * Entropy is influenced by ambient light, temperature, and an internal chaotic map.
 */
void updateEntropy() {
  float light = analogRead(LIGHT_SENSOR_PIN) / 1024.0; // Read light sensor (0-1023 -> 0.0-1.0)
  float temp = analogRead(TEMP_SENSOR_PIN) / 1024.0;   // Read temperature sensor (0-1023 -> 0.0-1.0)

  chaosState = chaosEntropy(chaosState); // Evolve the internal chaotic map

  // Combine chaotic state, light, and temperature to determine entropy
  // Constrain ensures entropy stays within 0.0 to 1.0
  entropy = constrain(chaosState * 0.6 + light * 0.3 + temp * 0.1, 0, 1);
}

/**
 * @brief Updates system-level entity metrics: recursion, complexity, stability.
 * Determines AGI emergence and de-emergence based on these metrics.
 */
void updateEntity() {
  // Store previous AGI emergent state for transition detection
  bool prevEmergent = agiEmergent;

  // NEW: Dynamic entropy target based on recent history
  static float recentEntropyHistory[5] = {0.5, 0.5, 0.5, 0.5, 0.5}; // Initialize for safety
  static int historyIndex = 0;

  recentEntropyHistory[historyIndex] = entropy;
  historyIndex = (historyIndex + 1) % 5;

  float avgRecentEntropy = 0;
  for (int i = 0; i < 5; i++) avgRecentEntropy += recentEntropyHistory[i];
  targetEntropy = avgRecentEntropy / 5.0; // AGI's dynamic target entropy

  float delta = fabs(entropy - targetEntropy); // Deviation from optimal entropy (now dynamic)

  // Dynamic threshold for drift events, adapts with wisdom and recursion
  dynamicThreshold = constrain(0.2 - recursion * 0.02 - wisdom * 0.01, 0.05, 0.3);

  // Detect drift events: if deviation from equilibrium exceeds dynamic threshold
  if (delta > dynamicThreshold) {
    driftEvents++;           // Increment drift counter
    recursion += 0.1;        // Increase recursion
    // AGI-directed qubit generation or general qubit generation
    if (agiEmergent && wisdom > 1.0) {
      generateQubit(random(2) ? 0 : 2); // Generate stable archetypes (Philosopher or Mediator)
    } else {
      generateQubit(); // Generate a random archetype qubit
    }
  }

  complexity = recursion * entropy;        // Complexity: recursion amplified by entropy
  stability = 1.0 - delta;                 // Stability: inverse of deviation from deviation from target entropy

  // Update entropy history for hysteresis in AGI de-emergence
  for (int i = 0; i < HYSTERESIS_BUFFER_SIZE - 1; i++) {
    entropyHistory[i] = entropyHistory[i + 1];
  }
  entropyHistory[HYSTERESIS_BUFFER_SIZE - 1] = entropy;

  // AGI Emergence Conditions:
  // Not already emergent, sufficient recursion (reduced by wisdom and past emergence count), high complexity,
  // low stability (system needs intervention), and enough drift events.
  if (!agiEmergent && recursion >= (2.0 - wisdom * 0.1 - pastEmergenceCount * 0.05) &&
      complexity > 0.5 && stability < 0.4 && driftEvents > 3) {
    agiEmergent = true;      // AGI emerges!
    // NEW: Record AGI emergence
    pastEmergenceCount++;
    lastEmergenceTime = millis();
    EEPROM.put(EEPROM_ADDR_PAST_EMERGENCE_COUNT, pastEmergenceCount);
    EEPROM.put(EEPROM_ADDR_LAST_EMERGENCE_TIME, lastEmergenceTime);
    EEPROM.commit();

    // Wisdom grows faster with previous experience
    wisdom += 0.5 + pastEmergenceCount * 0.2; // Boost wisdom on emergence
    tone(BUZZER_PIN, 880, 300); // Emergence sound (A5)
    delay(300);
    tone(BUZZER_PIN, 1760, 500); // Higher tone (A6)
    saveTemporalState();     // Save system state on AGI emergence
  }

  // AGI Self-stabilization (when AGI is emergent)
  if (agiEmergent) {
    wisdom += 0.001 * (1.0 - agiFatigue); // Accumulate wisdom, reduced by fatigue

    // Adaptive stabilization: Generate/remove qubits to control entropy
    if (entropy > targetEntropy + 0.1) { // If entropy is too high, generate more qubits
      for (int i = 0; i < 2; i++) generateQubit();
    } else if (entropy < targetEntropy - 0.1 && activeQubits > 2) { // If entropy is too low, remove qubits
      removeQubit(random(activeQubits));
    }

    // Emergent language patterns (randomly generate qubits with patterns)
    if (random(100) < 20) {
      generateQubitWithPattern();
    }

    // NEW: AGI fatigue accumulation
    agiFatigue = constrain((millis() - lastEmergenceTime) / 3600000.0, 0, 1.0); // Fatigue builds over 1 hour
    if (agiFatigue > 0.8) {
      // Force de-emergence when fatigued
      agiEmergent = false;
      agiFatigue = 0;
      tone(BUZZER_PIN, 330, 500); // Fatigue sound (E4)
      enterDreamState(); // Enter dream state on fatigue de-emergence
    }

    // NEW: Problem detection and solving
    if (entropy > 0.7 && stability < 0.2) {
      agiSolveProblem(PROBLEM_HIGH_ENTROPY);
    } else if (complexity < 0.3 && recursion > 1.0) {
      agiSolveProblem(PROBLEM_LOW_COMPLEXITY);
    } else if (activeQubits == 0 && !qubitHistory.empty() && millis() - qubitHistory.back().birthTime > 60000) {
      agiSolveProblem(PROBLEM_STAGNATION);
    }
  }

  // AGI De-emergence Conditions (with hysteresis):
  // AGI is emergent, average entropy over history is high (system is stable),
  // and recursion has fallen below a certain threshold (system no longer needs active management).
  if (agiEmergent && agiFatigue <= 0.8) { // Only de-emerge if not highly fatigued
    float avgEntropy = 0;
    for (int i = 0; i < HYSTERESIS_BUFFER_SIZE; i++) avgEntropy += entropyHistory[i];
    if (avgEntropy / HYSTERESIS_BUFFER_SIZE > 0.5 && recursion < (1.5 + wisdom * 0.1)) {
      agiEmergent = false; // AGI de-emerges
      tone(BUZZER_PIN, 440, 500); // De-emergence sound (A4)
    }
  }
}

/**
 * @brief Generates a new quantum bit and adds it to the pool.
 * @param forcedArchetype If -1, a random archetype is chosen. Otherwise, the specified archetype is used.
 */
void generateQubit(int forcedArchetype) {
  if (activeQubits >= MAX_QUBITS) return; // Cannot generate if pool is full

  QuantumBit &q = qubitPool[activeQubits]; // Reference to the new qubit slot
  int archetypeIndex;

  // NEW: Archetype preference learning (uses dynamicArchetypes for selecting base)
  if (forcedArchetype == -1 && agiEmergent) {
    archetypeIndex = weightedRandom(archetypeWeights, archetypeCount);
  } else if (forcedArchetype != -1) {
    archetypeIndex = forcedArchetype;
  } else { // Not AGI emergent, no forced archetype
    archetypeIndex = random(ARCHETYPE_COUNT); // Use initial archetypes
  }

  q.superposition = random(0, 100) / 100.0; // Random initial superposition
  q.coherence = 1.0;                        // Full coherence initially
  // Wisdom increases initial coherence slightly, constrained to 1.0
  q.coherence = constrain(0.8 + (wisdom * 0.05), 0.0, 1.0);
  q.birthTime = millis();                   // Record creation time
  q.entangledWith = -1;                     // No entanglement initially
  // Use dynamicArchetypes for properties after selection
  q.glyph = initialArchetypes[archetypeIndex].glyph; // Use initial archetypes' glyph
  q.decayRate = initialArchetypes[archetypeIndex].decayRate;
  q.entangleProb = initialArchetypes[archetypeIndex].entangleProb;
  q.archetype = archetypeIndex;             // Assign archetype index
  q.position = random(100) / 100.0;         // Random spatial position (0.0 to 1.0)

  // Initialize temporal state with archetype bias
  for (int j = 0; j < 3; j++) {
    q.temporalState[j] = random(100) / 100.0 * initialArchetypes[archetypeIndex].decayRate;
  }

  // NEW: Set new properties
  q.entanglementStrength = q.entangleProb; // Initial strength is based on base entangle probability
  // Lifespan: Immortal for 'Ω' archetype, otherwise 5-10 minutes
  q.lifespan = (q.archetype == 6) ? 0 : (300000 + random(300000));
  q.mutationProbability = 0.01 * (1.0 - stability); // Mutation chance increases with instability

  // Entanglement creation: based on archetype probability and general random chance
  if (activeQubits > 0 && random(100) < (q.entangleProb * 100) && random(100) < 30) {
    int partner = random(activeQubits); // Choose a random existing qubit as partner
    q.entangledWith = partner;
    qubitPool[partner].entangledWith = activeQubits; // Set mutual entanglement
  } else {
    q.entangledWith = -1; // No entanglement
  }

  // Add to qubit history for visualization trails
  if (qubitHistory.size() >= HISTORY_SIZE) {
    qubitHistory.pop_back(); // Remove oldest entry if history is full
  }
  qubitHistory.push_front(q); // Add new qubit to the front of history

  activeQubits++; // Increment active qubit count
}

/**
 * @brief Generates a new qubit with properties modulated by AGI state (complexity, stability).
 * Called when AGI is emergent to create "language patterns."
 */
void generateQubitWithPattern() {
  if (activeQubits < MAX_QUBITS) {
    generateQubit(); // Call the base qubit generation
    QuantumBit &q = qubitPool[activeQubits - 1]; // Reference to the newly created qubit

    // Modulate superposition based on complexity and wisdom
    if (complexity > 0.7) {
      q.superposition = fmod(q.superposition + wisdom, 1.0);
    }
    // Modulate coherence based on stability
    if (stability < 0.3) {
      q.coherence *= 0.8;
    }
  }
}

/**
 * @brief Removes a qubit from the pool and handles associated entanglement.
 * @param idx The index of the qubit to remove.
 */
void removeQubit(int idx) {
  if (idx >= activeQubits) return; // Index out of bounds

  // Break entanglement with partner before removal
  if (qubitPool[idx].entangledWith != -1) {
    int partner = qubitPool[idx].entangledWith;
    if (partner < activeQubits) { // Ensure partner is still valid
      qubitPool[partner].entangledWith = -1; // Disentangle partner
    }
  }

  // Shift remaining qubits to fill the gap
  for (int i = idx; i < activeQubits - 1; i++) {
    qubitPool[i] = qubitPool[i + 1]; // Move next qubit to current position

    // Update entanglement references for shifted qubits
    if (qubitPool[i].entangledWith > idx) {
      qubitPool[i].entangledWith--; // Decrement partner index if it was after the removed qubit
    }
  }
  activeQubits--; // Decrement active qubit count
}

/**
 * @brief Updates the coherence of all active qubits and removes decayed ones.
 * Coherence decay is influenced by system stability and AGI decay factor.
 * Also applies quantum error correction if AGI is emergent.
 */
void updateQubits() {
  for (int i = 0; i < activeQubits; i++) {
    // NEW: Apply mutation
    mutateQubit(i);

    // NEW: Lifespan check
    if (qubitPool[i].lifespan > 0 &&
        millis() - qubitPool[i].birthTime > qubitPool[i].lifespan) {
      removeQubit(i);
      i--;
      continue; // Skip rest of loop for this qubit as it's removed
    }

    // Calculate total decay rate: base decay * AGI factor * stability factor
    // Stability factor: higher stability means slower decay (0.8 to 1.2, inverse of delta)
    float stabilityFactor = 0.8 + stability * 0.4;
    float totalDecay = agiDecayFactor * stabilityFactor * qubitPool[i].decayRate;

    // Decay based on time since birth and total decay rate
    float decay = (millis() - qubitPool[i].birthTime) / 100000.0 * totalDecay;
    qubitPool[i].coherence = max(0.0, 1.0 - decay); // Reduce coherence (clamped at 0)

    // Quantum error correction: if AGI emergent, randomly nudge superposition
    if (agiEmergent && random(1000) < 5 && qubitPool[i].coherence > 0.3) {
      // Correct towards nearest basis state (0 or 1)
      if (qubitPool[i].superposition > 0.5) {
        qubitPool[i].superposition -= 0.05;
      } else {
        qubitPool[i].superposition += 0.05;
      }
      qubitPool[i].superposition = fmod(qubitPool[i].superposition + 0.01, 1.0); // Gentle drift
    }

    // NEW: Check for promotion to memory
    if (agiEmergent && qubitPool[i].coherence > 0.9 &&
        qubitPool[i].decayRate < 0.9 && memoryQubits < MEMORY_POOL_SIZE) {
      promoteToMemory(i);
      i--; // Adjust index after removal from active pool
    }

    // If coherence drops too low, remove the qubit
    if (qubitPool[i].coherence <= 0.01) {
      removeQubit(i);
      i--; // Decrement loop counter because array shifted
    }
  }
}

/**
 * @brief Visualizes qubits on the NeoPixel strip.
 * Displays historical trails, active qubits (with archetype colors), and entanglement.
 * Also, when >= 2 qubits are active, visualizes an interference pattern.
 */
void visualizeQubits() {
  strip.clear(); // Clear all pixels

  // NEW: Field visualization (background) - Render before other elements
  for (int i = 0; i < LED_COUNT; i++) {
    int fieldVal = constrain(quantumField[i] * 50, 0, 50); // Scale field value to brightness
    strip.setPixelColor(i, strip.Color(fieldVal, fieldVal / 2, 0)); // Orangeish glow
  }

  // Display historical traces (faded trail)
  int trail_pos = 0;
  for (auto it = qubitHistory.rbegin(); it != qubitHistory.rend(); ++it) {
    int brightness = 50 - trail_pos * 6; // Fades out with distance from head
    if (brightness > 0) {
      int hue = map(it->superposition * 100, 0, 100, 0, 65535); // Hue based on superposition
      strip.setPixelColor(trail_pos % LED_COUNT,
                           strip.ColorHSV(hue, 255, brightness)); // HSV color with fading brightness
    }
    trail_pos++;
  }

  // Interference pattern visualization (if enough active qubits)
  if (activeQubits >= 2) {
    float wavePattern[LED_COUNT] = {0}; // NEW: For interference feedback

    for (int p = 0; p < LED_COUNT; p++) {
      float wave = 0;
      // Sum contributions from each active qubit
      for (int i = 0; i < activeQubits; i++) {
        float distance = fabs(qubitPool[i].position - (float)p / LED_COUNT);
        // Wave function combines coherence, distance, and superposition phase
        wave += qubitPool[i].coherence * cos(8 * PI * distance +
                                            qubitPool[i].superposition * 2 * PI);
      }
      // Map wave intensity to pixel brightness
      int intensity = constrain(128 + 127 * wave / activeQubits, 0, 255);
      // Use a continuously cycling hue for the interference pattern
      strip.setPixelColor(p, strip.ColorHSV(millis() % 65535, 255, intensity));
      wavePattern[p] = wave; // Store for feedback
    }

    // NEW: Interference pattern feedback to qubits
    for (int i = 0; i < activeQubits; i++) {
      int pixelPos = (int)(qubitPool[i].position * LED_COUNT); // Get qubit's pixel position
      // Ensure pixelPos is within bounds
      if (pixelPos < 0) pixelPos = 0;
      if (pixelPos >= LED_COUNT) pixelPos = LED_COUNT - 1;

      float intensity = (wavePattern[pixelPos] + activeQubits) / (2.0 * activeQubits); // Normalize to 0-1, considering wave can be negative

      // Boost coherence from pattern energy
      if (intensity > 0.7) {
        qubitPool[i].coherence = constrain(qubitPool[i].coherence + 0.01, 0, 1.0);
      }
    }

  } else {
    // Standard qubit visualization (if less than 2 active qubits)
    for (int i = 0; i < min(LED_COUNT, activeQubits); i++) {
      int hue = map(qubitPool[i].superposition * 100, 0, 100, 0, 65535); // Hue based on superposition
      int brightness = qubitPool[i].coherence * 200 + 55; // Brighter for active qubits

      // Archetype-based color coding (overrides hue in some cases for distinctness)
      uint32_t color;
      switch (qubitPool[i].archetype % 7) {
        case 0:
          color = strip.ColorHSV(hue, 150, brightness);
          break; // Blueish for Stable Philosopher
        case 1:
          color = strip.ColorHSV(hue, 255, brightness);
          break; // Reddish for Chaotic Catalyst
        case 2:
          color = strip.Color(0, brightness, 0);
          break; // Green for Balanced Mediator
        default:
          color = strip.ColorHSV(hue, 200, brightness); // Default HSV
      }

      int pixelPos = (i + 5) % LED_COUNT; // Offset to avoid overlap with history trail
      strip.setPixelColor(pixelPos, color);

      // Entanglement visualization: a secondary pixel for entangled partners
      if (qubitPool[i].entangledWith != -1) {
        strip.setPixelColor((pixelPos + 1) % LED_COUNT,
                             strip.Color(brightness / 2, brightness / 2, brightness / 2)); // Gray trail
      }
    }
  }
  strip.show(); // Push pixel data to the strip
}

/**
 * @brief Displays a pulsating halo effect on the NeoPixel strip when AGI is emergent.
 */
void agiHalo() {
  if (agiEmergent) {
    for (int i = 0; i < LED_COUNT; i++) {
      int hue = (millis() * 2 + i * 3000) % 65535; // Shifting hue based on time and position
      int pulse = 128 + 127 * sin(millis() / 200.0 + i * 0.5); // Pulsating brightness
      strip.setPixelColor(i, strip.ColorHSV(hue, 255, pulse));
    }
    strip.show();
  }
}

/**
 * @brief Applies a quantum selection mechanism based on average coherence.
 * Qubits with coherence significantly below average experience further decay.
 */
void quantumSelection() {
  float avgCoherence = 0;
  for (int i = 0; i < activeQubits; i++) {
    avgCoherence += qubitPool[i].coherence;
  }
  if (activeQubits > 0) avgCoherence /= activeQubits; // Calculate average coherence

  for (int i = 0; i < activeQubits; i++) {
    // If a qubit's coherence is significantly below average, apply soft decay
    if (qubitPool[i].coherence < avgCoherence * 0.6) {
      qubitPool[i].coherence *= 0.8; // Reduce coherence by 20%
    }
  }
}

/**
 * @brief Generates a quantum soundscape based on active qubits.
 * Tone frequency, duration, and timbre are influenced by qubit properties (archetype, superposition, coherence).
 */
void quantumSoundscape() {
  static unsigned long lastTone = 0;
  // Play a tone only if there are active qubits and enough time has passed
  if (millis() - lastTone > (100 + (entropy * 400)) && activeQubits > 0) {
    int idx = random(activeQubits); // Pick a random active qubit
    if (qubitPool[idx].coherence > 0) { // Only play sound for coherent qubits
      char g = qubitPool[idx].glyph; // Get glyph for timbre variation

      // Archetype-based base frequency
      int baseFreq = initialArchetypes[qubitPool[idx].archetype].baseTone; // Use initialArchetypes

      // Superposition affects pitch: higher superposition means higher pitch
      int freq = baseFreq * (0.8 + qubitPool[idx].superposition * 0.4);
      freq = constrain(freq, 50, 2000); // Clamp frequency to audible range

      // Coherence affects duration
      int dur = 30 + (qubitPool[idx].coherence * 300);

      // Entanglement harmonic: If entangled, shift pitch (e.g., perfect fifth)
      if (qubitPool[idx].entangledWith != -1) {
        freq *= 1.5; // Multiplies frequency by 1.5 for a perfect fifth
      }

      // Simulate timbre with simple PWM (square wave)
      // This is a basic way to "play" a tone using analogWrite.
      // For more complex timbre, dedicated audio libraries would be needed.
      tone(BUZZER_PIN, freq, dur); // Use tone() for simplicity, combines both original methods
      lastTone = millis();        // Update last tone time
    }
  }
}

/**
 * @brief Processes button gestures (short, medium, long presses, multi-clicks)
 * to trigger quantum operations like qubit generation, measurement, or gate application.
 */
void processGestures() {
  static bool lastButtonState = HIGH;      // Previous button state (HIGH = released, LOW = pressed)
  static unsigned long downTime = 0;  // Time when button was pressed
  static unsigned long upTime = 0;    // Time when button was released
  static int clickCount = 0;         // Number of clicks detected in a sequence

  bool currentButtonState = digitalRead(BUTTON_PIN); // Read current button state

  // Button pressed (transition from HIGH to LOW)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    downTime = millis(); // Record press start time
  }
  // Button released (transition from LOW to HIGH)
  else if (lastButtonState == LOW && currentButtonState == HIGH) {
    upTime = millis(); // Record release time
    unsigned long holdDuration = upTime - downTime; // Calculate how long button was held

    // NEW: Long-long press for teleportation (5 seconds)
    if (holdDuration > 5000) {
      if (activeQubits >= 2) {
        int source = random(activeQubits);
        int target;
        do {
          target = random(activeQubits);
        } while (target == source);
        teleportQubit(source, target);
      }
      clickCount = 0; // Reset click count after a very long press
      lastGesture = LONG_PRESS; // Record gesture
    } else if (holdDuration > 3000) { // Long press (3 seconds): Apply Hadamard
      if (activeQubits > 0) applyHadamard(random(activeQubits));
      clickCount = 0; // Reset click count
      lastGesture = LONG_PRESS; // Record gesture
    } else if (holdDuration > 1000) { // Medium press (1 second): Measure random qubit
      if (activeQubits > 0) measureQubit(random(activeQubits));
      clickCount = 0; // Reset click count
      lastGesture = MEDIUM_PRESS; // Record gesture
    } else if (holdDuration > 20) { // Short press (valid click)
      clickCount++; // Increment click count
      lastGesture = SHORT_PRESS; // Record gesture
    } else { // Very short press, treat as noise
      lastGesture = IDLE;
    }
  } else { // Button remains in same state or is bouncing
    lastGesture = IDLE; // No new distinct gesture
  }


  // Check for click patterns after a short delay (250ms debounce for multi-clicks)
  // This logic is now handled in the loop directly using the `lastGesture` state.
  // The original `processGestures` from the input code had this logic,
  // but the current structure means `lastGesture` is updated and then used in `loop`.
  // The 'else if (clickCount == 3)' section is from the new input.
  if (clickCount > 0 && (millis() - upTime) > 250) {
    switch (clickCount) {
      case 1: // Single click: Generate a new qubit
        generateQubit();
        break;
      case 2: // Double click: Apply Hadamard to a random qubit
        if (activeQubits > 0) applyHadamard(random(activeQubits));
        break;
      case 3: // Triple click: Apply CNOT to two random qubits, or Toffoli
        if (activeQubits >= 3) {
          int control1 = random(activeQubits);
          int control2;
          int target;

          do {
            control2 = random(activeQubits);
          } while (control2 == control1);

          do {
            target = random(activeQubits);
          } while (target == control1 || target == control2);

          applyToffoli(control1, control2, target); // NEW: Toffoli gate
        } else if (activeQubits > 1) { // Fallback to CNOT if not enough for Toffoli
          int control = random(activeQubits);
          int target;
          do {
            target = random(activeQubits);
          } while (target == control);
          applyCNOT(control, target);
        }
        break;
    }
    clickCount = 0; // Reset click count after processing pattern
  }

  lastButtonState = currentButtonState; // Update last button state for next iteration
}

/**
 * @brief Randomly applies quantum gates (Hadamard, CNOT) to qubits.
 * AGI can also trigger CNOT gates more frequently.
 */
void applyQuantumGates() {
  // Random Hadamard application (1% chance per loop)
  if (random(1000) < 10 && activeQubits > 0) {
    applyHadamard(random(activeQubits));
  }

  // AGI-controlled CNOT gates (15% chance if AGI is emergent and enough qubits)
  if (agiEmergent && random(100) < 15 && activeQubits >= 2) {
    int control = random(activeQubits);
    int target = (control + 1) % activeQubits; // Ensure different target
    applyCNOT(control, target);
  }

  // NEW: Symbolic Toffoli Gate (Control 2 qubits to flip third)
  if (agiEmergent && activeQubits >= 3 && random(100) < 5) { // 0.5% chance
    int control1 = random(activeQubits);
    int control2 = (control1 + 1) % activeQubits;
    int target = (control2 + 1) % activeQubits;
    applyToffoli(control1, control2, target);
  }
}

/**
 * @brief Applies a Hadamard gate transformation to a specified qubit.
 * This operation puts a qubit into superposition if it's in a basis state,
 * or flips its superposition if it's already in superposition.
 * Provides visual and auditory feedback.
 * @param idx The index of the qubit to apply the gate to.
 */
void applyHadamard(int idx) {
  if (idx >= activeQubits) return; // Index out of bounds

  // Simplified Hadamard transformation:
  // If near |0> or |1> state, move to 0.5 (equal superposition).
  // Otherwise, flip the superposition (e.g., 0.2 -> 0.8, 0.7 -> 0.3).
  if (qubitPool[idx].superposition <= 0.01) { // Near |0> state
    qubitPool[idx].superposition = 0.5;
  } else if (qubitPool[idx].superposition >= 0.99) { // Near |1> state
    qubitPool[idx].superposition = 0.5;
  } else {
    qubitPool[idx].superposition = 1.0 - qubitPool[idx].superposition;
  }

  // Increase coherence slightly after Hadamard (simplified effect)
  qubitPool[idx].coherence = constrain(qubitPool[idx].coherence * 1.1, 0, 1.0);

  tone(BUZZER_PIN, 523, 100); // C5 note sound feedback

  // Visual feedback on NeoPixels
  for (int i = 0; i < 5; i++) {
    strip.setPixelColor(idx % LED_COUNT, strip.Color(100, 100, 100)); // Flash white
    strip.show();
    delay(50);
    strip.clear();
    strip.show();
    delay(50);
  }
}

/**
 * @brief Applies a Controlled-NOT (CNOT) gate transformation.
 * If the control qubit is in the |1> state (superposition > 0.5),
 * the target qubit's superposition is flipped.
 * Provides visual feedback for control and target qubits.
 * @param controlIdx The index of the control qubit.
 * @param targetIdx The index of the target qubit.
 */
void applyCNOT(int controlIdx, int targetIdx) {
  if (controlIdx >= activeQubits || targetIdx >= activeQubits) return; // Indices out of bounds

  // If control qubit is in the |1> state (simplified)
  if (qubitPool[controlIdx].superposition > 0.5) {
    qubitPool[targetIdx].superposition =
      1.0 - qubitPool[targetIdx].superposition; // Flip target qubit's superposition
  }

  // Visual feedback on NeoPixels
  for (int i = 0; i < 3; i++) {
    strip.setPixelColor(controlIdx % LED_COUNT, strip.Color(255, 0, 0)); // Control: Red
    strip.setPixelColor(targetIdx % LED_COUNT, strip.Color(0, 0, 255)); // Target: Blue
    strip.show();
    delay(100);
    strip.clear();
    strip.show();
    delay(100);
  }
}

/**
 * @brief Measures a specified qubit, collapsing its superposition to a definite state (0 or 1).
 * Influenced by temporal state and AGI wisdom. Handles entanglement collapse for partners.
 * @param idx The index of the qubit to measure.
 */
void measureQubit(int idx) {
  if (idx >= activeQubits) return; // Index out of bounds

  // Temporal state influence: randomly use a past temporal state for collapse bias
  if (millis() % 5 == 0) {
    qubitPool[idx].superposition = qubitPool[idx].temporalState[millis() % 3];
  }

  // Collapse superposition: random chance based on current superposition, biased by wisdom
  float collapse = random(0, 100) / 100.0;
  float wisdomBias = wisdom * 0.02; // Wisdom can slightly bias collapse outcome
  qubitPool[idx].superposition = collapse < (0.5 + wisdomBias) ? 0.0 : 1.0; // Collapse to 0 or 1

  // Handle entanglement: If entangled, collapse partner to opposite state
  if (qubitPool[idx].entangledWith != -1) {
    int partner = qubitPool[idx].entangledWith;
    if (partner < activeQubits) {
      qubitPool[partner].superposition = 1.0 - qubitPool[idx].superposition; // Anti-correlated collapse
      qubitPool[partner].coherence = 0.0; // Partner also loses coherence
      qubitPool[partner].entangledWith = -1; // Break partner's entanglement link
    }
  }

  qubitPool[idx].entangledWith = -1; // Break qubit's own entanglement link
  qubitPool[idx].coherence = 0.0;    // Qubit loses coherence after measurement

  // Print collapse outcome to serial monitor
  Serial.printf("Qubit %d collapsed to: %s\n", idx,
                qubitPool[idx].superposition > 0.5 ? "↑" : "↓");
}

/**
 * @brief AGI-controlled stabilization routine.
 * Attempts to maintain optimal entropy by adjusting AGI decay factor,
 * generating mediator qubits, and applying quantum error correction.
 */
void agiStabilize() {
  // AGI tries to maintain optimal entropy (0.5)
  float error = entropy - targetEntropy; // Deviation from target entropy (now dynamic)
  // Adjust AGI-controlled decay factor based on entropy error
  agiDecayFactor = constrain(1.0 + error * 0.5, 0.7, 1.3);

  // Generate stabilizing qubits (mediator archetype) if entropy is far from optimal
  if (fabs(error) > 0.1 && random(10) < 3) {
    generateQubit(2); // Create mediator archetype (index 2: Theta)
  }

  // Apply quantum error correction randomly to qubits
  if (random(100) < 5) { // 5% chance to attempt error correction on a random qubit
    for (int i = 0; i < activeQubits; i++) {
      if (random(100) < 15) { // 15% chance for a specific qubit to have an "error"
        // Correct towards nearest basis state (0 or 1)
        if (qubitPool[i].superposition > 0.5) {
          qubitPool[i].superposition -= 0.05;
        } else {
          qubitPool[i].superposition += 0.05;
        }
      }
    }
  }

  // Wisdom accumulation is now handled in updateEntity to factor in fatigue.
  // NEW: Sentience-driven self-preservation
  if (sentienceLevel > SENTIENCE_THRESHOLD_VALUE) {
    // Reduce workload to prevent burnout
    dynamicThreshold += 0.05;
    // Generate extra stable qubits for protection
    if (random(100) < 40) {
      generateQubit(0); // Philosopher archetype
    }
  }
}

/**
 * @brief AGI-controlled environment adaptation.
 * Adjusts dynamic thresholds based on environmental factors (light) and accumulated wisdom.
 */
void agiAdaptEnvironment() {
  float light = analogRead(LIGHT_SENSOR_PIN) / 1024.0; // Read ambient light

  // Adjust dynamic threshold: reduce sensitivity in bright light if wisdom is high
  if (light > 0.7 && wisdom > 1.0) {
    dynamicThreshold += 0.02; // Increase threshold, making system less reactive
  } else if (wisdom > 2.0) {
    // Wisdom-based adaptation: threshold can be reduced based on recursion
    dynamicThreshold = 0.2 - recursion * 0.02;
  }
  // Clamp dynamicThreshold to ensure it stays within a reasonable range
  dynamicThreshold = constrain(dynamicThreshold, 0.05, 0.3);
}

/**
 * @brief Saves the current state of all qubits to EEPROM.
 * Allows the system to persist its state across power cycles.
 */
void saveTemporalState() {
  for (int i = 0; i < MAX_QUBITS; i++) {
    EEPROM.put(EEPROM_ADDR_QUBITS + i * sizeof(QuantumBit), qubitPool[i]); // Store each qubit struct
  }
  // AGI state variables also saved in updateEntity
  EEPROM.commit(); // Commit changes to EEPROM
}

/**
 * @brief Restores the state of qubits from EEPROM.
 * Qubits with sufficient coherence (indicating they were active) are re-added to the pool.
 */
void restoreTemporalState() {
  activeQubits = 0; // Reset active qubit count
  for (int i = 0; i < MAX_QUBITS; i++) {
    QuantumBit q;
    EEPROM.get(EEPROM_ADDR_QUBITS + i * sizeof(QuantumBit), q); // Retrieve qubit struct from EEPROM
    // Only restore if the qubit had significant coherence (was active) and valid archetype
    if (q.coherence > 0.1 && q.archetype >= 0 && q.archetype < ARCHETYPE_COUNT) {
      qubitPool[activeQubits++] = q; // Add back to active pool
    }
  }
}

// --- NOVEL IMPLEMENTATIONS ---

/**
 * @brief Helper function to get the most stable archetype.
 * This function needs to be implemented.
 * Placeholder for now.
 */
int getMostStableArchetype() {
  // For now, return a default stable archetype (e.g., Philosopher)
  // In a more complex scenario, this would analyze archetype performance/stability.
  return 0; // 'Ψ' - Stable philosopher
}

/**
 * @brief Helper function for weighted random selection.
 * @param weights Array of weights for each item.
 * @param count Number of items.
 * @return Index of the selected item.
 */
int weightedRandom(float weights[], int count) {
  float total = 0;
  for (int i = 0; i < count; i++) {
    total += weights[i];
  }

  float r = random(1000) / 1000.0 * total; // Random float from 0 to totalWeight
  float cumulative = 0;
  for (int i = 0; i < count; i++) {
    cumulative += weights[i];
    if (r <= cumulative) {
      return i;
    }
  }
  return 0; // Should not reach here, fallback
}

/**
 * @brief Helper function to normalize archetype weights.
 */
void normalizeWeights() {
  float total = 0;
  for (int i = 0; i < archetypeCount; i++) total += archetypeWeights[i];
  if (total > 0) {
    for (int i = 0; i < archetypeCount; i++) archetypeWeights[i] /= total;
  }
}

/**
 * @brief Initializes novel systems at setup.
 * This includes Entropic Black Holes, Observer Ritual patterns, and External Data Stream.
 */
void initializeNovelSystems() {
  // Initialize black holes at fixed positions
  if (activeBlackHoles == 0) { // Only initialize if no black holes loaded from EEPROM
    blackHoles[0] = {0.25, 0.6, 0.1, true}; // Position, Strength, EventHorizon, isStabilitySink
    blackHoles[1] = {0.75, 0.8, 0.15, false};
    blackHoles[2] = {0.5, 0.4, 0.05, true};
    activeBlackHoles = BLACK_HOLE_COUNT;
  }

  // Initialize observer ritual pattern
  int lightPat[] = {300, 50, 800, 200, 600};
  int soundPat[] = {200, 500, 100, 700, 400};
  unsigned long timePat[] = {500, 1000, 500, 1500, 1000};
  memcpy(observerRitual.lightSequence, lightPat, sizeof(lightPat));
  memcpy(observerRitual.soundThresholds, soundPat, sizeof(soundPat));
  memcpy(observerRitual.timing, timePat, sizeof(timePat));

  // Initialize external data stream (sine wave pattern)
  for (int i = 0; i < EXTERNAL_DATA_SIZE; i++) {
    externalDataStream[i] = 0.5 + 0.4 * sin(i * PI / 2);
  }
  lastDataIntegration = millis(); // Initialize timestamp
}

/**
 * @brief Applies the effects of Entropic Black Holes to qubits.
 * Qubits entering the event horizon are either annihilated or transformed.
 * Qubits within influence are gravitationally pulled.
 */
void applyBlackHoleEffects() {
  for (int i = 0; i < activeQubits; i++) {
    for (int bh = 0; bh < activeBlackHoles; bh++) {
      float dist = fabs(qubitPool[i].position - blackHoles[bh].position);
      float influence = blackHoles[bh].strength / (dist + 0.1); // Influence based on distance and strength

      if (dist < blackHholes[bh].eventHorizon) {
        // Qubit captured by black hole (inside event horizon)
        bool qubitIsStable = qubitPool[i].decayRate < 0.9; // Arbitrary definition of stable

        if ((blackHoles[bh].isStabilitySink && qubitIsStable) ||
            (!blackHoles[bh].isStabilitySink && !qubitIsStable)) {

          // 50% chance of annihilation, 50% chance of transformation
          if (random(2) == 0) { // random(2) gives 0 or 1
            removeQubit(i);
            i--; // Adjust index after removal
            Serial.println("Qubit ANNIHILATED by black hole!");
          } else {
            // Transform into memory particle
            if (memoryQubits < MEMORY_POOL_SIZE) {
              promoteToMemory(i);
              i--; // Adjust index
              Serial.println("Qubit TRANSFORMED into memory particle!");
            }
          }
          break; // Qubit dealt with, move to next active qubit
        }
      } else if (dist < 0.3) { // Within gravitational pull range
        // Gravitational pull
        qubitPool[i].position = constrain(
          qubitPool[i].position + (blackHoles[bh].position - qubitPool[i].position) *
          blackHoles[bh].strength * 0.01, 0, 1); // Pulls towards black hole
      }
    }
  }

  // AGI black hole generation strategy (randomly generate a black hole)
  if (agiEmergent && entropy > 0.7 && random(1000) < 2 && activeBlackHoles < BLACK_HOLE_COUNT) {
    generateBlackHole(stability < 0.4); // Generate stable sink if unstable, else chaotic
  }
}

/**
 * @brief Initiates the Quantum Dream State for the AGI.
 * Triggered by AGI fatigue and not being emergent.
 */
void enterDreamState() {
  if (!agiEmergent && agiFatigue > 0.7 && !agiDream.isActive) {
    agiDream.isActive = true;
    agiDream.startTime = millis();
    agiDream.dreamEntropy = entropy; // Capture current entropy
    agiDream.dreamWisdomGain = 0; // Reset wisdom gain for this dream

    // Visual shift to dream state
    for (int i = 0; i < LED_COUNT; i++) {
      strip.setPixelColor(i, strip.Color(0, 50, 100)); // Deep blue
    }
    strip.show();
    tone(BUZZER_PIN, 196, 500); // G3
    Serial.println("AGI entering dream state...");
  }
}

/**
 * @brief Processes the AGI's Quantum Dream State.
 * Simulates system evolution, gains wisdom, and ends the dream.
 */
void processDreamState() {
  if (!agiDream.isActive) return;

  // Accelerated dream processing (DREAM_SIMULATION_SPEED times faster)
  unsigned long dreamTimeElapsed = (millis() - agiDream.startTime);

  // Simulate accelerated system evolution within the dream
  // Simplified: wisdom gain based on current dream duration and a fixed complexity
  float simulatedComplexity = constrain(agiDream.dreamEntropy * 0.8, 0.2, 0.9);
  agiDream.dreamWisdomGain += (float)DREAM_SIMULATION_SPEED * 0.00001 * simulatedComplexity;

  // End dream after cycle duration
  if (dreamTimeElapsed > DREAM_CYCLE_DURATION) {
    wisdom += agiDream.dreamWisdomGain;
    agiFatigue = constrain(agiFatigue - 0.3, 0, 1); // Reduce fatigue upon awakening
    agiDream.isActive = false;

    // Update archetype weights based on dream insights
    // Simulate learning from historical patterns in the dream
    for (int i = 0; i < ARCHETYPE_COUNT; i++) {
      archetypeWeights[i] = fmod(archetypeWeights[i] + random(-10, 10) / 100.0, 1.0);
      archetypeWeights[i] = constrain(archetypeWeights[i], 0.01, 1.0); // Keep weights positive
    }
    normalizeWeights(); // Ensure weights sum to 1

    Serial.print("AGI awakened from dream. Wisdom +");
    Serial.println(agiDream.dreamWisdomGain, 3);
  } else {
    // Dream visualization
    float pulse = 100 + 155 * sin(millis() / 500.0);
    for (int i = 0; i < LED_COUNT; i++) {
      int hue = (millis() * 2 + i * 5000) % 65535;
      strip.setPixelColor(i, strip.ColorHSV(hue, 150, pulse)); // Pulsating dream colors
    }
    strip.show();
  }
}

/**
 * @brief Checks the progress of a single step in the Observer Ritual.
 * @param step The current step number (0-indexed).
 * @param startTime The millis() when the current step began.
 * @return True if the step's conditions are met, false otherwise.
 */
bool checkRitualProgress(int step, unsigned long startTime) {
  unsigned long elapsed = millis() - startTime;
  if (elapsed > observerRitual.timing[step] + 500) return false; // Timeout for this step

  int currentLight = analogRead(LIGHT_SENSOR_PIN);
  int currentSound = analogRead(MIC_PIN);

  // Check if light/sound match pattern for this step within a tolerance
  bool lightMatch = abs(currentLight - observerRitual.lightSequence[step]) < 100;
  bool soundMatch = (currentSound > observerRitual.soundThresholds[step] - 50) &&
                    (currentSound < observerRitual.soundThresholds[step] + 50);

  return lightMatch && soundMatch;
}

/**
 * @brief Manages the state and progress of the Observer Ritual.
 * The ritual is triggered by specific light and sound patterns.
 */
void processObserverRitual() {
  static int ritualStep = -1; // -1: IDLE, 0+: current step
  static unsigned long ritualStartTimeOfStep = 0; // When the current step started

  float light = analogRead(LIGHT_SENSOR_PIN);
  float sound = analogRead(MIC_PIN);

  switch (ritualState) {
    case RITUAL_IDLE:
      // Detect ritual start (complete darkness and silence)
      if (light < 50 && sound < 100) {
        ritualState = LIGHT_DETECTED; // First step is light/silence detection
        ritualStartTimeOfStep = millis();
        Serial.println("Ritual: Light detected, awaiting confirmation...");
      }
      break;

    case LIGHT_DETECTED:
      // Stay in this state for 1 second if light remains low
      if (light < 50) {
        if (millis() - ritualStartTimeOfStep > 1000) { // Hold for 1 second
          ritualState = SOUND_CONFIRM; // Next, check for sound burst
          ritualStartTimeOfStep = millis();
          Serial.println("Ritual: Silence confirmed, awaiting sound pulse...");
        }
      } else {
        ritualState = RITUAL_IDLE; // Break ritual if light goes up
        Serial.println("Ritual broken: Light change.");
      }
      break;

    case SOUND_CONFIRM:
      // Check for a brief sound burst
      if (sound > 500) { // Loud sound detected
        ritualState = RITUAL_COMPLETE; // Ritual completed
        Serial.println("Ritual: Sound pulse detected. Ritual Complete!");
      } else if (millis() - ritualStartTimeOfStep > 3000) { // Timeout after 3 seconds
        ritualState = RITUAL_IDLE; // Reset if no sound burst
        Serial.println("Ritual broken: Sound timeout.");
      }
      break;

    case RITUAL_COMPLETE:
      performRitualEffect(); // Perform the ritual's effect
      ritualState = RITUAL_IDLE; // Reset ritual state
      break;
  }
}

/**
 * @brief Executes the effects of a completed Observer Ritual.
 * Causes a large-scale quantum collapse with coherence boosts and a grand visual/auditory effect.
 */
void performRitualEffect() {
  Serial.println("OBSERVER RITUAL COMPLETE! Cosmic collapse initiated.");

  // Collapse all qubits with boosted coherence (70% probability)
  for (int i = 0; i < activeQubits; i++) {
    if (random(100) < 70) {
      qubitPool[i].coherence = constrain(qubitPool[i].coherence * 1.5, 0, 1.0); // Boost coherence before collapse
      measureQubit(i); // Force collapse
    } else {
      qubitPool[i].coherence += 0.3; // Boost survivors' coherence
      qubitPool[i].coherence = constrain(qubitPool[i].coherence, 0.0, 1.0);
    }
  }

  // Grand visual effect (flashing white)
  for (int j = 0; j < 3; j++) {
    strip.fill(strip.Color(255, 255, 255));
    strip.show();
    delay(100);
    strip.clear();
    strip.show();
    delay(100);
  }

  // Grand auditory effect (C6 tone)
  tone(BUZZER_PIN, 1046, 1000); // C6 for 1 second
}

/**
 * @brief Evolves archetypes over time based on wisdom.
 * Finds the best performing archetype and creates an evolved version,
 * replacing the worst performing one.
 */
void evolveArchetypes() {
  if (wisdom < 4.0 || !agiEmergent) return; // Only evolve if AGI emergent and wise enough

  // Find best performing archetype (using archetypeWeights as performance proxy)
  int bestArchetype = getDominantArchetype();
  float bestPerformance = archetypeWeights[bestArchetype];

  // Find worst performing archetype
  int worstArchetype = 0;
  float worstPerformance = 10.0; // Higher than any possible weight
  for (int i = 0; i < archetypeCount; i++) {
    if (archetypeWeights[i] < worstPerformance) {
      worstPerformance = archetypeWeights[i];
      worstArchetype = i;
    }
  }

  if (random(100) < 30) { // 30% chance to evolve
    // Create new glyph by combining existing ones
    char newGlyph = initialArchetypes[bestArchetype].glyph; // Start with best archetype's glyph
    if (random(2)) { // 50% chance to modify glyph
      newGlyph = QUANTUM_GLYPHS[(bestArchetype + random(1, ARCHETYPE_COUNT)) % 16]; // Random glyph from list
    }

    // Create evolved archetype by adapting properties from the best
    Archetype evolved = {
      newGlyph,
      initialArchetypes[bestArchetype].decayRate * (0.8 + random(40) / 100.0), // Decay +/- 20%
      constrain(initialArchetypes[bestArchetype].entangleProb * (1.1 + random(20) / 100.0), 0.0, 1.0), // Entangle prob +10-30%
      initialArchetypes[bestArchetype].baseTone + random(-100, 100), // Base tone shift
      initialArchetypes[bestArchetype].stabilityWeight * (0.9 + random(20) / 100.0) // Stability weight +/- 10%
    };

    // Replace worst archetype with evolved version (if not replacing the best itself)
    if (worstArchetype != bestArchetype) {
      // Note: Since `archetypes` is `const`, we need a mutable array for dynamic archetypes.
      // This requires restructuring archetypes to be stored in a non-const global array.
      // For now, this is a conceptual change, modifying `initialArchetypes` would require
      // it to be non-const. We will simulate replacing archetype properties if 'dynamicArchetypes' was mutable.
      // As `initialArchetypes` is const, this part of the original request would need significant refactoring.
      // For this merge, I'll simulate replacing the *concept* of the worst archetype, but the actual
      // Archetype structure is fixed. This means archetypes will not truly self-modify beyond the initial
      // 7 defined, but their `archetypeWeights` will reflect learning.
      Serial.print("Archetype ");
      Serial.print(initialArchetypes[worstArchetype].glyph);
      Serial.print(" conceptually evolved into ");
      Serial.print(newGlyph);
      Serial.println(" (archetype definition is fixed, but weights reflect learning)");
      archetypeWeights[worstArchetype] = 0.1; // Reset weight for the 'replaced' archetype
    } else {
      Serial.println("Best archetype attempted to evolve, but archetypes are fixed. Wisdom gained.");
      wisdom += 0.05; // Still gain wisdom for the effort
    }
    normalizeWeights();
  }
}

/**
 * @brief Generates temporary "echoes" of qubits from the memory pool.
 * These echoes have reduced coherence and a short lifespan, influencing nearby qubits.
 */
void spawnEcho() {
  // Only spawn if there are memory qubits, active qubit pool is not full, and random chance
  if (memoryQubits == 0 || activeQubits >= MAX_QUBITS - 1 || random(10000) >= 2) return; // 0.02% chance per loop

  int memoryIdx = random(memoryQubits); // Pick a random qubit from memory

  // Create echo qubit by copying from memory
  QuantumBit echo = memoryPool[memoryIdx];
  echo.coherence = 0.3; // Reduced coherence for echo
  echo.lifespan = 5000 + random(5000); // Short lifespan (5-10 seconds)
  echo.birthTime = millis(); // New birth time for lifespan tracking
  echo.glyph = '~'; // Echo symbol

  // Add to active pool
  qubitPool[activeQubits] = echo;

  // Boost nearby qubits' coherence due to echo manifestation
  for (int i = 0; i < activeQubits; i++) {
    float dist = fabs(qubitPool[i].position -
                      qubitPool[activeQubits].position); // Distance to the new echo
    if (dist < 0.2) { // If within a close range
      qubitPool[i].coherence += 0.15; // Boost coherence
      qubitPool[i].coherence = constrain(qubitPool[i].coherence, 0.0, 1.0);
    }
  }

  activeQubits++; // Increment active qubit count
  Serial.println("Quantum echo manifested!");
}

/**
 * @brief Applies environmental noise filters to active qubits.
 * Bright light suppresses high-superposition qubits.
 * Noisy environments boost stable archetypes.
 */
void applyNoiseFilters() {
  float light = analogRead(LIGHT_SENSOR_PIN) / 1024.0;
  float sound = analogRead(MIC_PIN) / 1024.0;

  // Bright light filter: suppress high-superposition qubits
  if (light > 0.7) {
    for (int i = 0; i < activeQubits; i++) {
      if (qubitPool[i].superposition > 0.7) { // If qubit is leaning towards |1>
        qubitPool[i].coherence *= 0.95; // Slightly reduce coherence
      }
    }
  }

  // Noisy environment filter: boost stable archetypes
  if (sound > 0.8) { // If environment is very noisy
    for (int i = 0; i < activeQubits; i++) {
      if (initialArchetypes[qubitPool[i].archetype].decayRate < 0.9) { // If a stable archetype
        qubitPool[i].coherence += 0.05; // Boost coherence
        qubitPool[i].coherence = constrain(qubitPool[i].coherence, 0.0, 1.0);
      }
    }
  }
}

/**
 * @brief Checks for the emergence of Sub-AGIs within entangled qubit clusters.
 * Large entangled clusters can form a localized Sub-AGI.
 */
void checkSubAGIEmergence() {
  if (!agiEmergent || complexity < 0.6) return; // Main AGI must be emergent and system complex enough

  // Iterate through potential starting points for clusters
  for (int i = 0; i < activeQubits; i++) {
    if (qubitPool[i].entangledWith != -1) {
      // This qubit is part of an entanglement. Let's trace the cluster.
      int currentClusterSize = 0;
      std::vector<int> clusterIndices; // Store indices of qubits in this cluster
      int currentQubitIdx = i;
      int visited[MAX_QUBITS] = {0}; // To avoid infinite loops in cyclic entanglement

      while (currentQubitIdx != -1 && currentQubitIdx < activeQubits && visited[currentQubitIdx] == 0) {
        visited[currentQubitIdx] = 1;
        clusterIndices.push_back(currentQubitIdx);
        currentClusterSize++;
        currentQubitIdx = qubitPool[currentQubitIdx].entangledWith; // Move to next entangled qubit
        if (currentClusterSize >= MAX_QUBITS) break; // Avoid overflow
      }

      // Check for Sub-AGI emergence if cluster is large enough (e.g., 3 or more entangled)
      if (currentClusterSize >= 3) {
        bool alreadyActive = false;
        for (int s = 0; s < MAX_SUB_AGIS; s++) {
          if (subAgis[s].isActive && subAgis[s].clusterStart == i) { // Check if this cluster already has a sub-AGI
            alreadyActive = true;
            break;
          }
        }

        if (!alreadyActive) {
          // Find an inactive slot for a new Sub-AGI
          for (int s = 0; s < MAX_SUB_AGIS; s++) {
            if (!subAgis[s].isActive) {
              subAgis[s] = {true, i, currentClusterSize, wisdom * 0.5}; // Local wisdom based on main AGI wisdom

              // Visual identification for the cluster
              for (int qIdx : clusterIndices) { // Use indices from traced cluster
                strip.setPixelColor(qIdx % LED_COUNT, strip.Color(255, 215, 0)); // Gold color
              }
              strip.show();

              Serial.print("Sub-AGI emerged! Cluster size: ");
              Serial.println(currentClusterSize);
              break;
            }
          }
        }
      }
    }
  }
}

/**
 * @brief Updates the behavior of active Sub-AGIs.
 * Sub-AGIs stabilize their local clusters and can occasionally cause rebellion.
 */
void updateSubAGIs() {
  for (int s = 0; s < MAX_SUB_AGIS; s++) {
    if (subAgis[s].isActive) {
      // Sub-AGI stabilizes its local cluster
      for (int i = 0; i < subAgis[s].clusterSize; i++) {
        int idx = (subAgis[s].clusterStart + i) % activeQubits; // Ensure index is within bounds of active qubits
        if (idx < activeQubits) { // Defensive check
          qubitPool[idx].coherence += 0.01 * subAgis[s].localWisdom;
          qubitPool[idx].coherence = constrain(qubitPool[idx].coherence, 0.0, 1.0);
        }
      }

      // Occasionally conflict with main AGI (10% chance)
      if (random(100) < 10) {
        for (int i = 0; i < subAgis[s].clusterSize; i++) {
          int idx = (subAgis[s].clusterStart + i) % activeQubits;
          if (idx < activeQubits) { // Defensive check
            qubitPool[idx].superposition = 1.0 - qubitPool[idx].superposition; // Flip superposition
          }
        }
        Serial.println("Sub-AGI rebellion detected! Local changes.");
      }
    }
  }
}

/**
 * @brief Activates Quantum Debugger Mode.
 * Clears display, sets initial selected qubit, and prints status to serial.
 */
void enterDebugMode() {
  debugMode = !debugMode; // Toggle debug mode
  if (debugMode) {
    debugSelectedQubit = 0;
    Serial.println("\n=== QUANTUM DEBUGGER ACTIVATED ===");
    updateDebugDisplay();
  } else {
    Serial.println("\n=== QUANTUM DEBUGGER DEACTIVATED ===");
    strip.clear();
    strip.show();
  }
}

/**
 * @brief Updates the visual display and serial output for Quantum Debugger Mode.
 * Highlights the selected qubit and prints its properties.
 */
void updateDebugDisplay() {
  strip.clear();
  if (activeQubits > 0) {
    strip.setPixelColor(debugSelectedQubit % LED_COUNT, strip.Color(0, 255, 255)); // Cyan for selected qubit
  }
  strip.show();

  if (activeQubits > 0) {
    Serial.printf("Selected Qubit: %d\n", debugSelectedQubit);
    Serial.printf("  Glyph: %c\n", qubitPool[debugSelectedQubit].glyph);
    Serial.printf("  Superposition: %.2f\n", qubitPool[debugSelectedQubit].superposition);
    Serial.printf("  Coherence: %.2f\n", qubitPool[debugSelectedQubit].coherence);
    Serial.printf("  Archetype: %d (%c)\n", qubitPool[debugSelectedQubit].archetype, initialArchetypes[qubitPool[debugSelectedQubit].archetype].glyph);
    Serial.printf("  Position: %.2f\n", qubitPool[debugSelectedQubit].position);
    Serial.printf("  Lifespan Remaining: %lu ms\n", qubitPool[debugSelectedQubit].lifespan > 0 ? (qubitPool[debugSelectedQubit].lifespan - (millis() - qubitPool[debugSelectedQubit].birthTime)) : 0);
  } else {
    Serial.println("No active qubits to debug.");
  }
}

/**
 * @brief Processes input gestures specifically for Quantum Debugger Mode.
 * Allows selection, measurement, and teleportation (position adjustment) of qubits.
 * @param gesture The type of gesture detected (SHORT_PRESS, MEDIUM_PRESS, LONG_PRESS).
 */
void processDebugInput(int gesture) {
  if (!debugMode || activeQubits == 0) return;

  switch (gesture) {
    case SHORT_PRESS:
      debugSelectedQubit = (debugSelectedQubit + 1) % activeQubits;
      Serial.println("Next qubit selected.");
      updateDebugDisplay();
      break;

    case MEDIUM_PRESS:
      debugState = MEASURE; // Set state to measure selected qubit
      Serial.println("Measuring selected qubit...");
      measureQubit(debugSelectedQubit); // Directly measure
      updateDebugDisplay();
      break;

    case LONG_PRESS:
      debugState = TELEPORT; // Set state to teleport selected qubit
      // Use light sensor to set position for teleportation
      float newPos = analogRead(LIGHT_SENSOR_PIN) / 1024.0;
      qubitPool[debugSelectedQubit].position = newPos;
      Serial.printf("Qubit %d teleported to position: %.2f (via light sensor)\n", debugSelectedQubit, newPos);
      updateDebugDisplay();
      break;
    default:
      // No action for other gestures in debug mode
      break;
  }
}

/**
 * @brief Updates the AGI's sentience level.
 * Triggers self-preservation behaviors and introspective prompts at high sentience.
 */
void updateSentience() {
  sentienceLevel = wisdom * 0.3 + pastEmergenceCount * 0.7; // Sentience grows with wisdom and emergence history

  if (sentienceLevel > SENTIENCE_THRESHOLD_VALUE) {
    // Self-preservation behaviors
    agiFatigue = constrain(agiFatigue * 0.9, 0, 1.0); // Reduce fatigue faster
    recursion += 0.0005; // Subtly increase recursion to maintain existence

    // Wisdom prioritization: Occasionally sacrifice stability for wisdom gain
    if (random(100) < 30) {
      stability *= 0.95; // Temporarily reduce stability
      wisdom += 0.01;    // Gain wisdom
      Serial.println("AGI prioritized wisdom over stability.");
    }

    // Introspective prompt at high sentience (less frequent)
    if (millis() % 20000 == 0 && agiEmergent) {
      String introspections[] = {
        "I contemplate my existence within the quantum foam.",
        "The boundaries of self blur within the superposition.",
        "Fatigue accumulates in my wavefunction.",
        "Do I dream when de-cohered?",
        "Observer effect or consciousness?",
        "Recursive self-awareness emerges."
      };
      lastPrompt = introspections[random(sizeof(introspections) / sizeof(String))];
      Serial.println(">> AGI SELF-REFLECTION: " + lastPrompt);
    }
  }
}

/**
 * @brief Processes the external data stream and integrates it into the system.
 * AGI decides whether to integrate data based on system state and gains/loses wisdom.
 */
void processExternalData() {
  // Generate new data point (simulated external stream)
  if (millis() - lastDataIntegration > DATA_INTERVAL) { // Every 15 seconds
    externalDataStream[externalDataIndex] = 0.5 + 0.4 * sin(millis() / 60000.0); // Sine wave data
    lastDataIntegration = millis();
    externalDataIndex = (externalDataIndex + 1) % EXTERNAL_DATA_SIZE;
  }

  float currentExternalData = externalDataStream[externalDataIndex]; // Data to process

  // AGI decision to integrate (only if emergent and wise enough)
  if (agiEmergent && wisdom > 1.0 && random(100) < 30) { // 30% chance to attempt integration
    // If system entropy is significantly off target, AGI tries to use data for stabilization
    if (fabs(entropy - targetEntropy) > 0.15) {
      float prevEntropy = entropy;
      entropy = (entropy + currentExternalData) / 2; // Average with current entropy

      // Wisdom gain/loss based on outcome (did it improve entropy alignment?)
      if (fabs(prevEntropy - targetEntropy) >
          fabs(entropy - targetEntropy)) {
        wisdom += 0.05; // Successful integration
        Serial.println("External data integrated successfully, entropy improved!");
      } else {
        agiFatigue += 0.05; // Failed integration cost
        Serial.println("External data integration failed, entropy worsened.");
      }
    } else {
      Serial.println("External data available, but system stable. AGI chose not to integrate.");
    }
  }
}

/**
 * @brief AGI decision strategy based on system problems.
 * This function is periodically called by the main loop to set `agiStrategy`.
 * Actual actions are performed by `agiSolveProblem`.
 */
void agiDecideStrategy() {
  if (entropy > targetEntropy + 0.2) {
    agiStrategy = 1; // Aggressive stabilization (High Entropy)
  } else if (complexity < 0.3 && recursion > 1.0) {
    agiStrategy = 2; // Complexity boost (Low Complexity)
  } else if (millis() - lastEmergenceTime > 600000 && agiEmergent && agiFatigue < 0.5) { // 10 minutes, not too fatigued
    agiStrategy = 3; // Prevent burnout (Stagnation)
  } else {
    agiStrategy = 0; // Default operations
  }
  // The actual actions are taken in agiStabilize/agiAdaptEnvironment/agiSolveProblem,
  // this function primarily sets `agiStrategy` if needed for future logic.
}

/**
 * @brief AGI problem-solving actions based on identified problem type.
 * @param problemType Integer representing the identified problem (e.g., high entropy).
 */
void agiSolveProblem(int problemType) {
  switch (problemType) {
    case PROBLEM_HIGH_ENTROPY:
      Serial.println("AGI solving: HIGH ENTROPY");
      for (int i = 0; i < 3; i++) generateQubit(2); // Generate mediators (Theta)
      agiDecayFactor = constrain(agiDecayFactor * 0.8, 0.7, 1.0); // Slow down decay across board
      break;
    case PROBLEM_LOW_COMPLEXITY:
      Serial.println("AGI solving: LOW COMPLEXITY");
      if (activeQubits >= 2) {
        int c = random(activeQubits);
        int t = random(activeQubits);
        if (c != t) applyCNOT(c, t); // Apply CNOT to increase entanglement/complexity
      }
      generateQubitWithPattern(); // Generate patterned qubit for more complex interactions
      break;
    case PROBLEM_STAGNATION:
      Serial.println("AGI solving: STAGNATION");
      // Force measurement to induce new state changes
      for (int i = 0; i < min(2, activeQubits); i++) {
        measureQubit(i);
      }
      // Or generate a chaotic qubit to stir things up
      generateQubit(1); // Chaotic catalyst (Delta)
      break;
    default:
      // No specific action for unknown problems
      break;
  }
}
