/*!
 * @file 		stitcher_module.cpp
 * @author 		
 * @date 		11.2.2013
 * @date		16.2.2013
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under modified BSD Licence, details in file doc/LICENSE
 *
 */

#include "stitcher_module.h"
#include "yuri/core/Module.h"
#include "yuri/core/frame/RawVideoFrame.h"
#include "yuri/core/frame/raw_frame_types.h"
#include "yuri/core/frame/raw_frame_params.h"


namespace yuri {
namespace stitcher_module {

IOTHREAD_GENERATOR(stitcher_module)

MODULE_REGISTRATION_BEGIN("stitcher_module")
		REGISTER_IOTHREAD("stitcher_module",stitcher_module)
MODULE_REGISTRATION_END()

// So we can write log[info] instead of log[log::info]
using namespace yuri::log;

core::Parameters stitcher_module::configure()
{
	core::Parameters p = base_type::configure();
	p.set_description("stitcher_module");
	p["GPU"]["GPU flag (0/1 - allow GPU usage)"]=1;
	p["Warper"]["Warper settings (0 - CylindricalWarper / 1 - PlaneWarper / 2 - SphericalWarper)"]=0;
    p["Blender"]["Use blender to blend the images (0/1 - allow Blender)"]=0;
    p["Exposure_compensate"]["(0/1 - compensate exposure)"]=0;
    p["cam_count"]["Number of cameras"]=2;
//	p->set_max_pipes(-1,1);
	return p;
}


stitcher_module::stitcher_module(log::Log &log_, core::pwThreadBase parent, const core::Parameters &parameters):
base_type(log_,parent,1,1,std::string("stitcher_module")),GPU_(1),Warper_(0),cam_count_(2),Blender_(0),Exposure_compensate_(0)
{
	IOTHREAD_INIT(parameters)
	resize(cam_count_,1);
}

stitcher_module::~stitcher_module() noexcept
{
}

std::vector<core::pFrame> stitcher_module::do_single_step(std::vector<core::pFrame> framesx)
{

    if (!stitcherInit){
        stitcher = cv::Stitcher::createDefault(GPU_);

       	if (GPU_){

			log[log::warning] << "using GPU";

    		stitcher.setFeaturesMatcher(cv::makePtr<cv::detail::BestOf2NearestMatcher>(true));

            if (Exposure_compensate_){
                
                stitcher.setExposureCompensator(cv::makePtr<cv::detail::BlocksGainCompensator>());
            }
            else {
                stitcher.setExposureCompensator(cv::makePtr<cv::detail::NoExposureCompensator>());
            }

            if (Blender_)
                stitcher.setBlender(cv::makePtr<cv::detail::MultiBandBlender>(true));
            else
                stitcher.setBlender(cv::detail::Blender::createDefault(cv::detail::Blender::NO, true));
    	
    		if(Warper_ == 0){
    			stitcher.setWarper(cv::makePtr<cv::CylindricalWarperGpu>());
    		}
    		else if (Warper_ == 1){
    			stitcher.setWarper(cv::makePtr<cv::PlaneWarperGpu>());
    		}
    		else if (Warper_ == 2){
    			stitcher.setWarper(cv::makePtr<cv::SphericalWarperGpu>());
    		}

    		stitcher.setSeamFinder(cv::makePtr<cv::detail::GraphCutSeamFinderGpu>());
       	}else {


            if (Exposure_compensate_){
                stitcher.setExposureCompensator(cv::makePtr<cv::detail::BlocksGainCompensator>());
            }
            else {
                stitcher.setExposureCompensator(cv::makePtr<cv::detail::NoExposureCompensator>());
            }

            if (Blender_)
                stitcher.setBlender(cv::makePtr<cv::detail::MultiBandBlender>(true));
            else
                stitcher.setBlender(cv::detail::Blender::createDefault(cv::detail::Blender::NO, true));

       		if(Warper_ == 0)
    		  stitcher.setWarper(cv::makePtr<cv::CylindricalWarper>());
    		else if (Warper_ == 1)
    		  stitcher.setWarper(cv::makePtr<cv::PlaneWarper>());
    		else if (Warper_ == 2)
    		  stitcher.setWarper(cv::makePtr<cv::SphericalWarper>());
       	}

        log[log::warning] << "Stitcher Init complete.";

        stitcherInit = true;
    }


	const format_t format = framesx[0]->get_format();

	const auto& fi = core::raw_format::get_format_info(format);

	const size_t frames_no = framesx.size();

	if (fi.planes.size() != 1) {
		log[log::warning] << "Planar formats not supported";
		return {};
	}

	std::vector<core::pRawVideoFrame> frames;
	for (auto& x: framesx) {
		auto f = std::dynamic_pointer_cast<core::RawVideoFrame>(x);
		if (!f) {
			log[log::warning] << "Received non-raw frame.";
			return {};
		}
		frames.push_back(f);
	}

	for (size_t i=1;i<frames_no;++i) {
		if (frames[i]->get_format() != format) {
			log[log::warning] << "Wrong format for frame in pipe " << i;
			//frames[i].reset();
			return {};
		}
	}

	std::vector <cv::Mat> matFrames;
    std::vector<std::vector<cv::Rect>> rois;
    //std::cout << frames_no << std::endl;

    for (size_t i=0;i<frames_no;++i) {
       	resolution_t res = frames[i]->get_resolution();
        cv::Mat in_mat(res.height,res.width,CV_8UC3,PLANE_RAW_DATA(frames[i],0));
        matFrames.push_back (in_mat);
    }

    if (!haveTransform){
        //std::cout << "pictures size : " << matFrames.size() << std::endl;

        int stat  = stitcher.estimateTransform(matFrames);
        
        if (stat != 0){
            log[log::warning] << "Can't stitch images, error code = " << stat;
            return {};
        }else{
            haveTransform = true;
            cams = stitcher.cameras();
        }
    }

    cv::Mat pano;
    stitcher.setCameras(cams);
    
    cv::Stitcher::Status status = stitcher.composePanorama(matFrames, pano, GPU_);
    
    if (status != cv::Stitcher::OK)
            {
                log[log::warning] << "Can't stitch images, error code = " << int(status);
                haveTransform = false;
            }

    core::pFrame frame = core::RawVideoFrame::create_empty(core::raw_format::rgb24,
					{static_cast<dimension_t>(pano.cols), static_cast<dimension_t>(pano.rows)},
					pano.data,
					pano.total() * pano.elemSize());

	return {frame};
}
bool stitcher_module::set_param(const core::Parameter& param)
{
	if (assign_parameters(param)
			(GPU_, "GPU")
			(Warper_, "Warper")
            (Blender_, "Blender")
            (Exposure_compensate_, "Exposure_compensate")
            (cam_count_,"cam_count"))
		return true;
	return base_type::set_param(param);
}

} /* namespace dummy_module */
} /* namespace yuri */

