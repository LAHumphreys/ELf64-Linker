int isYes(char c) {
    if ( c == 'y' || c == 'Y') {
        return 0;
    } else {
        return 1;
    }
}

int main(int argc, char* argv[])
{
    return isYes(argv[1][0]);
}
