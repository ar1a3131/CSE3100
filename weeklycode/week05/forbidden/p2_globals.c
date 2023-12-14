/* WHAT NOT TO DO IN C: Using global variables inside a function */

int y = 1;

int foobar(int x) {
    return x + ++y;
}
