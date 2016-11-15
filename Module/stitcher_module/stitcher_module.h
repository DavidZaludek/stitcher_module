/*!
 * @file 		stitcher_module.h
 * @author 		Zdenek Travnicek
 * @date		7.3.2013
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under modified BSD Licence, details in
 *file doc/LICENSE
 *
 */

#ifndef stitcher_module_H_
#define stitcher_module_H_

#include "yuri/core/thread/MultiIOFilter.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "stitching.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace yuri {
namespace stitcher_module {

class stitcher_module : public core::MultiIOFilter {
  using base_type = core::MultiIOFilter;

 public:
  IOTHREAD_GENERATOR_DECLARATION
  static core::Parameters configure();
  stitcher_module(log::Log &log_, core::pwThreadBase parent,
                  const core::Parameters &parameters);
  virtual ~stitcher_module() noexcept;

 private:
  virtual std::vector<core::pFrame> do_single_step(
      std::vector<core::pFrame> frames) override;
  virtual bool set_param(const core::Parameter &param);
  size_t GPU_, Warper_, Blender_ , Exposure_compensate_ ,cam_count_;

  cv::Stitcher stitcher;
  std::vector<cv::detail::CameraParams> cams;
  bool stitcherInit = false;
  bool haveTransform = false;
};

} /* namespace stitcher_module */
} /* namespace yuri */
#endif /* DUMMYMODULE_H_ */
