#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * cc distaz.c -o cdistaz -lm
 *
 * Subroutine to calculate the Great Circle Arc distance
 * between two sets of geographic coordinates
 *
 * Given:  stalat => Latitude of first point (+N, -S) in degrees
 *         stalon => Longitude of first point (+E, -W) in degrees
 *         evtlat => Latitude of second point
 *         evtlon => Longitude of second point
 *
 * Returns: delta => Great Circle Arc distance in degrees
 *          az    => Azimuth from pt. 1 to pt. 2 in degrees
 *          baz   => Back Azimuth from pt. 2 to pt. 1 in degrees
 *
 * If you are calculating station-epicenter pairs, pt. 1 is the station
 *
 * Equations take from Bullen, pages 154, 155
 *
 * T. Owens, September 19, 1991
 *          Sept. 25 -- fixed az and baz calculations
 *
 * P. Crotwell, Setember 27, 1994
 *          Converted to c to fix annoying problem of fortran giving wrong
 *          answers if the input doesn't contain a decimal point.
 *
 */

int main(int argc, char *argv[])
{
    double stalat, stalon, evtlat, evtlon;
    double delta, az, baz;
    double scolat, slon, ecolat, elon;
    double a, b, c, d, e, aa, bb, cc, dd, ee, g, gg, h, hh, k, kk;
    double rhs1, rhs2, sph, rad, del, daz, dbaz, pi, piby2;

    if (argc != 5) {
        printf("Usage: distaz sta_lat sta_lon evt_lat evt_lon\n");
        printf("       Returns:  Delta Baz Az\n");
        exit(1);
    } else {
        stalat = atof(argv[1]);
        stalon = atof(argv[2]);
        evtlat = atof(argv[3]);
        evtlon = atof(argv[4]);
    }
    if ((stalat == evtlat)&&(stalon == evtlon)) {
        printf("%6.3f %6.3f %6.3f\n", 0.0, 0.0, 0.0);
        exit(0);
    }

    pi=3.141592654;
    piby2=pi/2.0;
    rad=2.*pi/360.0;
/*
 * scolat and ecolat are geocentric colatitudes as defined by Richter (pg. 318)
 *
 * Earth Flattening of 1/298.257 take from Bott (pg. 3)
 *
 */
    sph=1.0/298.257;

    scolat=piby2 - atan((1.-sph)*(1.-sph)*tan(stalat*rad));
    ecolat=piby2 - atan((1.-sph)*(1.-sph)*tan(evtlat*rad));
    slon=stalon*rad;
    elon=evtlon*rad;

/*
 * a - e are as defined by Bullen (pg. 154, Sec 10.2)
 * These are defined for the pt. 1
 */
    a=sin(scolat)*cos(slon);
    b=sin(scolat)*sin(slon);
    c=cos(scolat);
    d=sin(slon);
    e=-cos(slon);
    g=-c*e;
    h=c*d;
    k=-sin(scolat);

/* aa - ee are the same as a - e, except for pt. 2 */
    aa=sin(ecolat)*cos(elon);
    bb=sin(ecolat)*sin(elon);
    cc=cos(ecolat);
    dd=sin(elon);
    ee=-cos(elon);
    gg=-cc*ee;
    hh=cc*dd;
    kk=-sin(ecolat);

/*  Bullen, Sec 10.2, eqn. 4  */
    del=acos(a*aa + b*bb + c*cc);
    delta=del/rad;

/*
 * Bullen, Sec 10.2, eqn 7 / eqn 8
 * pt. 1 is unprimed, so this is technically the baz
 *
 * Calculate baz this way to avoid quadrant problems
 */
    rhs1=(aa-d)*(aa-d)+(bb-e)*(bb-e)+cc*cc - 2.;
    rhs2=(aa-g)*(aa-g)+(bb-h)*(bb-h)+(cc-k)*(cc-k) - 2.;
    dbaz=atan2(rhs1,rhs2);
    if (dbaz<0.0) dbaz=dbaz+2*pi;
    baz=dbaz/rad;

/*
 *  Bullen, Sec 10.2, eqn 7 / eqn 8
 *
 *  pt. 2 is unprimed, so this is technically the az
 */
    rhs1=(a-dd)*(a-dd)+(b-ee)*(b-ee)+c*c - 2.;
    rhs2=(a-gg)*(a-gg)+(b-hh)*(b-hh)+(c-kk)*(c-kk) - 2.;
    daz=atan2(rhs1,rhs2);
    if(daz<0.0) daz=daz+2*pi;
    az=daz/rad;

/* Make sure 0.0 is always 0.0, not 360. */
    if(abs(baz-360.) < .00001) baz=0.0;
    if(abs(az-360.) < .00001) az=0.0;

    printf("%6.3f %6.3f %6.3f\n", delta, baz, az);

    return 0;
}
