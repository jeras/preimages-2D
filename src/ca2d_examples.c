    // read CA configuration file
    int unsigned ca [siz.y] [siz.x];
    ca2d_size_t sz1 = {siz.y-2, siz.x-2};
    int unsigned ca1 [sz1.y] [sz1.x];
    ca2d_read (filename, siz, ca);
    ca2d_print (siz, ca);
    printf ("\n");
    ca2d_forward (ca2d, siz, ca, ca1);
    ca2d_print (sz1, ca1);

    mpz_t sum;
    mpz_init (sum);
    ca2d_network (ca2d, siz, ca, sum);
    gmp_printf ("sum=%Zi\n", sum);


// TODO, this is kind of a debug function
int unsigned ca2d_forward_loop (ca2d_t ca2d, ca2d_size_t siz,
    int unsigned ai[siz.y][siz.x],
) {
    ca2d_size_t difsiz = {ca2d.ngb.y-1, ca2d.ngb.x-1};
    ca2d_size_t newsiz = {siz.y-difsiz.y, siz.x-difsiz.x};
    int unsigned newca [newsiz.y] [newsiz.x];
    // printout input CA configuration
    ca2d_print (siz, ai);
    printf ("\n");
    do {
        ca2d_forward (ca2d, oldsiz, oldca, newca);
        ca2d_print (newsiz, newca);
    } while 
    return 0;
}

//    // test conversion array <-> gmp_t
//    ca2d_size_t tsi = {4,4};
//    ca2d_size_t tso = {tsi.y-(ca2d.ngb.y-1), tsi.x-(ca2d.ngb.x-1)};
//    int unsigned nmi = (size_t) pow (ca2d.sts, tsi.y*tsi.x);
//    int unsigned nmo = (size_t) pow (ca2d.sts, tso.y*tso.x);
//    mpz_t li [nmi];
//    int unsigned lo [nmo];
//    for (int unsigned i=0; i<nmo; i++) {
//       lo [i] = 0;
//    }
//    int unsigned tai [tsi.y] [tsi.x];
//    int unsigned tao [tso.y] [tso.x];
//    mpz_t num;
//    mpz_init (num);
//    for (int unsigned i=0; i<nmi; i++) {
//        mpz_set_ui (num, i);
//        ca2d_array_from_mpz (ca2d.sts, tsi, tai, num);
////        ca2d_print (tsi, tai);
//        ca2d_forward (ca2d, tsi, tai, tao);
//        mpz_init (li[i]);
//        ca2d_array_to_mpz (ca2d.sts, tso, tao, li[i]);
//        lo [mpz_get_ui (li[i])] ++;
//    }
//    for (int unsigned i=0; i<nmo; i++) {
//        gmp_printf ("%3u --> %3u%s", i, lo[i], (i+1)%8 ? " " : "\n");
//    }
//
//    mpz_t sum [nmo];
//    for (int unsigned i=0; i<nmo; i++) {
//        mpz_set_ui (num, i);
//        ca2d_array_from_mpz (ca2d.sts, tso, tao, num);
//        mpz_init (sum[i]);
//        ca2d_network (ca2d, tso, tao, sum[i]);
//    }
//    for (int unsigned i=0; i<nmo; i++) {
//        gmp_printf ("%3u --> %Zi%s", i, sum[i], (i+1)%8 ? " " : "\n");
//    }

