//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat May 31 08:54:58 GMT-0800 1997
// Last Modified: Sat May 31 08:54:58 GMT-0800 1997
// Filename:      ...sig/code/base/Distribution/Distribution.h
// Syntax:        C++ 
// $Smake:        cc -Wall -g -c %b.cpp -I../../../include && rm -f %b.o
//

#ifndef DISTRIBUTION_H_INCLUDED
#define DISTRIBUTION_H_INCLUDED


#define floatType    double

#define UNIFORM      (0)
#define LINEAR       (1)
#define TRIANGULAR   (2)
#define EXPONENTIAL  (3)
#define BILATERAL    (4)
#define GAUSSIAN     (5)
#define CAUCHY       (6)
#define BETA         (7)
#define WEIBULL      (8)
#define POISSON      (9)


class Distribution {
   public:
                    Distribution  (void);
                    Distribution  (int aSeed);
                   ~Distribution  ();

      void          doBeta        (void);
      void          doBeta        (floatType Avalue, floatType Bvalue);
      void          doBilateral   (void);
      void          doBilateral   (floatType aLambda);
      void          doCauchy      (void);
      void          doCauchy      (floatType aSpread, floatType aMean);
      void          doExponential (void);
      void          doExponential (floatType aLambda);
      void          doGaussian    (void);
      void          doGaussian    (floatType aDeviation, floatType aMean);
      void          doLinear      (void);
      void          doPoisson     (void);
      void          doTriangular  (void);
      void          doTriangular  (floatType alowerLimit, floatType aMax,
                                   floatType anUpperLimit);
      void          doUniform     (void);
      void          doUniform   (floatType aLowerLimit, floatType anUpperLimit);
      void          doWeibull     (void);
      void          doWeibull     (floatType t, floatType s);

      int           getType       (void);
      void          seed          (int seed);
      int           setType       (int newDistributionType);
      floatType     value         (void);
      floatType     value         (floatType min, floatType max, 
                                     int numAttempts);

   protected:
      int           seedValue;
      int           distributionType;
      int           int1, int2;         // for temporary storage space
      floatType     beta_inv_a;         // parameters for Beta distribution
      floatType     beta_inv_b;         // parameters for Beta distribution
      floatType     exponential_lambda; // parameter for Exponential dist.
      floatType     bilateral_lambda;   // parameter for Bilateral dist.
      floatType     gaussian_deviation; // parameter for Gaussian dist.
      floatType     gaussian_mean;      // parameter for Gaussian dist.
      floatType     cauchy_spread;      // parameter for Cauchy dist.
      floatType     cauchy_mean;        // parameter for Cauchy dist.
      floatType     weibull_inv_t;      // parameter for Weibull dist.
      floatType     weibull_s;          // parameter for Weibull dist.
      floatType     triangular_diffu;   // parameter for Triangular dist.
      floatType     triangular_diffl;   // parameter for Triangular dist.
      floatType     triangular_max;     // parameter for Triangular dist.
      floatType     uniform_low;        // parameter for Uniform dist.
      floatType     uniform_range;      // parameter for Uniform dist.

      // private functions:
      floatType     BetaDistribution        (void);
      floatType     BilateralDistribution   (void);
      floatType     CauchyDistribution      (void);
      floatType     ExponentialDistribution (void);
      floatType     GaussianDistribution    (void);
      floatType     LinearDistribution      (void);
      floatType     TriangularDistribution  (void);
      floatType     UniformDistribution     (void);
      floatType     WeibullDistribution     (void);

      floatType     myrand                  (void);
};



#endif  // DISTRIBUTION_H_INCLUDED



// md5sum:	b19fa7d6263d49df5e95a0d4464555ad  Distribution.h
