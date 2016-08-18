#include <vector>

#include "caffe/layers/complex_igaussian_layer.hpp"

namespace caffe {

template <typename Dtype>
void ComplexIGaussianLayer<Dtype>::Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  CHECK_EQ(2, bottom[0]->shape(-1));

  vector<int> shape = bottom[0]->shape();

  shape.pop_back(); // Drop complex dimension for top shape
  top[0]->Reshape(shape);
}

template <typename Dtype>
void ComplexIGaussianLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
  const std::complex<Dtype>* bottom_data = this->RealToComplexBottomData_cpu(bottom, 0);
  Dtype* top_data = top[0]->mutable_cpu_data();

  const int count = top[0]->count();

  for (int i = 0; i < count; ++i) {
    top_data[i] = 1-std::real(std::exp(-std::conj(bottom_data[i])*bottom_data[i]/(2*this->sigmaSq)));
  }

  this->SyncComplexTopData_cpu(top, 0);
}

template <typename Dtype>
void ComplexIGaussianLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob<Dtype>*>& bottom) {
  if (propagate_down[0]) {
    const Dtype* top_data = top[0]->cpu_data();
    const Dtype* top_diff = top[0]->cpu_diff();
    const std::complex<Dtype>* bottom_data = this->RealToComplexBottomData_cpu(bottom, 0);
    std::complex<Dtype>* bottom_diff = this->RealToComplexBottomDiff_mutable_cpu(bottom, 0);

    const int count = top[0]->count();

    for (int i = 0; i < count; ++i) {
      bottom_diff[i] = top_diff[i]*(1-top_data[i])*bottom_data[i]/(2*this->sigmaSq);
    }

    this->SyncComplexBlobDiff_cpu(0);
  }
}


// TODO Complex Uncomment when we implement Forward_gpu and Backward_gpu
//#ifdef CPU_ONLY
//STUB_GPU(ComplexIGaussianLayer);
//#endif

INSTANTIATE_CLASS(ComplexIGaussianLayer);
REGISTER_LAYER_CLASS(ComplexIGaussian);

}  // namespace caffe
