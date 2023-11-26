#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"

void func_done(int a) { /*nothing*/ }

int main(void) {
    int a = 42;
    func_done(a);
    return 0;
}
