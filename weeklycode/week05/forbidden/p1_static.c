/* WHAT NOT TO DO IN C: Using global variables inside a function */

int foobar(int x) {
    static int y = 1;
    return x + ++y;
}
