char const alphabet[] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};

const size_t ALPHABET_LENGTH = 10;

struct Password {
    char* strVal;
    size_t* combinationIndices;
    size_t length;
};

void* initTestPassword(int length){
    struct Password* testPassword = (struct Password*) malloc(sizeof(struct Password));
    testPassword->strVal = (char*) malloc(sizeof(char) * length);
    testPassword->combinationIndices = (size_t*) malloc(length * sizeof(size_t));
    testPassword->length = length;
    for (size_t i = 0; i < length; i++){
        *(testPassword->strVal + i) = 'a';
        *(testPassword->combinationIndices + i) = 0;
    }
    return testPassword;
}

void destroyPassword(void* password){
    struct Password* p = (struct Password*) password;
    free(p->strVal);
    free(p->combinationIndices);
    free(p);
}

void incrementPassword(void* password){
    struct Password* p = (struct Password*) password;
    int i = p->length - 1;
    while(i > -1){
        (*(p->combinationIndices + i))++;
        size_t newLetterIndex = *(p->combinationIndices + i);
        char correspondingLetter = *(alphabet + newLetterIndex % ALPHABET_LENGTH);
        *(p->strVal + i) = correspondingLetter;
        if(correspondingLetter != 'a')
            break;
    }
}
