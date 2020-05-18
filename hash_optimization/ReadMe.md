## Function optimization based on hash-table from 1 sem
Fyrstly I ran hash-tably with: `time ./hash_table`
and it showed 2500sec, thats bad... and I rewrote some functions and add function that do pointers to words.

```char** Create_words(char *text, int* wNumber){
    assert(text != nullptr);

    char** index = (char**) calloc(500000, sizeof(index[0]));
    assert(index != nullptr);

    index[0] = text;

    int k = 0;
    while ((*text)!= '\0')
    {
        if (!(isalpha(*text) || isdigit(*text)))
        {
            *text = '\0';

            k++;
            index[k] = text + 2;

            text++;
        }
        text++;
    }

  	(*wNumber) = k + 1;

    return index;
}```
