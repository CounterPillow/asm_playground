void pg_mat4x4sdiv_c(float mat[4][4], float scalar) {
    for(int row = 0; row < 4; row++) {
        for(int col = 0; col < 4; col++) {
            mat[row][col] = mat[row][col] / scalar;
        }
    }
}
