double form_volume(double radius_lg);

double Iq(double q, 
          double sld_lg, double sld_sm, double sld_solvent,
          double volfraction_lg, double volfraction_sm, double surf_fraction,
          double radius_lg, double radius_sm, double penetration);

double Iqxy(double qx, double qy,
          double sld_lg, double sld_sm, double sld_solvent,
          double volfraction_lg, double volfraction_sm, double surf_fraction,
          double radius_lg, double radius_sm, double penetration);

double form_volume(double radius_lg)
{
    //Because of the complex structure, volume normalization must
    //happen in the Iq code below.  Thus the form volume is set to 1.0 here
    double volume=1.0;
    return volume;
}

double Iq(double q,
          double sld_lg, double sld_sm, double sld_solvent,
          double volfraction_lg, double volfraction_sm, double surface_fraction,
          double radius_lg, double radius_sm, double penetration)
{
    // Ref: J. coll. inter. sci. (2010) vol. 343 (1) pp. 36-41.


    double vfL, rL, sldL, vfS, rS, sldS, deltaS, delrhoL, delrhoS, sldSolv, aSs;
    double VL, VS, Np, f2, fSs;
    double psiL,psiS;
    double sfLS,sfSS,Np2,fSs2;
    double slT;
 
    vfL = volfraction_lg;
    rL = radius_lg;
    sldL = sld_lg;
    vfS = volfraction_sm;
    fSs = surface_fraction;
    rS = radius_sm;
    sldS = sld_sm;
    deltaS = penetration;
    sldSolv = sld_solvent;
    
    delrhoL = fabs(sldL - sldSolv);
    delrhoS = fabs(sldS - sldSolv); 
     
    VL = M_4PI_3*rL*rL*rL;
    VS = M_4PI_3*rS*rS*rS;

    Np = vfS*fSs*VL/vfL/VS;

    slT = delrhoL*VL + Np*delrhoS*VS;

    sfLS = sph_j1c(q*rL)*sph_j1c(q*rS)*sinc(q*(rL+deltaS*rS));
    sfSS = sph_j1c(q*rS)*sph_j1c(q*rS)*sinc(q*(rL+deltaS*rS))*sinc(q*(rL+deltaS*rS));
        
    f2 = delrhoL*delrhoL*VL*VL*sph_j1c(q*rL)*sph_j1c(q*rL); 
    f2 += Np*delrhoS*delrhoS*VS*VS*sph_j1c(q*rS)*sph_j1c(q*rS);
    f2 += Np*(Np-1)*delrhoS*delrhoS*VS*VS*sfSS;
    f2 += 2*Np*delrhoL*delrhoS*VL*VS*sfLS;
    if (f2 != 0.0){
        f2 = f2/slT/slT;
        }

    f2 = f2*(vfL*delrhoL*delrhoL*VL + vfS*fSs*Np*delrhoS*delrhoS*VS);

    f2+= vfS*(1.0-fSs)*pow(delrhoS, 2)*VS*sph_j1c(q*rS)*sph_j1c(q*rS);
    
    // normalize to single particle volume and convert to 1/cm
    f2 *= 1.0e8;        // [=] 1/cm
    f2 *= 1.0e-12;      // convert for (1/A^-6)^2 to (1/A)^2
    
    return f2;
}



double Iqxy(double qx, double qy,
          double sld_lg, double sld_sm, double sld_solvent,
          double volfraction_lg, double volfraction_sm, double surf_fraction,
          double radius_lg, double radius_sm, double penetration)
          
{
    double q = sqrt(qx*qx + qy*qy);
    return Iq(q,
          sld_lg, sld_sm, sld_solvent,
          volfraction_lg, volfraction_sm, surf_fraction,
          radius_lg, radius_sm, penetration);

}
