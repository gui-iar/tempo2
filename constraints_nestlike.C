#include <cmath>
#include "constraints_nestlike.h"
#include <assert.h>
#include <string.h>
#define f1yr (1.0/3.16e7)

double constraints_nestlike_red(pulsar *psr,int ipsr, int iconstraint,int iparam,int constraintk,int k,void* special){
    assert(iconstraint == constraint_red_sin || iconstraint == constraint_red_cos);

    /* Constrain if
     *   This frequency matches the constraint frequency
     * and
     *   Sin/cos matches
     */
    if (constraintk==k &&
            (
             ((iconstraint == constraint_red_sin) && (iparam == param_red_sin)) ||
             ((iconstraint == constraint_red_cos) && (iparam == param_red_cos)) )
            ) {
        double maxtspan = psr[ipsr].param[param_finish].val[0] - psr[ipsr].param[param_start].val[0];
        double RedFLow = pow(10., psr[ipsr].TNRedFLow);
        double RedAmp = pow(10.,psr[ipsr].TNRedAmp);
        double freq = RedFLow*((double)(k+1.0))/(maxtspan);
        double RedIndex = psr[ipsr].TNRedGam;
        double RedCorner = psr[ipsr].TNRedCorner/maxtspan;

        /***
         * No idea what this equation represents! Copied from LL's code MJK2016
         */
        double rho = pow((1+(pow((1.0/365.25)/RedCorner,RedIndex/2))),2)*(RedAmp*RedAmp/12.0/(M_PI*M_PI))/pow((1+(pow(freq/RedCorner,RedIndex/2))),2)/(maxtspan*24*60*60)*pow(f1yr,-3.0);

        return 1.0/sqrt(rho);
    } else return 0;

}

double constraints_nestlike_red_dm(pulsar *psr,int ipsr, int iconstraint,int iparam,int constraintk,int k,void* special){
    assert(iconstraint == constraint_red_dm_sin || iconstraint == constraint_red_dm_cos);

    /* Constrain if
     *   This frequency matches the constraint frequency
     * and
     *   Sin/cos matches
     */
    if (constraintk==k &&
            (
             ((iconstraint == constraint_red_dm_sin) && (iparam == param_red_dm_sin)) ||
             ((iconstraint == constraint_red_dm_cos) && (iparam == param_red_dm_cos)) )
            ) {
        double maxtspan = psr[ipsr].param[param_finish].val[0] - psr[ipsr].param[param_start].val[0];
        double DMAmp = pow(10.,psr[ipsr].TNDMAmp);
        double freq = ((double)(k+1.0))/(maxtspan);
        double DMIndex = psr[ipsr].TNDMGam;

        /***
         * Still no idea what this equation represents! Copied from LL's code MJK2016
         */
        double rho = (DMAmp*DMAmp)*pow(f1yr,(-3)) * pow(freq*365.25,(-DMIndex))/(maxtspan*24*60*60);

        return 1.0/sqrt(rho);
    } else return 0;

}


double constraints_nestlike_red_chrom(pulsar *psr,int ipsr, int iconstraint,int iparam,int constraintk,int k,void* special){
    assert(iconstraint == constraint_red_chrom_sin || iconstraint == constraint_red_chrom_cos);

    /* Constrain if
     *   This frequency matches the constraint frequency
     * and
     *   Sin/cos matches
     */
    if (constraintk==k &&
            (
             ((iconstraint == constraint_red_chrom_sin) && (iparam == param_red_chrom_sin)) ||
             ((iconstraint == constraint_red_chrom_cos) && (iparam == param_red_chrom_cos)) )
            ) {
        double maxtspan = psr[ipsr].param[param_finish].val[0] - psr[ipsr].param[param_start].val[0];
        double ChromAmp = pow(10.,psr[ipsr].TNChromAmp);
        double freq = ((double)(k+1.0))/(maxtspan);
        double ChromIndex = psr[ipsr].TNChromGam;
	

        /***
         * Still no idea what this equation represents! Copied from LL's code MJK2016
         */


        double rho = (ChromAmp*ChromAmp)/12./M_PI/M_PI*pow(f1yr,(-3)) * pow(freq*365.25,(-ChromIndex))/(maxtspan*24*60*60);

        return 1.0/sqrt(rho);
    } else return 0;

}


double constraints_nestlike_jitter(pulsar *psr,int ipsr, int iconstraint,int iparam,int constraintk,int k,void* special){
    assert (iconstraint == constraint_jitter);

    if (iparam == param_jitter && constraintk == k) {
        double ecorrval=0;
        const int iobs = k;
        for (int iecorr=0; iecorr < psr[ipsr].nTNECORR; iecorr++){
            for (int iflag=0;iflag < psr[ipsr].obsn[iobs].nFlags; iflag++){
                if (
                        (strcmp(psr[ipsr].obsn[iobs].flagID[iflag],
                                psr[ipsr].TNECORRFlagID[iecorr])==0)
                        && (strcmp(psr[ipsr].obsn[iobs].flagVal[iflag],
                                psr[ipsr].TNECORRFlagVal[iecorr])==0)

                   ) {
                    ecorrval = psr[ipsr].TNECORRVal[iecorr];
                    break;
                }
                if (ecorrval > 0)break;
            }
        }

	

	for (int iecorr=0; iecorr < psr[ipsr].nTNSECORR; iecorr++)
	  {
	    
	    for (int iflag=0;iflag < psr[ipsr].obsn[iobs].nFlags; iflag++){
	      if (
		  (strcmp(psr[ipsr].obsn[iobs].flagID[iflag],
			  psr[ipsr].TNSECORRFlagID[iecorr])==0)
		  && (strcmp(psr[ipsr].obsn[iobs].flagVal[iflag],
			     psr[ipsr].TNSECORRFlagVal[iecorr])==0)
		  
		  ) 
		{
		// need to add in correction here to scale with tobs
		
		
		
		double tobsval=psr[ipsr].obsn[iobs].tobs;
		
	 
	 
		ecorrval = psr[ipsr].TNSECORRVal[iecorr]/sqrt(tobsval/3600.);
		
		fprintf(stderr, "TOBS ERR TNSECORR  %.3e %.3e %.3e\n", tobsval, ecorrval,  psr[ipsr].TNSECORRVal[iecorr]); 
		break;
	      }
	      if (ecorrval > 0)break;
	    }
	  }
	    
	return 1e6/ecorrval;
	    
	
    } 
    else 
      {
	return 0;
      }
    



}

double constraints_nestlike_band(pulsar *psr,int ipsr, int iconstraint,int iparam,int constraintk,int k,void* special){
    assert(iconstraint == constraint_band_red_sin || iconstraint == constraint_band_red_cos);

    /* Constrain if
     *   This frequency matches the constraint frequency
     * and
     *   Sin/cos matches
     */
    if (constraintk==k &&
            (
             ((iconstraint == constraint_band_red_sin) && (iparam == param_band_red_sin)) ||
             ((iconstraint == constraint_band_red_cos) && (iparam == param_band_red_cos)) )
       ) {
        int iband = 0;
        int ichan = k;
        while (ichan >= psr[ipsr].TNBandNoiseC[iband]){
            ichan -= psr[ipsr].TNBandNoiseC[iband];
            ++iband;
        }

        // we are in channel ichan, band iband.



        //double BandLF = psr[ipsr].TNBandNoiseLF[iband];
        //double BandHF = psr[ipsr].TNBandNoiseHF[iband];
        double BandAmp=pow(10.0, psr[ipsr].TNBandNoiseAmp[iband]);
        double BandSpec=psr[ipsr].TNBandNoiseGam[iband];


        double maxtspan = psr[ipsr].param[param_finish].val[0] - psr[ipsr].param[param_start].val[0];
        double freq = ((double)(ichan+1.0))/(maxtspan);


        /***
         * No idea what this equation represents! Copied from LL's code MJK2016
         */

        double rho = (BandAmp*BandAmp/12.0/(M_PI*M_PI))*pow(f1yr,(-3)) * pow(freq*365.25,(-BandSpec))/(maxtspan*24*60*60);	
        return 1.0/sqrt(rho);
    } else return 0;

}


double constraints_nestlike_group(pulsar *psr,int ipsr, int iconstraint,int iparam,int constraintk,int k,void* special){
    assert(iconstraint == constraint_group_red_sin || iconstraint == constraint_group_red_cos);

    /* Constrain if
     *   This frequency matches the constraint frequency
     * and
     *   Sin/cos matches
     */
    if (constraintk==k &&
            (
             ((iconstraint == constraint_group_red_sin) && (iparam == param_group_red_sin)) ||
             ((iconstraint == constraint_group_red_cos) && (iparam == param_group_red_cos)) )
       ) {
        //int totalGroupCoeff=0;

        int igroup = 0;
        int ichan = k;
        while (ichan >= psr[ipsr].TNGroupNoiseC[igroup]){
            ichan -= psr[ipsr].TNGroupNoiseC[igroup];
            ++igroup;
        }

        // we are in channel ichan, group igroup.


        //double ret=0.0;

        double GroupAmp=pow(10.0, psr[ipsr].TNGroupNoiseAmp[igroup]);
        double GroupSpec=psr[ipsr].TNGroupNoiseGam[igroup];
        //int GroupC=psr[ipsr].TNGroupNoiseC[igroup];

        double maxtspan = psr[ipsr].param[param_finish].val[0] - psr[ipsr].param[param_start].val[0];
        double freq = ((double)(ichan+1.0))/(maxtspan);


        /***
         * No idea what this equation represents! Copied from LL's code MJK2016
         */

        double rho = (GroupAmp*GroupAmp/12.0/(M_PI*M_PI))*pow(f1yr,(-3)) * pow(freq*365.25,(-GroupSpec))/(maxtspan*24*60*60);	
        return 1.0/sqrt(rho);
    } else return 0;

}
