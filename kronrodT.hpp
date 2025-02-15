#include <vector>
#include <tuple>
#include <iostream>
#include <limits>

// // void kronrod_adjust ( FloatingPoint a, FloatingPoint b, int n, FloatingPoint x[], FloatingPoint w1[], FloatingPoint w2[] );

template <typename FloatingPoint>
FloatingPoint r8_epsilon ( )

//****************************************************************************80
//
//  Purpose:
//
//    R8_EPSILON returns the R8 roundoff unit.
//
//  Discussion:
//
//    The roundoff unit is a number R which is a power of 2 with the
//    property that, to the precision of the computer's arithmetic,
//      1 < 1 + R
//    but
//      1 = ( 1 + R / 2 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    01 September 2012
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, FloatingPoint R8_EPSILON, the R8 round-off unit.
//
{
  static FloatingPoint value = std::numeric_limits<FloatingPoint>::epsilon();

  return value;
}

template <typename FloatingPoint>
std::tuple<FloatingPoint, FloatingPoint> abwe1 (int N, const FloatingPoint& eps, const FloatingPoint& coef2, const bool& even, const std::vector<FloatingPoint>& b, FloatingPoint x)

//****************************************************************************80
//
//  Purpose:
//
//    ABWE1 calculates a Kronrod abscissa and weight.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    03 August 2010
//
//  Author:
//
//    Original FORTRAN77 version by Robert Piessens, Maria Branders.
//    Original C++ version by John Burkardt.
//    Modern C++ version by Okwuchukwu Nwobi.
//
//  Reference:
//
//    Robert Piessens, Maria Branders,
//    A Note on the Optimal Addition of Abscissas to Quadrature Formulas
//    of Gauss and Lobatto,
//    Mathematics of Computation,
//    Volume 28, Number 125, January 1974, pages 135-139.
//
//  Parameters:
//
//    Input, int N, the order of the Gauss rule.
//
//    Input, int M, the value of ( N + 1 ) / 2.
//
//    Input, FloatingPoint EPS, the requested absolute accuracy of the
//    abscissas.
//
//    Input, FloatingPoint COEF2, a value needed to compute weights.
//
//    Input, bool EVEN, is TRUE if N is even.
//
//    Input, FloatingPoint B[M+1], the Chebyshev coefficients.
//
//    Input/output, FloatingPoint *X; on input, an estimate for
//    the abscissa, and on output, the computed abscissa.
//
//    Output, FloatingPoint *W, the weight.
//
{
  FloatingPoint ai;
  FloatingPoint b0;
  FloatingPoint b1;
  FloatingPoint b2;
  FloatingPoint d0;
  FloatingPoint d1;
  FloatingPoint d2;
  FloatingPoint delta;
  FloatingPoint dif;
  FloatingPoint f;
  FloatingPoint fd;
  FloatingPoint yy;

  int M = ( N + 1 ) / 2;
  int i;
  int iter;
  int k;
  int ka;

  if ( x == 0.0 )
  {
    ka = 1;
  }
  else
  {
    ka = 0;
  }
//
//  Iterative process for the computation of a Kronrod abscissa.
//
  for ( iter = 1; iter <= 50; iter++ )
  {
    b1 = 0.0;
    b2 = b[M];
    yy = 4.0 * (x) * (x) - 2.0;
    d1 = 0.0;

    if ( even )
    {
      ai = M + M + 1;
      d2 = ai * b[M];
      dif = 2.0;
    }
    else
    {
      ai = M + 1;
      d2 = 0.0;
      dif = 1.0;
    }

    for ( k = 1; k <= M; k++ )
    {
      ai = ai - dif;
      i = M - k + 1;
      b0 = b1;
      b1 = b2;
      d0 = d1;
      d1 = d2;
      b2 = yy * b1 - b0 + b[i-1];
      if ( !even )
      {
        i = i + 1;
      }
      d2 = yy * d1 - d0 + ai * b[i-1];
    }

    if ( even )
    {
      f = ( x ) * ( b2 - b1 );
      fd = d2 + d1;
    }
    else
    {
      f = 0.5 * ( b2 - b0 );
      fd = 4.0 * ( x ) * d2;
    }
//
//  Newton correction.
//
    delta = f / fd;
    x = x - delta;

    if ( ka == 1 )
    {
      break;
    }

    if ( fabs ( delta ) <= eps )
    {
      ka = 1;
    }
  }
//
//  Catch non-convergence.
//
  if ( ka != 1 )
  {
    std::cout << "\n";
    std::cout << "ABWE1 - Fatal error!\n";
    std::cout << "  Iteration limit reached.\n";
    std::cout << "  EPS is " << eps << "\n";
    std::cout << "  Last DELTA was " << delta << "\n";
    exit ( 1 );
  }
//
//  Computation of the weight.
//
  d0 = 1.0;
  d1 = x;
  ai = 0.0;
  for ( k = 2; k <= N; k++ )
  {
    ai = ai + 1.0;
    d2 = ( ( ai + ai + 1.0 ) * ( x ) * d1 - ai * d0 ) / ( ai + 1.0 );
    d0 = d1;
    d1 = d2;
  }


  FloatingPoint w = coef2 / ( fd * d2 );

  return std::make_tuple(x, w);
}

template <typename FloatingPoint>
std::tuple<FloatingPoint, FloatingPoint, FloatingPoint> abwe2 (int N, const FloatingPoint& eps, const FloatingPoint& coef2, const bool& even, const std::vector<FloatingPoint>& b, FloatingPoint x)

//****************************************************************************80
//
//  Purpose:
//
//    ABWE2 calculates a Gaussian abscissa and two weights.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    30 April 2013
//
//  Author:
//
//    Original FORTRAN77 version by Robert Piessens, Maria Branders.
//    Original C++ version by John Burkardt.
//    Modern C++ version by Okwuchukwu Nwobi.
//
//  Reference:
//
//    Robert Piessens, Maria Branders,
//    A Note on the Optimal Addition of Abscissas to Quadrature Formulas
//    of Gauss and Lobatto,
//    Mathematics of Computation,
//    Volume 28, Number 125, January 1974, pages 135-139.
//
//  Parameters:
//
//    Input, int N, the order of the Gauss rule.
//
//    Input, int M, the value of ( N + 1 ) / 2.
//
//    Input, FloatingPoint EPS, the requested absolute accuracy of the
//    abscissas.
//
//    Input, FloatingPoint COEF2, a value needed to compute weights.
//
//    Input, bool EVEN, is TRUE if N is even.
//
//    Input, FloatingPoint B[M+1], the Chebyshev coefficients.
//
//    Input/output, FloatingPoint *X; on input, an estimate for
//    the abscissa, and on output, the computed abscissa.
//
//    Output, FloatingPoint *W1, the Gauss-Kronrod weight.
//
//    Output, FloatingPoint *W2, the Gauss weight.
//
{
  FloatingPoint ai;
  FloatingPoint an;
  FloatingPoint delta;
  FloatingPoint p0;
  FloatingPoint p1;
  FloatingPoint p2;
  FloatingPoint pd0;
  FloatingPoint pd1;
  FloatingPoint pd2;
  FloatingPoint yy;

  int i;
  int iter;
  int k;
  int ka;
  int M = ( N + 1 ) / 2;
  if ( x == 0.0 )
  {
    ka = 1;
  }
  else
  {
    ka = 0;
  }
//
//  Iterative process for the computation of a Gaussian abscissa.
//
  for ( iter = 1; iter <= 50; iter++ )
  {
    p0 = 1.0;
    p1 = x;
    pd0 = 0.0;
    pd1 = 1.0;
//
//  When N is 1, we need to initialize P2 and PD2 to avoid problems with DELTA.
//
    if ( N <= 1 )
    {
      if ( r8_epsilon<FloatingPoint> ( ) < fabs ( x ) )
      {
        p2 = ( 3.0 * ( x ) * ( x ) - 1.0 ) / 2.0;
        pd2 = 3.0 * ( x );
      }
      else
      {
        p2 = 3.0 * ( x );
        pd2 = 3.0;
      }
    }

    ai = 0.0;
    for ( k = 2; k <= N; k++ )
    {
      ai = ai + 1.0;
      p2 = ( ( ai + ai + 1.0 ) * (x) * p1 - ai * p0 ) / ( ai + 1.0 );
      pd2 = ( ( ai + ai + 1.0 ) * ( p1 + (x) * pd1 ) - ai * pd0 ) 
        / ( ai + 1.0 );
      p0 = p1;
      p1 = p2;
      pd0 = pd1;
      pd1 = pd2;
    }
//
//  Newton correction.
//
    delta = p2 / pd2;
    x = x - delta;

    if ( ka == 1 )
    {
      break;
    }

    if ( fabs ( delta ) <= eps )
    {
      ka = 1;
    }
  }
//
//  Catch non-convergence.
//

  if ( ka != 1 )
  {
    std::cout << "\n";
    std::cout << "ABWE2 - Fatal error!\n";
    std::cout << "  Iteration limit reached.\n";
    std::cout << "  EPS is " << eps << "\n";
    std::cout << "  Last DELTA was " << delta << "\n";
    exit ( 1 );
  }
//
//  Computation of the weight.
//
  an = N;

  FloatingPoint w2 = 2.0 / ( an * pd2 * p0 );

  p1 = 0.0;
  p2 = b[M];
  yy = 4.0 * (x) * (x) - 2.0;
  for ( k = 1; k <= M; k++ )
  {
    i = M - k + 1;
    p0 = p1;
    p1 = p2;
    p2 = yy * p1 - p0 + b[i-1];
  }
  FloatingPoint w1;
  if ( even )
  {
    w1 = w2 + coef2 / ( pd2 * (x) * ( p2 - p1 ) );
  }
  else
  {
    w1 = w2 + 2.0 * coef2 / ( pd2 * ( p2 - p0 ) );
  }

  return std::make_tuple(x, w1, w2);
}


template <typename FloatingPoint>
std::tuple<std::vector<FloatingPoint>, std::vector<FloatingPoint>, std::vector<FloatingPoint>> kronrod (int N, const FloatingPoint& eps)

//****************************************************************************80
//
//  Purpose:
//
//    KRONROD adds N+1 points to an N-point Gaussian rule.
//
//  Discussion:
//
//    This subroutine calculates the abscissas and weights of the 2N+1
//    point Gauss Kronrod quadrature formula which is obtained from the 
//    N point Gauss quadrature formula by the optimal addition of N+1 points.
//
//    The optimally added points are called Kronrod abscissas.  The 
//    abscissas and weights for both the Gauss and Gauss Kronrod rules
//    are calculated for integration over the interval [-1,+1].
//
//    Since the quadrature formula is symmetric with respect to the origin,
//    only the nonnegative abscissas are calculated.
//
//    Note that the code published in Mathematics of Computation 
//    omitted the definition of the variable which is here called COEF2.
//
//  Storage:
//
//    Given N, let M = ( N + 1 ) / 2.  
//
//    The Gauss-Kronrod rule will include 2*N+1 points.  However, by symmetry,
//    only N + 1 of them need to be listed.
//
//    The arrays X, W1 and W2 contain the nonnegative abscissas in decreasing
//    order, and the weights of each abscissa in the Gauss-Kronrod and
//    Gauss rules respectively.  This means that about half the entries
//    in W2 are zero.
//
//    For instance, if N = 3, the output is:
//
//    I      X               W1              W2
//
//    1    0.960491        0.104656         0.000000   
//    2    0.774597        0.268488         0.555556    
//    3    0.434244        0.401397         0.000000
//    4    0.000000        0.450917         0.888889
//
//    and if N = 4, (notice that 0 is now a Kronrod abscissa)
//    the output is
//
//    I      X               W1              W2
//
//    1    0.976560        0.062977        0.000000   
//    2    0.861136        0.170054        0.347855    
//    3    0.640286        0.266798        0.000000   
//    4    0.339981        0.326949        0.652145    
//    5    0.000000        0.346443        0.000000
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    02 September 2014
//
//  Author:
//
//    Original FORTRAN77 version by Robert Piessens, Maria Branders.
//    Original C++ version by John Burkardt.
//    Modern C++ version by Okwuchukwu Nwobi.
//
//  Reference:
//
//    Robert Piessens, Maria Branders,
//    A Note on the Optimal Addition of Abscissas to Quadrature Formulas
//    of Gauss and Lobatto,
//    Mathematics of Computation,
//    Volume 28, Number 125, January 1974, pages 135-139.
//
//  Parameters:
//
//    Input, int N, the order of the Gauss rule.
//
//    Input, FloatingPoint EPS, the requested absolute accuracy of the
//    abscissas.
//
//    Output, FloatingPoint X[N+1], the abscissas.
//
//    Output, FloatingPoint W1[N+1], the weights for the Gauss-Kronrod rule.
//
//    Output, FloatingPoint W2[N+1], the weights for 
//    the Gauss rule.
//
{
  std::vector<FloatingPoint> x(N+1);
  std::vector<FloatingPoint> w1(N+1);
  std::vector<FloatingPoint> w2(N+1);

//   return std::make_tuple(x, w1, w2);
  FloatingPoint ak;
  FloatingPoint an;
  FloatingPoint bb;
  FloatingPoint c;
  FloatingPoint coef;
  FloatingPoint coef2;
  FloatingPoint d;
  bool even;
  int i;
  int k;
  int l;
  int ll;
  FloatingPoint s;
  FloatingPoint x1;
  FloatingPoint xx;
  FloatingPoint y;

  int m = ( N + 1 ) / 2;

  std::vector<FloatingPoint> b(m+1);
  std::vector<FloatingPoint> tau(m); 
  // std::array<FloatingPoint, N+1> x, w1, w2;
  
  
  even = ( 2 * m == N );

  d = 2.0;
  an = 0.0;
  for ( k = 1; k <= N; k++ )
  {
    an = an + 1.0;
    d = d * an / ( an + 0.5 );
  }
//
//  Calculation of the Chebyshev coefficients of the orthogonal polynomial.
//
  tau[0] = ( an + 2.0 ) / ( an + an + 3.0 );
  b[m-1] = tau[0] - 1.0;
  ak = an;

  for ( l = 1; l < m; l++ )
  {
    ak = ak + 2.0;
    tau[l] = ( ( ak - 1.0 ) * ak 
      - an * ( an + 1.0 ) ) * ( ak + 2.0 ) * tau[l-1] 
      / ( ak * ( ( ak + 3.0 ) * ( ak + 2.0 ) 
      - an * ( an + 1.0 ) ) );
    b[m-l-1] = tau[l];

    for ( ll = 1; ll <= l; ll++ )
    {
      b[m-l-1] = b[m-l-1] + tau[ll-1] * b[m-l+ll-1];
    }
  }

  b[m] = 1.0;
//
//  Calculation of approximate values for the abscissas.
//
  bb = sin ( 1.570796 / ( an + an + 1.0 ) );
  x1 = sqrt ( 1.0 - bb * bb );
  s = 2.0 * bb * x1;
  c = sqrt ( 1.0 - s * s );
  coef = 1.0 - ( 1.0 - 1.0 / an ) / ( 8.0 * an * an );
  xx = coef * x1;
//
//  Coefficient needed for weights.
//
//  COEF2 = 2^(2*N+1) * N! * N! / (2n+1)! 
//        = 2 * 4^N * N! / product( (N+1)*...*(2*N+1))
//
  coef2 = 2.0 / FloatingPoint(2 * N + 1);
  for ( i = 1; i <= N; i++ )
  {
    coef2 = coef2 * 4.0 * FloatingPoint( i ) / FloatingPoint(N + i);
  }
//
//  Calculation of the K-th abscissa (a Kronrod abscissa) and the
//  corresponding weight.
//
  for ( k = 1; k <= N; k = k + 2 )
  {
    FloatingPoint w1k, w2k;
    std::tie(xx, w1k) = abwe1<FloatingPoint>(N, eps, coef2, even, b, xx);

    w1[k-1] = w1k;
    w2[k-1] = 0.0;

    x[k-1] = xx;
    y = x1;
    x1 = y * c - bb * s;
    bb = y * s + bb * c;

    if ( k == N )
    {
      xx = 0.0;
    }
    else
    {
      xx = coef * x1;
    }
//
//  Calculation of the K+1 abscissa (a Gaussian abscissa) and the
//  corresponding weights.
//
    std::tie(xx, w1k, w2k) = abwe2<FloatingPoint>(N, eps, coef2, even, b, xx);

    w1[k] = w1k;
    w2[k] = w2k;

    x[k] = xx;
    y = x1;
    x1 = y * c - bb * s;
    bb = y * s + bb * c;
    xx = coef * x1;
  }
//
//  If N is even, we have one more Kronrod abscissa to compute,
//  namely the origin.
//
  if ( even )
  {
    xx = 0.0;
    FloatingPoint w1n;
    std::tie(xx, w1n) = abwe1<FloatingPoint>(N, eps, coef2, even, b, xx);

    w1[N] = w1n;
    w2[N] = 0.0;
    x[N] = xx;
  }

  return std::make_tuple(x, w1, w2);
}
void timestamp ( );
