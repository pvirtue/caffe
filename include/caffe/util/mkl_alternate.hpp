#ifndef CAFFE_UTIL_MKL_ALTERNATE_H_
#define CAFFE_UTIL_MKL_ALTERNATE_H_

#ifdef USE_MKL

#include <mkl.h>

#else  // If use MKL, simply include the MKL header

extern "C" {
#include <cblas.h>
}
#include <math.h>
#include <complex>

// Functions that caffe uses but are not present if MKL is not linked.

// A simple way to define the vsl unary functions. The operation should
// be in the form e.g. y[i] = sqrt(a[i])
#define DEFINE_VSL_UNARY_FUNC(name, operation) \
  template<typename Dtype> \
  void v##name(const int n, const Dtype* a, Dtype* y) { \
    CHECK_GT(n, 0); CHECK(a); CHECK(y); \
    for (int i = 0; i < n; ++i) { operation; } \
  } \
  inline void vs##name( \
    const int n, const float* a, float* y) { \
    v##name<float>(n, a, y); \
  } \
  inline void vd##name( \
      const int n, const double* a, double* y) { \
    v##name<double>(n, a, y); \
  } \
  inline void vc##name( \
    const int n, const std::complex<float>* a, std::complex<float>* y) { \
    v##name<std::complex<float> >(n, a, y); \
  } \
  inline void vz##name( \
    const int n, const std::complex<double>* a, std::complex<double>* y) { \
    v##name<std::complex<double> >(n, a, y); \
  }

DEFINE_VSL_UNARY_FUNC(Sqr, y[i] = a[i] * a[i]);
DEFINE_VSL_UNARY_FUNC(Exp, y[i] = exp(a[i]));
DEFINE_VSL_UNARY_FUNC(Ln, y[i] = log(a[i]));
DEFINE_VSL_UNARY_FUNC(Abs, y[i] = std::abs(a[i]));

// A simple way to define the vsl unary functions with singular parameter b.
// The operation should be in the form e.g. y[i] = pow(a[i], b)
#define DEFINE_VSL_UNARY_FUNC_WITH_PARAM(name, operation) \
  template<typename Dtype> \
  void v##name(const int n, const Dtype* a, const Dtype b, Dtype* y) { \
    CHECK_GT(n, 0); CHECK(a); CHECK(y); \
    for (int i = 0; i < n; ++i) { operation; } \
  } \
  inline void vs##name( \
    const int n, const float* a, const float b, float* y) { \
    v##name<float>(n, a, b, y); \
  } \
  inline void vd##name( \
      const int n, const double* a, const double b, double* y) { \
    v##name<double>(n, a, b, y); \
  } \
  inline void vc##name( \
    const int n, const std::complex<float>* a, const std::complex<float> b, std::complex<float>* y) { \
    v##name<std::complex<float> >(n, a, b, y); \
  } \
  inline void vz##name( \
    const int n, const std::complex<double>* a, const std::complex<double> b, std::complex<double>* y) { \
    v##name<std::complex<double> >(n, a, b, y); \
  }

DEFINE_VSL_UNARY_FUNC_WITH_PARAM(Powx, y[i] = pow(a[i], b));

// A simple way to define the vsl binary functions. The operation should
// be in the form e.g. y[i] = a[i] + b[i]
#define DEFINE_VSL_BINARY_FUNC(name, operation) \
  template<typename Dtype> \
  void v##name(const int n, const Dtype* a, const Dtype* b, Dtype* y) { \
    CHECK_GT(n, 0); CHECK(a); CHECK(b); CHECK(y); \
    for (int i = 0; i < n; ++i) { operation; } \
  } \
  inline void vs##name( \
    const int n, const float* a, const float* b, float* y) { \
    v##name<float>(n, a, b, y); \
  } \
  inline void vd##name( \
      const int n, const double* a, const double* b, double* y) { \
    v##name<double>(n, a, b, y); \
  } \
  inline void vc##name( \
    const int n, const std::complex<float>* a, const std::complex<float>* b, std::complex<float>* y) { \
    v##name<std::complex<float> >(n, a, b, y); \
  } \
  inline void vz##name( \
    const int n, const std::complex<double>* a, const std::complex<double>* b, std::complex<double>* y) { \
    v##name<std::complex<double> >(n, a, b, y); \
  }

DEFINE_VSL_BINARY_FUNC(Add, y[i] = a[i] + b[i]);
DEFINE_VSL_BINARY_FUNC(Sub, y[i] = a[i] - b[i]);
DEFINE_VSL_BINARY_FUNC(Mul, y[i] = a[i] * b[i]);
DEFINE_VSL_BINARY_FUNC(Div, y[i] = a[i] / b[i]);

// In addition, MKL comes with an additional function axpby that is not present
// in standard blas. We will simply use a two-step (inefficient, of course) way
// to mimic that.
inline void cblas_saxpby(const int N, const float alpha, const float* X,
                         const int incX, const float beta, float* Y,
                         const int incY) {
  cblas_sscal(N, beta, Y, incY);
  cblas_saxpy(N, alpha, X, incX, Y, incY);
}
inline void cblas_daxpby(const int N, const double alpha, const double* X,
                         const int incX, const double beta, double* Y,
                         const int incY) {
  cblas_dscal(N, beta, Y, incY);
  cblas_daxpy(N, alpha, X, incX, Y, incY);
}
inline void cblas_caxpby(const int N, const std::complex<float> alpha, const std::complex<float>* X,
                         const int incX, const std::complex<float> beta, std::complex<float>* Y,
                         const int incY) {
  cblas_cscal(N, reinterpret_cast<const float*>(&beta), reinterpret_cast<float*>(Y), incY);
  cblas_caxpy(N, reinterpret_cast<const float*>(&alpha), reinterpret_cast<const float*>(X),
		  incX, reinterpret_cast<float*>(Y), incY);
}
inline void cblas_zaxpby(const int N, const std::complex<double> alpha, const std::complex<double>* X,
                         const int incX, const std::complex<double> beta, std::complex<double>* Y,
                         const int incY) {
  cblas_zscal(N, reinterpret_cast<const double*>(&beta), reinterpret_cast<double*>(Y), incY);
  cblas_zaxpy(N, reinterpret_cast<const double*>(&alpha), reinterpret_cast<const double*>(X),
		  incX, reinterpret_cast<double*>(Y), incY);
}

#endif  // USE_MKL
#endif  // CAFFE_UTIL_MKL_ALTERNATE_H_
