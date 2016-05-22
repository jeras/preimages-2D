
    // memory allocation for preimage network
    mpz_t net [siz.y+1] [siz.x+1] [ovl];

    // initialize array
    for (int unsigned y=0; y<=siz.y; y++) {
        for (int unsigned x=0; x<=siz.x; x++) {
            for (int unsigned o=0; o<ovl; o++) {
                // TODO: for now only a unit weight edge is supported unsigned int *
                mpz_init2 (net [y] [x] [o], btn);
            }
        }
    }

    // set boundary value
    for (int unsigned y=0; y<=siz.y; y++) {
        for (int unsigned o=0; o<ovl; o++) {
            mpz_set_ui (net [y] [0] [o], 1);
        }
    }
    for (int unsigned x=0; x<=siz.x; x++) {
        for (int unsigned o=0; o<ovl; o++) {
            mpz_set_ui (net [0] [x] [o], 1);
        }
    }


    // temporary structure
    const int unsigned ovl_x  = pow (sts, (ngb.x-1)*(ngb.y  )  );
    const int unsigned ovl_y  = pow (sts, (ngb.x  )*(ngb.y-1)  );
    const int unsigned ovl_z  = pow (sts, (ngb.x-1)*(ngb.y-1)  );

    // pointers to X/Y/Z(xy) dimension edges
    int unsigned pix [ovl], pox [ovl];
    int unsigned piy [ovl], poy [ovl];
    int unsigned piz [ovl], poz [ovl];

    int unsigned pts [ovl][sts], pto [ovl][sts];

    for (int unsigned o=0; o<ovl; o++) {
        int unsigned n;
        int unsigned a [ngb.y] [ngb.x];

        size2D_t     s0 = {0, 0}; 
        size2D_t     s1 = {1, 1}; 

        size2D_t     sx = {ngb.y, ngb.x-1}; 
        int unsigned ax [sx.y] [sx.x];

        size2D_t     sy = {ngb.y-1, ngb.x}; 
        int unsigned ay [sy.y] [sy.x];

        size2D_t     sz = {ngb.y-1, ngb.x-1}; 
        int unsigned az [sz.y] [sz.x];

        // input pointers
        n = o * sts;
        ca2d_array_from_ui (sts, ngb, a, n);
        ca2d_array_slice (ngb, (size2D_t) {0, 1}, (size2D_t) {0+sx.y, 1+sx.x}, a, ax);
        ca2d_array_to_ui (sts, sx, ax, &pix[o]);
        ca2d_array_slice (ngb, (size2D_t) {1, 0}, (size2D_t) {1+sy.y, 0+sy.x}, a, ay);
        ca2d_array_to_ui (sts, sy, ay, &piy[o]);
        ca2d_array_slice (ngb, (size2D_t) {1, 1}, (size2D_t) {1+sz.y, 1+sz.x}, a, az);
        ca2d_array_to_ui (sts, sz, az, &piz[o]);
        printf ("pointers i: ");
        ca2d_array_print (ngb, a); printf (" :: ");
        ca2d_array_print (sx, ax); printf (" -> pix[%u] = %u | ", o, pix[o]);
        ca2d_array_print (sy, ay); printf (" -> piy[%u] = %u | ", o, piy[o]);
        ca2d_array_print (sz, az); printf (" -> piz[%u] = %u | ", o, piz[o]);
        printf ("\n");

        // output pointers
        n = o;
        ca2d_array_from_ui (sts, ngb, a, n);
        ca2d_array_slice (ngb, (size2D_t) {0, 0}, sx, a, ax);
        ca2d_array_to_ui (sts, sx, ax, &pox[o]);
        ca2d_array_slice (ngb, (size2D_t) {0, 0}, sy, a, ay);
        ca2d_array_to_ui (sts, sy, ay, &poy[o]);
        ca2d_array_slice (ngb, (size2D_t) {0, 0}, sz, a, az);
        ca2d_array_to_ui (sts, sz, az, &poz[o]);
        printf ("pointers o: ");
        ca2d_array_print (ngb, a); printf (" :: ");
        ca2d_array_print (sx, ax); printf (" -> pox[%u] = %u | ", o, pox[o]);
        ca2d_array_print (sy, ay); printf (" -> poy[%u] = %u | ", o, poy[o]);
        ca2d_array_print (sz, az); printf (" -> poz[%u] = %u | ", o, poz[o]);
        printf ("\n");

        // transcribe pointers
        n = o;
        for (int unsigned s=0; s<sts; s++) {
            ca2d_array_from_ui (sts, ngb, a, n);
            a [ngb.y-1] [ngb.x-1] = s;
            printf (" ");
            ca2d_array_print (ngb, a);
            ca2d_array_to_ui (sts, ngb, a, &pts[o][s]);
            pto[o][s] = pts[o][s] / sts;
            printf (" pts[%u][%u]=%u, pto[%u][%u]=%u", o, s, pts[o][s], o, s, pto[o][s]);
        }
        printf ("\n");
    }

    // compute network weights
    // weights for X/Y/Z dimension edges
    mpz_t wx [ovl_x];
    mpz_t wy [ovl_y];
    mpz_t wz [ovl_z];
    mpz_t w;
    for (int unsigned o=0; o<ovl_x; o++)  mpz_init2 (wx[o], btn);
    for (int unsigned o=0; o<ovl_y; o++)  mpz_init2 (wy[o], btn);
    for (int unsigned o=0; o<ovl_z; o++)  mpz_init2 (wz[o], btn);
    mpz_init (w);

    for (int unsigned y=1; y<=siz.y; y++) {
        for (int unsigned x=1; x<=siz.x; x++) {
            // initialize temporary result array/structure
            for (int unsigned o=0; o<ovl_x; o++)  mpz_set_ui (wx[o], 0);
            for (int unsigned o=0; o<ovl_y; o++)  mpz_set_ui (wy[o], 0);
            for (int unsigned o=0; o<ovl_z; o++)  mpz_set_ui (wz[o], 0);
            // construct temporary result array/structure
            for (int unsigned o=0; o<ovl; o++) {
                mpz_add (wx [pix[o]], wx [pix[o]], net [y-1] [x  ] [o]);
                mpz_add (wy [piy[o]], wy [piy[o]], net [y  ] [x-1] [o]);
                mpz_add (wz [piz[o]], wz [piz[o]], net [y-1] [x-1] [o]);
            }
            gmp_printf ("net[%u][%u] :: ", y, x);
            for (int unsigned o=0; o<ovl; o++) {
                mpz_mul (w, wy [poy[o]], wx [pox[o]]);
                if (mpz_sgn (w)) {
                    mpz_divexact (w, w, wz [poz[o]]);
                    for (int unsigned s=0; s<sts; s++) {
                        if (tab [pts[o][s]] == ca [y-1] [x-1]) {
                        gmp_printf ("sw (%u,%Zi,%u) pts=%u ", s, w, mpz_sgn (w), pts[o][s]);
                            mpz_add (net [y] [x] [pto[o][s]], net [y] [x] [pto[o][s]], w);
                        }
                    }
                }
                gmp_printf ("xyzw (%Zi,%Zi,%Zi,%Zi) ", wx [pox[o]], wy [poy[o]], wz [poz[o]], w);
            }
            gmp_printf ("\n");
        }
    }

    // printout preimage network weights
    printf ("network\n");
    for (int unsigned y=0; y<=siz.y; y++) {
        for (int unsigned x=0; x<=siz.x; x++) {
            printf (" [");
            for (int unsigned o=0; o<ovl; o++) {
                gmp_printf ("%s%Zi", o ? "," : "", net [y] [x] [o]);
            }
            printf ("]");
        }
        printf ("\n");
    }

