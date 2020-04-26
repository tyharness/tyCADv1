
    




//simulate adding several moves as if read from file
    struct tyMov xm;
    for (int i = 3; i < 9; i++) {

	xm.key = i;
	xm.fromF = c;
	xm.fromR = i;
	xm.toF = d;
	xm.toR = i;
	t1 = insertMoveafter(xm, t1);
    }
