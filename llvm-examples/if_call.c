int call_less_than() {
  return 10;
}

int call_greater_than() {
  return 20;
}

int main(int argc, char** argv) {
  if (argc > 2) {
    return call_less_than();
  } else {
    return call_greater_than();
  }
}