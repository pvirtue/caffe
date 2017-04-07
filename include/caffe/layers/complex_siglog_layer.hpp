#ifndef CAFFE_COMPLEX_SIGLOG_LAYER_HPP_
#define CAFFE_COMPLEX_SIGLOG_LAYER_HPP_

#include <vector>

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"

#include "caffe/layers/complex_layer.hpp"

namespace caffe {

    
template <typename Dtype>
class ComplexSiglogLayer : public ComplexLayer<Dtype> {
 public:
  explicit ComplexSiglogLayer(const LayerParameter& param)
      : ComplexLayer<Dtype>(param) {}
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

  virtual inline const char* type() const { return "ComplexSiglog"; }

 protected:

  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  // virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
  //     const vector<Blob<Dtype>*>& top);


  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);
  // virtual void Backward_gpu(const vector<Blob<Dtype>*>& top,
  //     const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);

  ///parameters s,d,r,c
  Dtype d;
  Dtype s;
  Dtype r;
  Dtype c;



};

}  // namespace caffe

#endif  // CAFFE_COMPLEX_SIGLOG_LAYER_HPP_
