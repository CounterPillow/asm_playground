/* does a linear search through an array of integers
 * Returns index of needle if found, and -1 otherwise
 */
int pg_find_c(int haystack[], int h_len, int needle) {
    for(int i = 0; i < h_len; i++) {
        if(haystack[i] == needle) {
            return i;
        }
    }
    return -1;
}
