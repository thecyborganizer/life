#define CELLS 30

typedef enum{
  dead,
  died,
  born,
  alive
} state;

state cur_gen[CELLS];
state next_gen[CELLS];

#define NUM_REPEATS 6
long last_patterns[NUM_REPEATS];
int last_index = 0;

bool is_alive(state s) {
  return (s == born || s == alive);
}

bool is_dead(state s) {
  return (s == died || s == dead);
}

state update_cell(int pos, state cells[], int len) {
  state s = cells[pos];
  int alive_neighbors = 0;
  if (pos > 1) {
    alive_neighbors += is_alive(cells[pos - 2]);
  }
  if (pos > 0) {
    alive_neighbors += is_alive(cells[pos - 1]);
  }
  if (len - pos > 2) {
    alive_neighbors += is_alive(cells[pos + 2]);
  }
  if (len - pos > 1) {
    alive_neighbors += is_alive(cells[pos + 1]);
  }

  // the cell is alive, does it stay alive?
  if (is_alive(s)) {
    if (alive_neighbors == 2 || alive_neighbors == 4) {
      return alive;
    } else {
      return died;
    }
  } else {
    if (alive_neighbors == 2 || alive_neighbors == 3) {
      return born;
    } else {
      return dead;
    }
  }

}

void create_life(state a[], int len) {
  for (int i = 0; i < len; i++) {
    if (random(2)) {
      a[i] = born;
    }
  }
}

bool check_pattern(state cells[], int len) {
  long new_pattern = 0UL;
  int alive_count = 0;
  bool to_return = false;
//  for (int i = 0; i < len; i++){
//    Serial.print(cells[i]);
//    Serial.print(" ");
//  }
//  Serial.println();
  for (int i = 0; i < len; i++){
    if (is_alive(cells[i])) {
      new_pattern = new_pattern ^ (1UL << i);
      alive_count++;
    }
  }
  for (int i = 0; i < NUM_REPEATS; i++) {
    if (last_patterns[i] == new_pattern) {
      to_return = true;
    }
  }

  last_patterns[last_index] = new_pattern;
  last_index = (last_index + 1) % NUM_REPEATS;
  return to_return;
}

void reset_patterns(long p[], int len) {
  for (int i = 0; i < len; i++){
    p[i] = 0;
  }
}

