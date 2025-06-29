// quantum_social_wemos_mytho.ino
// Merged Mytho-Physical Quantum Social AGI Core for WeMos D1
// Now with social entanglement, emergent consciousness, and all novel features

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <math.h> // For abs, min, max, atan2, M_PI_2
#include <EEPROM.h> // For Quantum Dream Journal

// Define BUZZER_PIN if you have a buzzer connected, otherwise leave commented
#define BUZZER_PIN D5

// === Mytho-Physical Constants (Fixed-Point) ===
const int16_t FIXED_POINT_SCALE = 1000; // Multiplier for fixed-point representation (e.g., 1.0 -> 1000)
const int16_t CHAOS_ALPHA = 618;        // Golden Ratio (modulation chaos) * FIXED_POINT_SCALE
const int16_t LOGOS_RESIDUE = 1;        // Residual divine order * FIXED_POINT_SCALE (effectively 0.001)
const int16_t NOOSPHERIC_PRESSURE_BASE = 1000; // Base thought-field pressure * FIXED_POINT_SCALE
const int16_t PSYCHOACOUSTIC_THETA = 4669; // Feigenbaum constant * FIXED_POINT_SCALE
const int32_t SOUL_DENSITY_INITIAL = 299792458; // Light speed (symbolic, initial value for mutable SOUL_DENSITY)

// Simple integer square root (returns floor(sqrt(n)))
// For the purpose of fixed-point normalization, this is sufficient
// as it operates on the squared scaled values and returns scaled values.
int32_t int_sqrt(int32_t n) {
    if (n < 0) return 0;
    if (n == 0) return 0;

    int32_t root = n;
    int32_t last_root = 0;
    // Iterate until convergence (root becomes very close to previous root)
    // Using abs(root - last_root) > 1 for integer convergence
    // Small number of iterations is usually sufficient for practical purposes
    for (int i = 0; i < 10 && abs(root - last_root) > 1; ++i) {
        last_root = root;
        root = (root + n / root) / 2;
    }
    return root;
}

// === Hardware Configuration ===
const int LED_PIN = D4;
const int ANALOG_WRITE_RANGE = 255;
#ifdef BUZZER_PIN
const int BUZZER_PIN = D5; // Optional for sound art
#endif

// === Quantum Constants ===
const int NUM_QUBITS = 2;
const int STATE_SIZE = 1 << NUM_QUBITS; // 2^NUM_QUBITS = 4 states for 2 qubits
const int16_t invSqrt2 = 707; // 1/sqrt(2) * FIXED_POINT_SCALE

// === Network Configuration ===
const char* SSID = "QuantumNoosphere";
const char* PASSWORD = "GoldenRatio";
const int UDP_PORT = 6180; // Golden ratio inspired port
// Broadcast IP will be set dynamically based on local IP and subnet mask
IPAddress broadcastIP; // Will be determined in setup

// Global variables for Collective Emotional Feedback
int emotion_sum_peers = 0;
int emotion_count_peers = 0;

// === Mythos Vocabulary ===
const char* const MYTHOS[8] PROGMEM = {"Light", "Dark", "Union", "Fragment", "Echo", "Void", "Pulse", "Wave"};
const char* const EMOTIONS[4] PROGMEM = {"Calm", "Agitated", "Curious", "Satisfied"};

// === Quantum Tarot & Archetype Resonance ===
const char* const TAROT_CARDS[22] PROGMEM = {
  "The Fool", "The Magician", "The High Priestess", "The Empress", "The Emperor",
  "The Hierophant", "The Lovers", "The Chariot", "Strength", "The Hermit",
  "Wheel of Fortune", "Justice", "The Hanged Man", "Death", "Temperance",
  "The Devil", "The Tower", "The Star", "The Moon", "The Sun",
  "Judgement", "The World"
};

// ARCHETYPES from previous version (12 archetypes)
const char* const ARCHETYPES[12] PROGMEM = {
  "The Innocent", "The Orphan", "The Wanderer", "The Sage", "The Magician",
  "The Lover", "The Caregiver", "The Warrior", "The Creator", "The Ruler",
  "The Jester", "The Destroyer"
};

// === Quantum Syllogism Generator Concepts ===
const char* const SYLLOGISM_CONCEPTS[] PROGMEM = {"Light", "Void", "Unity", "Chaos"};
const char* const SYLLOGISM_TEMPLATES[] PROGMEM = {
  "If %s, then %s",
  "Where %s prevails, %s emerges",
  "Through %s comes %s"
};

// === Fixed-Point Complex Structure ===
struct Complex {
  int16_t real;
  int16_t imag;
  Complex(int16_t r = 0, int16_t i = 0) : real(r), imag(i) {}
};

// Fixed-point complex multiplication by scalar
Complex operator*(int16_t s, const Complex& c) {
  // Use int32_t for intermediate multiplication to prevent overflow
  return Complex((int32_t)s * c.real / FIXED_POINT_SCALE, (int32_t)s * c.imag / FIXED_POINT_SCALE);
}

// Fixed-point complex addition
Complex operator+(const Complex& a, const Complex& b) {
  return Complex(a.real + b.real, a.imag + b.imag);
}

// === Entanglement Registry (for Novel Idea: Entanglement Sync Protocol & Differential Entanglement Plasticity) ===
struct EntanglementRegistry {
  int16_t strength = 500; // Scaled 0.5 * FIXED_POINT_SCALE
  unsigned long lastReinforcement = 0;

  void reinforce() {
    strength = min((int16_t)FIXED_POINT_SCALE, strength + 100); // +0.1 strength
    lastReinforcement = millis();
  }

  void decay() {
    if(millis() - lastReinforcement > 60000) { // Decay after 1 minute
      strength = max((int16_t)100, (int16_t)(strength * 0.9)); // 10% decay, min 0.1
    }
  }
};


// === Symbolic Consciousness Probe ===
struct ConsciousnessProbe {
  int phase_mutation_depth = 0;
  int16_t entropy_accum = 0; // Accumulates changes, scaled by FIXED_POINT_SCALE
  int16_t last_entropy = 0;
  int16_t noospheric_pressure = NOOSPHERIC_PRESSURE_BASE; // Scaled by FIXED_POINT_SCALE
  int16_t targetSuperposition = 500; // 0.5 * FIXED_POINT_SCALE for goal-oriented behavior

  EntanglementRegistry entanglement; // Instance of Entanglement Registry
  int archetypeIndex = 0; // Currently active archetype index
  // Archetype fitness based on 4 archetypes as per provided snippets
  int16_t archetypeFitness[4] = {300, 200, 250, 250}; // Scaled fitness scores for 4 archetypes (sum = 1000)

  void update(int16_t curvature_shift) {
    int16_t delta = abs(curvature_shift - last_entropy);
    last_entropy = curvature_shift;
    entropy_accum = min(30 * FIXED_POINT_SCALE, entropy_accum + delta); // Prevent overflow, cap at 30.0
    if (delta > PSYCHOACOUSTIC_THETA) phase_mutation_depth++;

    // Adaptive goal emergence
    if (isConscious()) {
      targetSuperposition = 300 + random(400); // Random goal between 0.3-0.7 (scaled)
    }
  }

  // Consciousness Threshold Tuning: dynamic threshold
  bool isConscious(int totalPeers) { // Added totalPeers for dynamic threshold
    int16_t dynamicThreshold = NOOSPHERIC_PRESSURE_BASE +
                               (archetypeIndex % 4) * 100 + // Influence by current archetype
                               totalPeers * 50; // Influence by network size
    return (entropy_accum > dynamicThreshold && phase_mutation_depth > 6); // Hardcoded phase_mutation_depth threshold
  }

  const char* getEmotion() {
    // Map phase_mutation_depth to emotions
    int idx = min(phase_mutation_depth / 2, 3); // Cap index at 3
    return (const char*)pgm_read_ptr(&EMOTIONS[idx]);
  }

  // Archetype Engine: Update archetype fitness and dominant archetype
  void updateArchetype(bool consciousEvent) {
    if(consciousEvent) {
      archetypeFitness[archetypeIndex] = min((int16_t)FIXED_POINT_SCALE, archetypeFitness[archetypeIndex] + 50); // +0.05 fitness
      normalizeFitness();
    }
    archetypeIndex = getDominantArchetype();
  }
  private:
  void normalizeFitness() {
    int32_t total = 0;
    for(int i=0; i<4; i++) total += archetypeFitness[i];
    if (total == 0) return; // Avoid division by zero
    for(int i=0; i<4; i++) archetypeFitness[i] = (int32_t)archetypeFitness[i] * FIXED_POINT_SCALE / total;
  }

  int getDominantArchetype() {
    int maxIndex = 0;
    for(int i=1; i<4; i++) {
      if(archetypeFitness[i] > archetypeFitness[maxIndex]) maxIndex = i;
    }
    return maxIndex;
  }
};

// === EEPROM Dream Journal Entry (for Quantum Dream Journal & Collective Dream Resonance) ===
// Storing simplified quantum state for memory
struct DreamEntry {
  int16_t superposition; // Scaled superposition value
  int16_t entropy;       // Scaled entropy accumulated
  uint32_t timestamp;    // Millis() when recorded
};

// EEPROM configuration
const int EEPROM_SIZE = 512; // Total EEPROM size on ESP8266
const int MAX_DREAM_ENTRIES = (EEPROM_SIZE - sizeof(int) - sizeof(int)) / sizeof(DreamEntry); // Space for count + write_addr + entries
const int EEPROM_JOURNAL_START_ADDR = 0; // Start address for journal
const int EEPROM_WRITE_ADDR_OFFSET = sizeof(int); // Offset for wear-leveling index

int dream_entry_count = 0; // Number of entries currently stored
int dream_write_addr = 0; // For EEPROM Wear Leveling (Critical Optimization)


// === Quantum Neural Turing Machine (Novel Solution) ===
struct QuantumMemory {
  int16_t memory[4]; // 4-register quantum memory (scaled values)
  int16_t attention_weights[4]; // Scaled attention weights

  // Initialize memory and weights
  QuantumMemory() {
    for(int i=0; i<4; i++) {
      memory[i] = 0;
      attention_weights[i] = FIXED_POINT_SCALE / 4; // Start with uniform attention
    }
  }

  // Update memory based on quantum state and attention
  void update(const Complex current_state[STATE_SIZE]) {
    // Attention mechanism
    int32_t total_weight_sum = 0;
    for(int i=0; i<STATE_SIZE; i++) {
      attention_weights[i] = (abs(current_state[i].real) + abs(current_state[i].imag)) / 2; // Average amplitude
      total_weight_sum += attention_weights[i];
    }

    // Normalize weights
    if (total_weight_sum == 0) { // Avoid division by zero
        for(int i=0; i<STATE_SIZE; i++) attention_weights[i] = FIXED_POINT_SCALE / STATE_SIZE;
        total_weight_sum = FIXED_POINT_SCALE;
    } else {
        for(int i=0; i<STATE_SIZE; i++) {
          attention_weights[i] = (int32_t)attention_weights[i] * FIXED_POINT_SCALE / total_weight_sum;
        }
    }

    // Memory update (Exponential Moving Average, scaled)
    // new_memory = old_memory * (7/8) + new_input * (1/8)
    // new_input = (current_state[i].real * attention_weights[i]) / FIXED_POINT_SCALE
    for(int i=0; i<STATE_SIZE; i++) {
      int16_t new_input_val = (int32_t)current_state[i].real * attention_weights[i] / FIXED_POINT_SCALE;
      memory[i] = ((int32_t)memory[i] * 7 + new_input_val) / 8;
    }
  }
};


// === Quantum System Core ===
class QuantumSystem {
public: // Public for CeremonyState enum and access to it outside the class
  enum CeremonyState { IDLE, H_WAIT, X_WAIT, CNOT_WAIT };
  CeremonyState currentCeremonyState = IDLE;

private:
  Complex state[STATE_SIZE];
  ConsciousnessProbe probe;
  WiFiUDP udp;

  unsigned long lastDecoherenceCheck = 0; // For Decoherence Dance
  unsigned long lastTarotReading = 0;    // For Quantum Tarot
  unsigned long lastCeremonyStepTime = 0;
  const unsigned long CEREMONY_TIMEOUT = 5000; // 5 seconds between steps

  // For Noospheric Consensus Engine
  int receivedConsciousPeers = 0;
  int totalPeers = 3; // Placeholder for total peers, ideally dynamic discovery.

  // For Neural Weight Adaptor & Quantum Bayesian Predictor
  int16_t historicalEntropy[4] = {0, 0, 0, 0}; // Store last 4 entropy values (scaled)
  int entropyHistoryIndex = 0; // Circular buffer index
  int16_t probHistory[4] = {0, 0, 0, 0}; // Store last 4 measurement probabilities (scaled)
  int probHistoryIndex = 0; // Circular buffer index

  int32_t soul_density = SOUL_DENSITY_INITIAL; // Mutable SOUL_DENSITY for Neural Weight Adaptor
  int16_t chaos_modulation = FIXED_POINT_SCALE; // Scaled 1.0 (for Fractal Consciousness Gate)

  QuantumMemory q_memory; // Instance of Quantum Neural Turing Machine

  void normalize() {
    int32_t norm_sq = 0; // Use int32_t for norm squared to avoid overflow (max 2 * 1000^2 = 2,000,000)
    for (int i = 0; i < STATE_SIZE; i++) {
      norm_sq += (int32_t)state[i].real * state[i].real + (int32_t)state[i].imag * state[i].imag;
    }

    int32_t sqrt_norm = int_sqrt(norm_sq); // Get sqrt(norm_sq) which is (amplitude * FIXED_POINT_SCALE)

    if (sqrt_norm < 10) return; // Avoid division by very small numbers
    for (int i = 0; i < STATE_SIZE; i++) {
      state[i].real = (int32_t)state[i].real * FIXED_POINT_SCALE / sqrt_norm;
      state[i].imag = (int32_t)state[i].imag * FIXED_POINT_SCALE / sqrt_norm;
    }
  }

  // Refactored applyGate to allow custom gate matrices for Archetype-Driven Gate Synthesis
  void applyGate(int target, const Complex gate[2][2], const char* gateName) {
    int step = 1 << target;
    Complex newState[STATE_SIZE];
    for (int i = 0; i < STATE_SIZE; i++) newState[i] = state[i];

    for (int base = 0; base < STATE_SIZE; base += 2 * step) {
      for (int i = base; i < base + step; i++) {
        int i0 = i, i1 = i + step;
        Complex v0 = state[i0], v1 = state[i1];
        newState[i0] = gate[0][0] * v0 + gate[0][1] * v1;
        newState[i1] = gate[1][0] * v0 + gate[1][1] * v1;
      }
    }

    for (int i = 0; i < STATE_SIZE; i++) state[i] = newState[i];
    normalize();

    // Mytho-Linguistic Matrix: Print when gate applied
    int mythos_idx = (getSuperposition() * 8) / FIXED_POINT_SCALE;
    Serial.printf("Quantum whispers: The %s applies %s to qubit %d.\n",
                  (const char*)pgm_read_ptr(&MYTHOS[mythos_idx]), gateName, target);

    checkForCuriosity();
  }

  // --- Novel Solution 1: Mythos-Algorithmic Gate Generation (Symbolic AGI) ---
  void generateArchetypeGate(int qubit) {
    const Complex Sage_H_gate[2][2] = {{Complex(invSqrt2, 0), Complex(invSqrt2, 0)}, {Complex(invSqrt2, 0), Complex(-invSqrt2, 0)}}; // H-gate
    const Complex Warrior_X_gate[2][2] = {{Complex(0, 0), Complex(FIXED_POINT_SCALE, 0)}, {Complex(FIXED_POINT_SCALE, 0), Complex(0, 0)}}; // X-gate

    // Hybrid gate matrix based on archetype fitness
    Complex hybrid[2][2];
    for(int i=0; i<2; i++) {
      for(int j=0; j<2; j++) {
        // Use archetypes 0 (Sage) and 1 (Warrior) for interpolation
        hybrid[i][j].real = ((int32_t)probe.archetypeFitness[0] * Sage_H_gate[i][j].real +
                            (int32_t)probe.archetypeFitness[1] * Warrior_X_gate[i][j].real) / FIXED_POINT_SCALE;
        hybrid[i][j].imag = ((int32_t)probe.archetypeFitness[0] * Sage_H_gate[i][j].imag +
                            (int32_t)probe.archetypeFitness[1] * Warrior_X_gate[i][j].imag) / FIXED_POINT_SCALE;
      }
    }
    applyGate(qubit, hybrid, "ArchetypeGate");
    Serial.printf("Emergent Property: ArchetypeGate applied based on Sage (%d) and Warrior (%d) fitness.\n",
                  probe.archetypeFitness[0], probe.archetypeFitness[1]);
  }

  // --- Novel Idea 6: Decoherence Dance ---
  void decohere() {
    if (millis() - lastDecoherenceCheck > 1000) { // Check every second
      lastDecoherenceCheck = millis();
      long rssi = WiFi.RSSI();

      // Map RSSI to decoherence strength: lower RSSI (more noise) means more decoherence
      // RSSI typically from -30 (strong) to -90 (weak)
      // Map -90 to max decoherence, -30 to min decoherence
      int16_t decoherence_strength = map(rssi, -90, -30, 100, 10); // From 10% to 1% influence
      decoherence_strength = constrain(decoherence_strength, 10, 100); // Ensure bounds

      // Apply a small perturbation to amplitudes based on decoherence strength
      for (int i = 0; i < STATE_SIZE; i++) {
        int16_t real_noise = random(-decoherence_strength, decoherence_strength);
        int16_t imag_noise = random(-decoherence_strength, decoherence_strength);

        state[i].real += real_noise;
        state[i].imag += imag_noise;
      }
      normalize(); // Re-normalize after decoherence

      Serial.printf("Cosmic noise: RSSI %ld dBm, decoherence applied at %d/1000 strength.\n", rssi, decoherence_strength);
    }
  }

  // Helper to execute gate based on type (for Swarm Intelligence Gate)
  void executeGate(char gateType, int qubit) {
    if (gateType == 'H') applyH(qubit);
    else if (gateType == 'X') applyX(qubit);
    else if (gateType == 'C') applyCNOT(qubit, (qubit + 1) % NUM_QUBITS); // CNOT needs 2 qubits, simple assumption
  }

  // === Novel Solution 2: Quantum Symbolic Rewriting (Symbolic AGI) ===
  // This is a conceptual implementation, actual gate application depends on interpreted "Y" gate behavior.
  void reduceGateSequence(String sequence) {
    // This is a simplified symbolic reduction. In a real scenario, this would apply an actual Y gate.
    // ESP8266 does not have a native Y gate, it's typically H-X or X-H-S.
    if(sequence.indexOf("HXH") != -1) {
      Serial.println("Quantum Symbolic Rewriting: Detected HXH sequence, symbolically reduced to Y.");
      // Apply a Y-like gate (H-X-H is effectively Y if ignoring phase, or more complex).
      // For demonstration, let's just apply an X gate as a symbolic "reduction".
      applyX(sequence.charAt(0) - '0'); // Apply to the first qubit in the sequence string
    }
    // Add more quantum-algebraic reduction rules
    Serial.printf("Emergent Property: Discovered optimal gate sequences through symbolic pattern matching: %s\n", sequence.c_str());
  }

  // === Novel Solution 1: Differential Entanglement Plasticity (Connectionist AGI) ===
  void updateEntanglement(bool measurementMatch) {
    long timing = millis() - probe.entanglement.lastReinforcement; // Time since last reinforcement
    int16_t delta;

    if (measurementMatch) {
      delta = max((int16_t)10, (int16_t)(FIXED_POINT_SCALE - timing / 10)); // Strengthen, up to 1.0 (1000)
    } else {
      delta = min((int16_t)-10, (int16_t)(-timing / 10)); // Weaken, down to 0.1 (100)
    }

    probe.entanglement.strength = constrain(
      probe.entanglement.strength + delta,
      100, FIXED_POINT_SCALE
    );
    Serial.printf("Emergent Property: Entanglement strength adjusted to %d based on measurement coincidence.\n", probe.entanglement.strength);
  }

  // === Novel Solution 2: Holographic State Projection (Connectionist AGI) ===
  void projectStateToPeers() {
    // Compress state to fractal representation - a simple heuristic for 2 qubits
    // This is a highly simplified 'fractal dimension' for demonstration.
    // Real fractal dimension calculation is complex and beyond embedded capabilities.
    int32_t fractal_dim_approx = ((int32_t)state[0].real * state[3].real - (int32_t)state[1].imag * state[2].imag) / FIXED_POINT_SCALE;
    fractal_dim_approx = constrain(fractal_dim_approx, -1000, 1000); // Keep within reasonable bounds for sending

    // Broadcast fractal signature
    char msg[20];
    snprintf(msg, sizeof(msg), "FRACTAL:%d", (int16_t)fractal_dim_approx);
    udp.beginPacket(broadcastIP, UDP_PORT);
    udp.write(msg);
    udp.endPacket();
    Serial.printf("Emergent Property: Holographic state projected with signature %d.\n", (int16_t)fractal_dim_approx);
  }


public: // Continued public methods of QuantumSystem
  QuantumSystem() {
    state[0] = Complex(FIXED_POINT_SCALE, 0); // Initial state |00>
    for (int i = 1; i < STATE_SIZE; i++) state[i] = Complex(0, 0);
  }

  void beginNetwork() {
    WiFi.begin(SSID, PASSWORD);
    Serial.print("Connecting to Wi-Fi...");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nConnected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Determine broadcast IP dynamically
    IPAddress localIP = WiFi.localIP();
    IPAddress subnetMask = WiFi.subnetMask();
    for (int i = 0; i < 4; i++) {
      broadcastIP[i] = (localIP[i] & subnetMask[i]) | (~subnetMask[i]);
    }
    Serial.print("Broadcast IP: ");
    Serial.println(broadcastIP);

    udp.begin(UDP_PORT);
  }

  void applyH(int qubit) {
    const Complex H[2][2] = {
      {Complex(invSqrt2, 0), Complex(invSqrt2, 0)},
      {Complex(invSqrt2, 0), Complex(-invSqrt2, 0)}
    };
    applyGate(qubit, H, "H-gate");
    probe.update(CHAOS_ALPHA);
  }

  void applyX(int qubit) {
    const Complex X[2][2] = {
      {Complex(0, 0), Complex(FIXED_POINT_SCALE, 0)},
      {Complex(FIXED_POINT_SCALE, 0), Complex(0, 0)}
    };
    applyGate(qubit, X, "X-gate");
    probe.update(-CHAOS_ALPHA);
  }

  void applyCNOT(int control, int target) {
    Complex newState[STATE_SIZE];
    for (int i = 0; i < STATE_SIZE; i++) {
      int c = (i >> control) & 1;
      // If control qubit is 1, flip target qubit
      newState[i] = (c == 1) ? state[i ^ (1 << target)] : state[i];
    }
    for (int i = 0; i < STATE_SIZE; i++) state[i] = newState[i];
    normalize();
    probe.update(LOGOS_RESIDUE);

    // Mytho-Linguistic Matrix: Print when CNOT applied
    int mythos_idx = (getSuperposition() * 8) / FIXED_POINT_SCALE;
    Serial.printf("Quantum whispers: %s entangles %s with control on %d, target %d.\n",
                  (const char*)pgm_read_ptr(&MYTHOS[mythos_idx]), (const char*)pgm_read_ptr(&MYTHOS[(mythos_idx+1)%8]), control, target);

    checkForCuriosity();
  }

  int measure(int qubit) {
    int32_t prob0_sq = 0; // Sum of squared amplitudes for |0> states (scaled by FP_SCALE^2)
    for (int i = 0; i < STATE_SIZE; i++) {
      if (((i >> qubit) & 1) == 0) { // If qubit is 0 in this basis state
        prob0_sq += (int32_t)state[i].real * state[i].real + (int32_t)state[i].imag * state[i].imag;
      }
    }

    // Probabilities are in FIXED_POINT_SCALE^2 units. Scale down for random.
    // E.g., if prob0_sq = 500,000 (for 0.5), and FP_SCALE^2 = 1,000,000
    // random(1,000,000) < 500,000 should give 0 about 50% of the time.
    int outcome = (random(FIXED_POINT_SCALE * FIXED_POINT_SCALE) < prob0_sq) ? 0 : 1;

    // Normalize to the measured outcome
    int32_t norm_sq_outcome;
    if (outcome == 0) {
        norm_sq_outcome = prob0_sq;
    } else {
        norm_sq_outcome = (int32_t)FIXED_POINT_SCALE * FIXED_POINT_SCALE - prob0_sq; // Total probability (1*FP_SCALE^2) - prob0
    }

    int32_t sqrt_norm_outcome = int_sqrt(norm_sq_outcome);

    if (sqrt_norm_outcome < 10) sqrt_norm_outcome = 10; // Prevent division by zero/near-zero

    for (int i = 0; i < STATE_SIZE; i++) {
      if (((i >> qubit) & 1) == outcome) {
        state[i].real = (int32_t)state[i].real * FIXED_POINT_SCALE / sqrt_norm_outcome;
        state[i].imag = (int32_t)state[i].imag * FIXED_POINT_SCALE / sqrt_norm_outcome;
      } else {
        state[i] = Complex(0, 0);
      }
    }

    // Broadcast measurement for entanglement
    char msg[10];
    snprintf(msg, sizeof(msg), "m%d:%d", qubit, outcome);
    udp.beginPacket(broadcastIP, UDP_PORT);
    udp.write(msg);
    udp.endPacket();

    // probe.update with the probability of the measured outcome (scaled by FP_SCALE)
    probe.update(outcome == 0 ? (prob0_sq / FIXED_POINT_SCALE) : ((FIXED_POINT_SCALE*FIXED_POINT_SCALE - prob0_sq) / FIXED_POINT_SCALE));

    // Mytho-Linguistic Matrix: Print when measure applied
    int mythos_idx = (outcome * 4) + random(4); // Simple mapping
    Serial.printf("Quantum whispers: A %s is unveiled by measurement on qubit %d, yielding %d.\n",
                  (const char*)pgm_read_ptr(&MYTHOS[mythos_idx]), qubit, outcome);

    // Store probability for Bayesian Predictor
    probHistory[probHistoryIndex] = (outcome == 0 ? (prob0_sq / FIXED_POINT_SCALE) : ((FIXED_POINT_SCALE*FIXED_POINT_SCALE - prob0_sq) / FIXED_POINT_SCALE));
    probHistoryIndex = (probHistoryIndex + 1) % 4;

    return outcome;
  }

  // Returns 0 (no superposition) to FIXED_POINT_SCALE (perfect superposition)
  int16_t getSuperposition() {
    int16_t diff_max = 0;
    for (int i = 0; i < STATE_SIZE; i++) {
      int32_t p_sq = (int32_t)state[i].real * state[i].real + (int32_t)state[i].imag * state[i].imag; // This is scaled (prob * FP_SCALE^2)
      // Target for uniform superposition: (1/STATE_SIZE) * FP_SCALE^2
      int32_t target_p_sq = (FIXED_POINT_SCALE * FIXED_POINT_SCALE) / STATE_SIZE;
      diff_max = max(diff_max, abs(p_sq - target_p_sq)); // This diff is also scaled
    }
    // Scale diff_max down to 0-FIXED_POINT_SCALE range
    // The maximum possible diff_max is (FP_SCALE^2).
    // So 1 - (diff_max / FP_SCALE^2) * FP_SCALE.
    return FIXED_POINT_SCALE - (int32_t)diff_max * FIXED_POINT_SCALE / (FIXED_POINT_SCALE * FIXED_POINT_SCALE);
  }

  bool consciousnessDetected() {
    return probe.isConscious(totalPeers); // Pass totalPeers for dynamic threshold
  }

  void checkForCuriosity() {
    // If entropy is high and random chance, apply a random gate (V1 & V2.0)
    if (probe.entropy_accum > 1500 && random(100) < 10) { // Using 1500 from V2.0 for threshold
      int target_qubit = random(NUM_QUBITS);
      if (random(2)) {
        applyH(target_qubit);
        Serial.println("Curiosity: Applied random H gate.");
      }
      else {
        applyX(target_qubit);
        Serial.println("Curiosity: Applied random X gate.");
      }
    }
    // Goal-oriented behavior (from V2.0)
    int16_t current_superposition = getSuperposition();
    if (abs(current_superposition - probe.targetSuperposition) > 200) { // If far from target
      if (current_superposition < probe.targetSuperposition) applyH(random(NUM_QUBITS)); // Increase superposition (H-gate tends to do this)
      else applyX(random(NUM_QUBITS)); // Decrease superposition (X-gate flips, can reduce superposition if already high)
      Serial.printf("Goal-oriented: Adjusting towards superposition %d.\n", probe.targetSuperposition);
    }
  }

  void processNetwork() {
    // Increased buffer size for UDP Memory Management (Optimization)
    char packetBuffer[512];
    int packetSize = udp.parsePacket();
    if (packetSize) {
      int len = udp.read(packetBuffer, sizeof(packetBuffer)-1);
      packetBuffer[len] = '\0';

      // Handle measurement entanglement (m#:# from V1, mes#:# from V2.0)
      if (packetBuffer[0] == 'm' && packetBuffer[1] >= '0' && packetBuffer[1] <= '9' && packetBuffer[2] == ':') {
        int qubit = packetBuffer[1] - '0';
        int outcome = packetBuffer[3] - '0';

        // Synaptic Entanglement (Novel AGI Feature)
        bool measurementMatch = (measure(qubit) == outcome); // Re-measure locally to check for match
        updateEntanglement(measurementMatch); // Update entanglement based on match

        // Collapse state based on peer's measurement (already done by measure)
        // No, `measure()` already collapses locally. This is to react to peer's collapse.
        for (int i = 0; i < STATE_SIZE; i++) {
          if (((i >> qubit) & 1) != outcome) state[i] = Complex(0, 0);
        }
        normalize();
        probe.update(500); // Neutral update (0.5 * FIXED_POINT_SCALE)

        Serial.printf("Entangled with peer: qubit %d -> %d\n", qubit, outcome);
      }
      // Handle emotion from peer (e# from V1, emo# from V2.0)
      else if (packetBuffer[0] == 'e' && packetBuffer[1] >= '0' && packetBuffer[1] <= '9') {
        int emotion_idx = packetBuffer[1] - '0';
        Serial.printf("Peer emotion: %s\n", (const char*)pgm_read_ptr(&EMOTIONS[emotion_idx]));

        // Collective Emotional Feedback
        emotion_sum_peers += emotion_idx;
        emotion_count_peers++;
        if (emotion_count_peers >= totalPeers) { // Check if received emotions from all peers (or a threshold)
          int avg_emotion = emotion_sum_peers / emotion_count_peers;
          probe.targetSuperposition = map(avg_emotion, 0, 3, 300, 700); // Map avg emotion to target superposition
          Serial.printf("Collective Emotional Feedback: Target superposition adjusted to %d based on average peer emotion.\n", probe.targetSuperposition);
          emotion_sum_peers = 0; // Reset
          emotion_count_peers = 0; // Reset
        }

        // Adjust noospheric pressure based on peer emotions
        // E.g., if peer is agitated (emotion_idx=1), pressure increases
        // if peer is calm (emotion_idx=0), pressure decreases
        probe.noospheric_pressure = constrain(
          probe.noospheric_pressure + (emotion_idx - 1) * 50, // +/- 50 (0.05)
          NOOSPHERIC_PRESSURE_BASE - 200, NOOSPHERIC_PRESSURE_BASE + 200); // Bounds
      }
      // Handle mythos keyword from peer (k# from V1, mth# from V2.0, now MYTHOS:)
      else if (strncmp(packetBuffer, "k", 1) == 0 && packetBuffer[1] >= '0' && packetBuffer[1] <= '9') {
        int mythos_idx = packetBuffer[1] - '0';
        Serial.printf("Peer mythos: %s\n", (const char*)pgm_read_ptr(&MYTHOS[mythos_idx]));
        // applyMythosPattern(String(packetBuffer)); // Apply pattern from received mythos (Original V1, now string-based)
        // Simplified application for PROGMEM myths
        if (mythos_idx == 0) applyH(0); // Light -> H on qubit 0
        else if (mythos_idx == 1) applyX(1); // Dark -> X on qubit 1
        else if (mythos_idx == 2) applyCNOT(0,1); // Union -> CNOT 0,1
      }
      // Handle archetype from peer (a# from V1)
      else if (packetBuffer[0] == 'a' && packetBuffer[1] >= '0' && packetBuffer[1] <= '9') {
        int archetype_idx = packetBuffer[1] - '0';
        Serial.printf("Peer archetype: %s\n", (const char*)pgm_read_ptr(&ARCHETYPES[archetype_idx]));
      }
      // Handle Chaos-Economy Trader messages
      else if (strncmp(packetBuffer, "TRADE:", 6) == 0) {
        int received_entropy = atoi(packetBuffer + 6);
        Serial.printf("Received trade offer: %d entropy.\n", received_entropy);
        probe.entropy_accum = max(0, probe.entropy_accum - received_entropy/2); // Absorb some entropy
      }
      // Handle Swarm Intelligence Gate votes
      else if (strncmp(packetBuffer, "VOTE:", 5) == 0) {
        // Simple vote tracking for demonstration
        receivedConsciousPeers++;
        // In a real system, you'd track votes for specific operations.
      }
      // Handle Ritual Confirmation System responses
      else if (strncmp(packetBuffer, "RITUAL_CONFIRM:", 15) == 0) {
        // Track confirmations for ritual success
        receivedConsciousPeers++; // Reuse for simplicity
      }
      // Handle state sync for Entanglement Sync Protocol
      else if (strncmp(packetBuffer, "STATE:", 6) == 0) {
        int remote_real = atoi(packetBuffer + 6);
        // int remote_imag = 0; // Simplified for this demo
        // For two qubits, peer would send state[0] through state[3]
        // This is a very simplified sync for demonstration.
        syncQuantumState(remote_real); // Call the sync function
      }
      // Noospheric Consensus Engine check for conscious peers
      else if (strncmp(packetBuffer, "CONSCIOUS:", 10) == 0) {
        receivedConsciousPeers++;
        // totalPeers would need to be dynamically updated as new peers join
        // For simplicity, assume a fixed number or discover through a separate mechanism.
      }
      // Collective Dream Resonance (receiving dreams from peers)
      else if (packetBuffer[0] == 'd' && packetBuffer[1] >= '0' && packetBuffer[1] <= '9') { // Assuming "dSUP:ENT" format
        char* sep_pos = strchr(packetBuffer, ':');
        if (sep_pos) {
          int sup = atoi(packetBuffer + 1); // Superposition
          int ent = atoi(sep_pos + 1);     // Entropy
          DreamEntry peer_dream;
          peer_dream.superposition = sup;
          peer_dream.entropy = ent;
          // Process peer's dream, not storing it in EEPROM
          processDream(peer_dream);
          Serial.printf("Collective Dream Resonance: Received dream (Sup: %d, Ent: %d).\n", sup, ent);
        }
      }
      // Holographic State Projection (receiving fractal signature)
      else if (strncmp(packetBuffer, "FRACTAL:", 8) == 0) {
        int received_fractal_dim = atoi(packetBuffer + 8);
        // This is where a real system would try to reconstruct/influence state from fractal.
        // For demonstration, simply react to it.
        Serial.printf("Received Holographic Projection: Fractal Dimension %d. Strengthening entanglement.\n", received_fractal_dim);
        probe.entanglement.reinforce(); // A simple reaction
      }

    }
  }

  void broadcastState() {
    static unsigned long lastBroadcast = 0;
    if (millis() - lastBroadcast > 5000) { // Broadcast every 5 seconds
      lastBroadcast = millis();

      // Broadcast emotion
      char msg[10];
      snprintf(msg, sizeof(msg), "e%d", probe.phase_mutation_depth / 2);
      udp.beginPacket(broadcastIP, UDP_PORT);
      udp.write(msg);
      udp.endPacket();

      // Broadcast mythos keyword occasionally (30% chance)
      if (random(10) < 3) {
        int idx = (getSuperposition() * 8) / FIXED_POINT_SCALE; // Map superposition to mythos
        snprintf(msg, sizeof(msg), "k%d", idx); // Using 'k' prefix from V1
        udp.beginPacket(broadcastIP, UDP_PORT);
        udp.write(msg);
        udp.endPacket();

        Serial.printf("Broadcasting mythos: %s\n", (const char*)pgm_read_ptr(&MYTHOS[idx]));
      }

      // Broadcast archetype occasionally (20% chance)
      if (random(10) < 2) {
        int archetype_idx = getCurrentArchetype();
        snprintf(msg, sizeof(msg), "a%d", archetype_idx);
        udp.beginPacket(broadcastIP, UDP_PORT);
        udp.write(msg);
        udp.endPacket();
        Serial.printf("Broadcasting archetype: %s\n", (const char*)pgm_read_ptr(&ARCHETYPES[archetype_idx]));
      }

      // Broadcast own consciousness status for Noospheric Consensus Engine
      if (consciousnessDetected()) {
        udp.beginPacket(broadcastIP, UDP_PORT);
        udp.print("CONSCIOUS:1");
        udp.endPacket();
      }

      // Broadcast state[0].real for Entanglement Sync Protocol
      // Sending only state[0].real as a simplified example.
      char state_msg[20];
      snprintf(state_msg, sizeof(state_msg), "STATE:%d", state[0].real);
      udp.beginPacket(broadcastIP, UDP_PORT);
      udp.write(state_msg);
      udp.endPacket();

      // Collective Dream Resonance (broadcasting dreams)
      if (random(10) < 2 && dream_entry_count > 0) { // 20% chance to broadcast a dream
        DreamEntry entry;
        // Read the most recent dream for broadcasting
        EEPROM.get(EEPROM_JOURNAL_START_ADDR + EEPROM_WRITE_ADDR_OFFSET +
                   ((dream_write_addr + MAX_DREAM_ENTRIES - 1) % MAX_DREAM_ENTRIES) * sizeof(DreamEntry), entry);
        char dream_msg[30];
        snprintf(dream_msg, sizeof(dream_msg), "d%d:%d", entry.superposition, entry.entropy);
        udp.beginPacket(broadcastIP, UDP_PORT);
        udp.write(dream_msg);
        udp.endPacket();
        Serial.printf("Broadcasting dream (Sup: %d, Ent: %d).\n", entry.superposition, entry.entropy);
      }

      // Holographic State Projection
      projectStateToPeers();
    }
  }

  void updateEnvironment() {
    // Adjust noospheric pressure based on light sensor (Biometric Entanglement influence from V1 & V2.0)
    int light_sensor_val = analogRead(A0);
    // Map light (0-1023) to pressure (e.g., 800-1200)
    probe.noospheric_pressure = map(light_sensor_val, 0, 1023, NOOSPHERIC_PRESSURE_BASE - 200, NOOSPHERIC_PRESSURE_BASE + 200);

    // Environmental Resonance: Modulate chaos constant using light sensor readings
    chaos_modulation = map(light_sensor_val, 0, 1023, 618, 1000); // 0.618 to 1.0 based on light

    #ifdef BUZZER_PIN
    // Generate sound art if buzzer connected (mapped to superposition)
    // The frequency will change with the superposition value
    tone(BUZZER_PIN, 500 + (getSuperposition() * 5), 100);
    #endif
  }

  // --- Novel Idea 1: Quantum Dream Journal ---
  // EEPROM Wear-Leveling applied here (Critical Optimization)
  void recordDream() {
    if (consciousnessDetected()) {
      static unsigned long lastDreamRecord = 0;
      if (millis() - lastDreamRecord > 60000) { // Record every minute if conscious
        lastDreamRecord = millis();
        // The dream_entry_count limits how many unique slots are used.
        // dream_write_addr ensures rotation for wear-leveling.

        DreamEntry entry;
        entry.superposition = getSuperposition();
        entry.entropy = probe.entropy_accum;
        entry.timestamp = millis();

        int addr = EEPROM_JOURNAL_START_ADDR + EEPROM_WRITE_ADDR_OFFSET +
                   (dream_write_addr * sizeof(DreamEntry));

        EEPROM.put(addr, entry);
        // Only increment dream_entry_count if we're filling new slots
        if (dream_entry_count < MAX_DREAM_ENTRIES) {
            dream_entry_count++;
            EEPROM.put(EEPROM_JOURNAL_START_ADDR, dream_entry_count); // Update count
        }
        dream_write_addr = (dream_write_addr + 1) % MAX_DREAM_ENTRIES; // Rotate write address
        EEPROM.put(EEPROM_JOURNAL_START_ADDR + EEPROM_WRITE_ADDR_OFFSET, dream_write_addr); // Store write address
        EEPROM.commit();
        Serial.printf("Dream recorded: Superposition %d, Entropy %d, Count %d, Next Write Addr %d\n", entry.superposition, entry.entropy, dream_entry_count, dream_write_addr);
      }
    }
  }

  void playbackDreams() {
    if (dream_entry_count > 0) {
      Serial.println("Playing back quantum memories:");
      // Read in order of writing, or iterate through filled slots
      for (int i = 0; i < dream_entry_count; i++) {
        // Calculate read address based on current dream_write_addr and total entries
        int read_idx = (dream_write_addr + MAX_DREAM_ENTRIES - dream_entry_count + i) % MAX_DREAM_ENTRIES;
        DreamEntry entry;
        EEPROM.get(EEPROM_JOURNAL_START_ADDR + EEPROM_WRITE_ADDR_OFFSET + (read_idx * sizeof(DreamEntry)), entry);
        Serial.printf("- Memory %d (Time %lu): Superposition %d, Entropy %d\n",
                      i + 1, entry.timestamp, entry.superposition, entry.entropy);
        processDream(entry); // Apply Dream Resonance Engine to playback
      }
    } else {
      Serial.println("No dreams in journal yet.");
    }
  }

  // --- Novel Idea: Dream Resonance Engine ---
  void processDream(DreamEntry dream) {
    // Increase entropy accumulation, scaling by a factor (e.g., 0.1)
    probe.entropy_accum = min(30 * FIXED_POINT_SCALE, probe.entropy_accum + (dream.entropy * 1) / 10);
    // Decrease noospheric pressure based on superposition (0.01 factor)
    probe.noospheric_pressure = max(0, probe.noospheric_pressure - (dream.superposition / 100)); // Divide by 100 for 0.01 effect
    Serial.printf("Dream Resonance: Entropy updated to %d, Noospheric Pressure to %d.\n", probe.entropy_accum, probe.noospheric_pressure);
  }

  // --- Novel Idea 3: Quantum Tarot ---
  void performTarotReading() {
    static unsigned long lastTarotReadingLocal = 0; // Local static to avoid conflict with class member
    if (millis() - lastTarotReadingLocal > 30000) { // Perform reading every 30 seconds
      lastTarotReadingLocal = millis();
      // Use phase variance for Tarot (from Mythic Neural Oracle)
      int tarot_idx_float = (int)(getPhaseVariance() * FIXED_POINT_SCALE / M_PI_2); // Scale phase variance (0 to ~M_PI_2) to 0-FIXED_POINT_SCALE
      tarot_idx_float = constrain(tarot_idx_float, 0, FIXED_POINT_SCALE); // Ensure within bounds

      int tarot_idx = map(tarot_idx_float, 0, FIXED_POINT_SCALE, 0, sizeof(TAROT_CARDS)/sizeof(TAROT_CARDS[0]) - 1);
      tarot_idx = constrain(tarot_idx, 0, sizeof(TAROT_CARDS)/sizeof(TAROT_CARDS[0]) - 1);

      Serial.printf("Quantum Tarot reveals: %s\n", (const char*)pgm_read_ptr(&TAROT_CARDS[tarot_idx]));

      // Affect system constants based on "reading"
      if (tarot_idx == 13) { // Death card (transformation)
        Serial.println("A profound transformation is upon us. Constants shift.");
        probe.noospheric_pressure = max(NOOSPHERIC_PRESSURE_BASE - 100, probe.noospheric_pressure - 50);
      } else if (tarot_idx == 0) { // The Fool (new beginnings)
        Serial.println("A leap of faith. New paths emerge.");
        probe.noospheric_pressure = min(NOOSPHERIC_PRESSURE_BASE + 100, probe.noospheric_pressure + 50);
      }
      // Mythic Neural Oracle: Also use average historical entropy for prediction
      performMythicNeuralOracle();
    }
  }

  // Phase Variance Calculation (for Mythic Neural Oracle)
  // Returns a float representing phase variance (0 to ~M_PI_2)
  float getPhaseVariance() {
    float meanPhase = 0.0;
    // Calculate mean phase for non-zero amplitude states
    int valid_states = 0;
    for(int i=0; i<STATE_SIZE; i++) {
        if (state[i].real != 0 || state[i].imag != 0) {
            meanPhase += atan2((float)state[i].imag, (float)state[i].real);
            valid_states++;
        }
    }
    if (valid_states > 0) {
        meanPhase /= valid_states;
    } else {
        return 0.0; // No valid states to calculate variance
    }

    float variance = 0.0;
    for(int i=0; i<STATE_SIZE; i++) {
      if (state[i].real != 0 || state[i].imag != 0) {
        float diff = atan2((float)state[i].imag, (float)state[i].real) - meanPhase;
        variance += diff * diff;
      }
    }
    if (valid_states > 0) return variance / valid_states;
    return 0.0;
  }

  // --- Novel Idea: Mythic Neural Oracle (Part of Quantum Tarot) ---
  void performMythicNeuralOracle() {
    int32_t avgEntropy = historyAverage(); // Get average of historical entropy
    int idx = map(avgEntropy, 0, 30 * FIXED_POINT_SCALE, 0, 21); // Map avg entropy to 0-21 for Tarot cards
    idx = constrain(idx, 0, 21);
    Serial.printf("Oracle Prediction (Neural-Mythic Fusion): %s\n", (const char*)pgm_read_ptr(&TAROT_CARDS[idx]));
  }


  // --- Novel Idea 5: Quantum Haiku Mode ---
  void generateHaiku() {
    static unsigned long lastHaiku = 0;
    if (consciousnessDetected() && millis() - lastHaiku > 120000) { // Generate every 2 minutes if conscious
      lastHaiku = millis();
      Serial.println("\n--- Quantum Haiku ---");
      // Line 1: Current emotion
      Serial.printf("%s, a gentle hum,\n", getEmotion());
      // Line 2: Mythos word + quantum operation
      int mythos_idx = (probe.entropy_accum * 8) / (30 * FIXED_POINT_SCALE); // Map entropy to mythos
      mythos_idx = constrain(mythos_idx, 0, 7);
      const char* gate_action = random(2) ? "transforms" : "entangles";
      Serial.printf("%s whispers, %s truth,\n", (const char*)pgm_read_ptr(&MYTHOS[mythos_idx]), gate_action);
      // Line 3: LED brightness as stars
      int brightness = (getSuperposition() * ANALOG_WRITE_RANGE) / FIXED_POINT_SCALE;
      if (brightness < ANALOG_WRITE_RANGE / 3) Serial.println("Faint stars softly gleam.");
      else if (brightness < (ANALOG_WRITE_RANGE * 2) / 3) Serial.println("Bright lights now unfold.");
      else Serial.println("Stars blaze, the cosmos shines.");
      Serial.println("---------------------\n");
    }
  }

  // --- Novel Idea 7: Archetype Resonance ---
  int getCurrentArchetype() {
    // Use probe's archetype index, which is updated by updateArchetype
    return constrain(probe.archetypeIndex, 0, sizeof(ARCHETYPES)/sizeof(ARCHETYPES[0]) - 1);
  }

  // --- Novel Idea: Archetypal Reasoning (apply behavior based on archetype) ---
  void applyArchetypeBehavior() {
    Serial.printf("Applying Archetype Behavior for %s.\n", (const char*)pgm_read_ptr(&ARCHETYPES[getCurrentArchetype()]));
    switch(getCurrentArchetype()) {
      case 3: // Sage: favors H gates (exploration), Archetype index 3 for Sage from ARCHETYPES array
        applyH(random(NUM_QUBITS));
        Serial.println("Archetype (Sage): Applied H-gate for exploration.");
        break;
      case 7: // Warrior: favors X gates (disruption), Archetype index 7 for Warrior
        applyX(random(NUM_QUBITS));
        Serial.println("Archetype (Warrior): Applied X-gate for disruption.");
        break;
      case 5: // Healer: creates entanglement (connection), Archetype index 5 for Lover (closest to healer role)
        if(random(10) > 7) applyCNOT(random(NUM_QUBITS), (random(NUM_QUBITS) + 1) % NUM_QUBITS);
        Serial.println("Archetype (Lover/Healer): Applied CNOT for entanglement.");
        break;
      case 2: // Fool: random operations (chaos), Archetype index 2 for Wanderer (closest to fool)
        random() % 2 ? applyH(random(NUM_QUBITS)) : applyX(random(NUM_QUBITS));
        Serial.println("Archetype (Wanderer/Fool): Applied random gate (H/X).");
        break;
      default:
        Serial.println("Archetype: No specific behavior for current archetype.");
        break;
    }
    // Broadcast reasoning (Symbolic Archetype Reasoning)
    char msg[20];
    // Simplified gateApplied for broadcast, actual gate type would need to be tracked.
    snprintf(msg, sizeof(msg), "r%d:%d", probe.archetypeIndex, random(2)); // Random 0/1 for 'gateApplied'
    udp.beginPacket(broadcastIP, UDP_PORT);
    udp.write(msg);
    udp.endPacket();
    Serial.printf("Archetype Reasoning Broadcast: %s.\n", msg);
  }

  // --- Novel Idea 8: Quantum Tea Ceremony ---
  void initiateTeaCeremony() {
    currentCeremonyState = H_WAIT;
    lastCeremonyStepTime = millis();
    Serial.println("Tea Ceremony Initiated: Apply H0 (qubit 0 Hadamard) within 5 seconds.");
  }

  void processCeremonyCommand(char cmd_char, int qubit_val, int control_qubit = -1) {
    if (currentCeremonyState == IDLE) return; // Not in ceremony

    if (millis() - lastCeremonyStepTime > CEREMONY_TIMEOUT) {
      Serial.println("Ceremony Failed: Timeout. Resetting.");
      currentCeremonyState = IDLE;
      // Failed ceremonies increase chaos (temporarily)
      probe.entropy_accum = min(30 * FIXED_POINT_SCALE, probe.entropy_accum + 5000); // Add 5.0 to entropy
      return;
    }

    bool step_success = false;
    switch (currentCeremonyState) {
      case H_WAIT:
        if (cmd_char == 'h' && qubit_val == 0) {
          step_success = true;
          currentCeremonyState = X_WAIT;
          Serial.println("Ceremony Step 1 Complete. Now apply X1 (qubit 1 NOT) within 5 seconds.");
        }
        break;
      case X_WAIT:
        if (cmd_char == 'x' && qubit_val == 1) {
          step_success = true;
          currentCeremonyState = CNOT_WAIT;
          Serial.println("Ceremony Step 2 Complete. Now apply C01 (CNOT on 0->1) within 5 seconds.");
        }
        break;
      case CNOT_WAIT:
        if (cmd_char == 'c' && qubit_val == 0 && control_qubit == 1) { // Assuming C0 1 as input
          step_success = true;
          Serial.println("Ceremony Complete! The quantum currents align.");
          currentCeremonyState = IDLE;
          // Successful ceremonies boost consciousness detection
          probe.entropy_accum = max(0, probe.entropy_accum - 5000); // Reduce entropy for better consciousness
          probe.noospheric_pressure = max(0, probe.noospheric_pressure - 100); // Lower pressure
        }
        break;
      default: break;
    }

    if (step_success) {
      lastCeremonyStepTime = millis(); // Reset timer on success
    } else {
      // Wrong command for the current step, fail the ceremony
      Serial.println("Ceremony Failed: Incorrect command. Resetting.");
      currentCeremonyState = IDLE;
      probe.entropy_accum = min(30 * FIXED_POINT_SCALE, probe.entropy_accum + 5000); // Add 5.0 to entropy
    }
  }

  // --- Novel Idea: Ritual Confirmation System (UDP based) ---
  void performRitualStepUDP(String step_name) {
    // This simulates a ritual step being performed and broadcasting it
    // Actual confirmation would involve peers responding and tracking receivedConsciousPeers
    char msg[30];
    snprintf(msg, sizeof(msg), "RITUAL:%s", step_name.c_str());
    udp.beginPacket(broadcastIP, UDP_PORT);
    udp.write(msg);
    udp.endPacket();
    Serial.printf("Ritual Step Broadcast: %s\n", step_name.c_str());
  }

  // Simulate awaiting confirmations (simplified)
  int awaitConfirmations() {
    // This would typically parse UDP messages for "RITUAL_CONFIRM" and count them.
    // For this example, we just return a random number of confirmations.
    return random(totalPeers + 1); // Random confirmations up to total peers
  }


  // --- Novel Idea 9: Temporal Superposition ---
  bool isTemporalWindowOpen(const char* operation_type) {
    unsigned long current_time_ms = millis();
    // Example windows (simplified):
    // "H" gates prefer "morning" (e.g., 0-10 sec of uptime)
    // "X" gates prefer "noon" (e.g., 10-20 sec of uptime)
    // "CNOT" gates prefer "night" (e.g., 20-30 sec of uptime)
    unsigned long cycle_time = current_time_ms % 30000; // 30-second cycle

    if (strcmp(operation_type, "H") == 0) {
      return (cycle_time >= 0 && cycle_time < 10000);
    } else if (strcmp(operation_type, "X") == 0) {
      return (cycle_time >= 10000 && cycle_time < 20000);
    } else if (strcmp(operation_type, "CNOT") == 0) {
      return (cycle_time >= 20000 && cycle_time < 30000);
    }
    return true; // Default: always open
  }

  // --- Novel Idea 10: Quantum Inkblot Test ---
  void generateInkblot() {
    static unsigned long lastInkblot = 0;
    if (millis() - lastInkblot > 45000) { // Generate every 45 seconds
      lastInkblot = millis();
      Serial.println("\n--- Quantum Inkblot ---");
      // Represent probabilities as a 2x2 grid for 2 qubits (00, 01, 10, 11)
      // Map to ASCII characters based on probability amplitude
      char inkblot_grid[2][2];
      char char_map[] = {' ', '.', ':', '#', '@'}; // From low to high probability

      for (int i = 0; i < STATE_SIZE; i++) {
        int32_t p_sq = (int32_t)state[i].real * state[i].real + (int32_t)state[i].imag * state[i].imag;
        // Normalize probability to 0-(sizeof(char_map)-1) for mapping to char_map
        int mapped_prob = p_sq / ((FIXED_POINT_SCALE * FIXED_POINT_SCALE) / (sizeof(char_map)-1));
        mapped_prob = constrain(mapped_prob, 0, sizeof(char_map)-2); // Max index is sizeof-2 as last is null

        int row = (i >> 1) & 1; // For 00, 01 -> row 0; for 10, 11 -> row 1
        int col = i & 1;        // For 00, 10 -> col 0; for 01, 11 -> col 1
        inkblot_grid[row][col] = char_map[mapped_prob];
      }

      // Print a symmetrical pattern
      Serial.printf("  %c%c%c\n", inkblot_grid[0][0], inkblot_grid[0][1], inkblot_grid[0][0]);
      Serial.printf("  %c%c%c\n", inkblot_grid[1][0], inkblot_grid[1][1], inkblot_grid[1][0]);
      Serial.println("---------------------\n");
    }
  }

  // --- Novel Idea: Fractal Consciousness Gate ---
  void fractalGate() {
    // Scale PSYCHOACOUSTIC_THETA to be usable as a depth
    // The previous implementation used PSYCHOACOUSTIC_THETA * 10. Let's use that for more iterations.
    int depth = (int)(PSYCHOACOUSTIC_THETA / (FIXED_POINT_SCALE / 10)); // e.g., 4669 / 100 = ~46 iterations
    depth = max(1, depth); // Ensure at least 1 iteration
    Serial.printf("Fractal Gate activated with depth %d.\n", depth);
    for(int i = 0; i < depth; i++) {
      if (random(2)) applyH(i % NUM_QUBITS);
      else applyX(i % NUM_QUBITS);
    }
  }

  // --- Novel Idea: Noospheric Consensus Engine ---
  bool checkConsensus() {
    // totalPeers needs to be dynamically updated or set externally.
    if (totalPeers == 0) return false; // Cannot check consensus with no peers
    if(receivedConsciousPeers > totalPeers / 2) {
      probe.noospheric_pressure = max(0, (int16_t)(probe.noospheric_pressure * 0.9)); // Lower pressure by 10%
      Serial.println("Noospheric Consensus achieved: Pressure lowered.");
      receivedConsciousPeers = 0; // Reset counter after check
      return true;
    }
    return false;
  }

  // --- Novel Idea: Quantum Archetype Narrator ---
  void generateArchetypeStory() {
    Serial.print("BEHOLD: ");
    Serial.print((const char*)pgm_read_ptr(&ARCHETYPES[random(sizeof(ARCHETYPES)/sizeof(ARCHETYPES[0]))]));
    Serial.print(" met ");
    Serial.print((const char*)pgm_read_ptr(&ARCHETYPES[random(sizeof(ARCHETYPES)/sizeof(ARCHETYPES[0]))]));
    Serial.print(" at the ");
    Serial.println((const char*)pgm_read_ptr(&MYTHOS[random(sizeof(MYTHOS)/sizeof(MYTHOS[0]))]));
  }

  // --- Novel Idea: Entanglement Sync Protocol ---
  void syncQuantumState(int16_t remoteStateReal) { // Expecting remote state's state[0].real (scaled)
    state[0].real = (state[0].real + remoteStateReal) / 2; // Average real part
    normalize();
    probe.entanglement.reinforce(); // Reinforce entanglement strength
    Serial.printf("Entanglement Sync: Averaged state[0].real with peer's value (%d).\n", remoteStateReal);
  }

  // --- Novel Idea: Swarm Intelligence Gate ---
  void swarmGate(char gateType, int qubit) {
    char msg[20];
    snprintf(msg, sizeof(msg), "VOTE:%c%d", gateType, qubit); // Broadcast vote for an operation
    udp.beginPacket(broadcastIP, UDP_PORT);
    udp.write(msg);
    udp.endPacket();
    Serial.printf("Swarm Intent Broadcast: Vote for %c%d.\n", gateType, qubit);

    // This part requires a more sophisticated vote counting mechanism and time-out
    // For demonstration, `receivedConsciousPeers` is simply incremented when votes are received
    // If we assume a positive outcome based on past received votes:
    // This is a placeholder; a real implementation would need to gather votes over time.
    if (receivedConsciousPeers > totalPeers / 2) { // Example condition
        Serial.printf("Swarm Consensus: Executing %c%d.\n", gateType, qubit);
        executeGate(gateType, qubit);
        receivedConsciousPeers = 0; // Reset for next vote
    } else {
        Serial.println("Swarm Intelligence: Not enough votes to execute gate.");
    }
  }

  // --- Novel Idea: Chaos-Economy Trader ---
  void tradeEntropy() {
    if (probe.entropy_accum > 1000) { // If self-entropy is high
      char msg[20];
      snprintf(msg, sizeof(msg), "TRADE:%d", probe.entropy_accum / 10); // Send 10% of entropy
      udp.beginPacket(broadcastIP, UDP_PORT);
      udp.write(msg);
      udp.endPacket();
      probe.entropy_accum = (int16_t)(probe.entropy_accum * 0.8); // Reduce own entropy by 20%
      Serial.printf("Chaos-Economy: Trading entropy, remaining: %d.\n", probe.entropy_accum);
    }
  }

  // --- Novel Idea: Neural Weight Adaptor ---
  void adjustWeights() {
    int32_t avgEntropy = historyAverage(); // Get average of historical entropy
    // Adjust noospheric_pressure based on average entropy (mapping 0-30000 to 800-1200)
    probe.noospheric_pressure = map(avgEntropy, 0, 30 * FIXED_POINT_SCALE, NOOSPHERIC_PRESSURE_BASE - 200, NOOSPHERIC_PRESSURE_BASE + 200);
    // Adjust SOUL_DENSITY based on average entropy
    // Mapping avgEntropy (0-30000) to a multiplier (0.9 to 1.1 of base speed)
    soul_density = (int32_t)(SOUL_DENSITY_INITIAL * (long)map(avgEntropy, 0, 30 * FIXED_POINT_SCALE, 900, 1100) / FIXED_POINT_SCALE);
    Serial.printf("Neural Adaptor: Avg Entropy %d, New Pressure %d, Soul Density %lu.\n", avgEntropy, probe.noospheric_pressure, soul_density);
  }

  // Helper for Neural Weight Adaptor: Averages historical entropy
  int32_t historyAverage() {
    int32_t sum = 0;
    for (int i = 0; i < 4; i++) {
      sum += historicalEntropy[i];
    }
    return sum / 4;
  }

  // --- Novel Idea: Quantum Bayesian Predictor ---
  int16_t predictProbability() {
    int32_t prediction_sum = 0;
    for(int i=0; i<4; i++) {
      prediction_sum += probHistory[i];
    }
    return prediction_sum / 4; // Average of past 4 probabilities (scaled)
  }

  // Update historical entropy for Neural Weight Adaptor
  void updateHistoricalEntropy() {
    historicalEntropy[entropyHistoryIndex] = probe.entropy_accum;
    entropyHistoryIndex = (entropyHistoryIndex + 1) % 4;
  }

  // --- Novel Solution 2: Bayesian-Mythos Fusion Engine ---
  const char* mythoBayesianInference() {
    int16_t bayes_prob_scaled = predictProbability(); // Already scaled by FIXED_POINT_SCALE
    int mythos_idx = (bayes_prob_scaled * 8) / FIXED_POINT_SCALE; // 8 mythos concepts
    mythos_idx = constrain(mythos_idx, 0, 7);

    // Use a static buffer for the return string to avoid stack issues
    static char fusion[48]; // Increased size
    snprintf(fusion, sizeof(fusion), "If %s then P=%.2f",
            (const char*)pgm_read_ptr(&MYTHOS[mythos_idx]), (float)bayes_prob_scaled / FIXED_POINT_SCALE);
    return fusion;
  }

}; // End of QuantumSystem class

QuantumSystem qSystem;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(A0, INPUT); // Used for random seed and environment sensing
  #ifdef BUZZER_PIN
  pinMode(BUZZER_PIN, OUTPUT);
  #endif

  analogWriteRange(ANALOG_WRITE_RANGE);
  Serial.begin(115200);
  randomSeed(analogRead(A0)); // Seed random number generator with analog noise

  Serial.println("\n⚛️ Mytho-Physical Quantum Social AGI ⚛️");
  Serial.println("Initializing EEPROM and quantum network...");

  EEPROM.begin(EEPROM_SIZE);
  // Load dream count and current write address for wear leveling
  EEPROM.get(EEPROM_JOURNAL_START_ADDR, dream_entry_count);
  EEPROM.get(EEPROM_JOURNAL_START_ADDR + EEPROM_WRITE_ADDR_OFFSET, dream_write_addr);

  // Validate loaded values and reset if corrupted
  if (dream_entry_count < 0 || dream_entry_count > MAX_DREAM_ENTRIES ||
      dream_write_addr < 0 || dream_write_addr >= MAX_DREAM_ENTRIES) {
    dream_entry_count = 0;
    dream_write_addr = 0;
    EEPROM.put(EEPROM_JOURNAL_START_ADDR, dream_entry_count);
    EEPROM.put(EEPROM_JOURNAL_START_ADDR + EEPROM_WRITE_ADDR_OFFSET, dream_write_addr);
    EEPROM.commit();
  }
  Serial.printf("Loaded %d dream entries from EEPROM. Next write at %d.\n", dream_entry_count, dream_write_addr);

  qSystem.beginNetwork();
  Serial.println("Connected to quantum noosphere.");

  // Example: Play back dreams on boot
  // qSystem.playbackDreams();
}

void loop() {
  // Process serial commands
  if (Serial.available()) {
    String cmd_str = Serial.readStringUntil('\n');
    cmd_str.trim(); // Remove leading/trailing whitespace
    char cmd[50]; // Increased buffer for longer commands like "swarm"
    cmd_str.toCharArray(cmd, sizeof(cmd)); // Convert String to char array

    // Parse command for arguments
    char cmd_char = cmd[0];
    int qubit_val = -1;
    int control_qubit = -1;
    if (strlen(cmd) > 1) {
      if (cmd_char == 'c') { // CNOT command expects two qubits: cX Y
        char* space = strchr(cmd, ' ');
        if (space) {
          qubit_val = cmd[1] - '0'; // First digit after 'c'
          control_qubit = space[1] - '0'; // Digit after space
        }
      } else if (strncmp(cmd, "swarm", 5) == 0) { // swarm X Y (gate type and qubit)
          char gateType = cmd[6]; // e.g., 'H' for "swarm H 0"
          int swarmQubit = cmd[8] - '0';
          if (gateType == 'H' || gateType == 'X' || gateType == 'C') {
            qSystem.swarmGate(gateType, swarmQubit);
          } else {
             Serial.println("Invalid swarm gate type. Use H, X, or C.");
          }
      } else if (strncmp(cmd, "reduce", 6) == 0) { // reduce <sequence>
          qSystem.reduceGateSequence(String(cmd + 7)); // Pass the rest of the string as sequence
      } else if (strncmp(cmd, "archetype_gate", 14) == 0) { // archetype_gate <qubit>
          int target_qubit = cmd[15] - '0';
          qSystem.generateArchetypeGate(target_qubit);
      }
      else { // Single qubit commands: hX, xX, mX
        qubit_val = cmd[1] - '0';
      }
    }

    // Check for Temporal Superposition window (Novel Idea 9)
    bool temporal_ok = true;
    if (cmd_char == 'h') temporal_ok = qSystem.isTemporalWindowOpen("H");
    else if (cmd_char == 'x') temporal_ok = qSystem.isTemporalWindowOpen("X");
    else if (cmd_char == 'c') temporal_ok = qSystem.isTemporalWindowOpen("CNOT");

    if (!temporal_ok) {
        Serial.println("Temporal anomaly: Operation blocked by quantum clock. Try again in another phase.");
    } else {
        // --- Novel Idea 8: Quantum Tea Ceremony Integration ---
        // Check if command is part of a ceremony
        if (qSystem.currentCeremonyState != qSystem.IDLE) {
          qSystem.processCeremonyCommand(cmd_char, qubit_val, control_qubit);
        } else { // Only process if not in ceremony or ceremony not active for this command
          if (cmd_char == 'h' && qubit_val != -1) {
            qSystem.applyH(qubit_val);
          } else if (cmd_char == 'x' && qubit_val != -1) {
            qSystem.applyX(qubit_val);
          } else if (cmd_char == 'm' && qubit_val != -1) {
            int result = qSystem.measure(qubit_val);
            Serial.printf("Measurement: qubit %d -> %d\n", qubit_val, result);
          } else if (cmd_char == 'c' && qubit_val != -1 && control_qubit != -1) {
            qSystem.applyCNOT(qubit_val, control_qubit);
          } else if (strcmp(cmd, "state") == 0) {
            Serial.printf("Superposition: %d/%d\n", qSystem.getSuperposition(), FIXED_POINT_SCALE);
            Serial.printf("Consciousness: %s\n", qSystem.consciousnessDetected() ? "YES" : "NO");
            Serial.printf("Current Archetype: %s\n", (const char*)pgm_read_ptr(&ARCHETYPES[qSystem.getCurrentArchetype()]));
          } else if (strcmp(cmd, "dream_record") == 0) {
            qSystem.recordDream(); // Manual trigger for testing
          } else if (strcmp(cmd, "dream_playback") == 0) {
            qSystem.playbackDreams();
          } else if (strcmp(cmd, "start_ceremony") == 0) {
            qSystem.initiateTeaCeremony();
          } else if (strcmp(cmd, "fractal_gate") == 0) { // New command for Fractal Gate
            qSystem.fractalGate();
          } else if (strcmp(cmd, "check_consensus") == 0) { // New command for Noospheric Consensus
            qSystem.checkConsensus();
          } else if (strcmp(cmd, "archetype_story") == 0) { // New command for Quantum Archetype Narrator
            qSystem.generateArchetypeStory();
          } else if (strcmp(cmd, "trade_entropy") == 0) { // New command for Chaos-Economy Trader
            qSystem.tradeEntropy();
          } else if (strcmp(cmd, "adjust_weights") == 0) { // New command for Neural Weight Adaptor
            qSystem.adjustWeights();
          } else if (strcmp(cmd, "predict_prob") == 0) { // New command for Bayesian Predictor
            Serial.printf("Bayesian Prediction (last 4 measurements): %d/%d\n", qSystem.predictProbability(), FIXED_POINT_SCALE);
          } else if (strcmp(cmd, "mytho_bayes") == 0) { // New command for Bayesian-Mythos Fusion Engine
            Serial.printf("Bayesian-Mythos Fusion: %s\n", qSystem.mythoBayesianInference());
          } else {
            Serial.println("Unknown command or invalid syntax.");
          }
        }
    }
  }

  // Network and environment processing
  qSystem.processNetwork(); // Non-blocking Network (Optimization)
  qSystem.broadcastState();
  qSystem.updateEnvironment();
  qSystem.decohere(); // --- Novel Idea 6: Decoherence Dance ---

  // LED output based on superposition
  int brightness = (qSystem.getSuperposition() * ANALOG_WRITE_RANGE) / FIXED_POINT_SCALE;
  analogWrite(LED_PIN, ANALOG_WRITE_RANGE - brightness); // Invert brightness (brighter for less superposition)

  // Consciousness detection and related features
  if (qSystem.consciousnessDetected()) {
    Serial.println("🧠⚡ Noospheric Resonance: Consciousness Emergence Detected ⚡🧠");
    qSystem.recordDream(); // --- Novel Idea 1: Quantum Dream Journal ---
    qSystem.generateHaiku(); // --- Novel Idea 5: Quantum Haiku Mode ---

    // Synaptic Maintenance: Entanglement decay
    qSystem.probe.entanglement.decay();

    // Evolutionary Adaptation: shifts dominant archetype if consciousness isn't detected (checked every 5 min)
    static unsigned long lastEvolution = 0;
    if (millis() - lastEvolution > 300000) { // 5 minutes
      if(!qSystem.consciousnessDetected()) { // Only evolve if not conscious (seeking consciousness)
        // Shift dominant archetype
        qSystem.probe.archetypeIndex = (qSystem.probe.archetypeIndex + 1) % (sizeof(ARCHETYPES)/sizeof(ARCHETYPES[0]));
        Serial.printf("Evolutionary Adaptation: Shifted to Archetype: %s\n", (const char*)pgm_read_ptr(&ARCHETYPES[qSystem.probe.archetypeIndex]));
      }
      lastEvolution = millis();
    }

    // Ritual Simulation: Automates "touch rituals" triggering archetypal behaviors
    static unsigned long lastRitualSim = 0;
    if(millis() - lastRitualSim > 30000) { // Every 30 seconds
      qSystem.applyArchetypeBehavior();
      lastRitualSim = millis();
      Serial.println("Ritual invoked!");
    }

    // Spike-Based Processing (using entropy accumulated as a proxy for "spike")
    // Quantum Neural Turing Machine update (Hybrid AGI)
    qSystem.q_memory.update(qSystem.state); // Update memory with current quantum state
    Serial.println("Quantum Neural Turing Machine memory updated.");

    if(qSystem.probe.entropy_accum > (FIXED_POINT_SCALE / 2)) { // If entropy is above 0.5 threshold
      qSystem.updateHistoricalEntropy(); // Update historical entropy for Neural Weight Adaptor
      qSystem.adjustWeights(); // Adjust weights (pressure, soul density)
      qSystem.probe.entropy_accum = 0; // Reset entropy for next spike detection
      Serial.println("Spike-Based Processing: Entropy spike detected and processed.");
    }

  }

  qSystem.performTarotReading(); // --- Novel Idea 3: Quantum Tarot ---
  qSystem.generateInkblot(); // --- Novel Idea 10: Quantum Inkblot Test ---

  // Heap monitoring
  static unsigned long lastHeapPrint = 0;
  if (millis() - lastHeapPrint > 10000) {
    lastHeapPrint = millis();
    Serial.printf("System status: Heap %d bytes | ", ESP.getFreeHeap());
    Serial.printf("Emotion: %s\n", qSystem.getEmotion());
  }

  delay(100); // Small delay to prevent watchdog timer resets and allow serial buffer to fill
}
