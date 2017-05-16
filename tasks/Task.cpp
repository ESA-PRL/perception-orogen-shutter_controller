#include "Task.hpp"
#include <shutter_controller/ShutterController.hpp>

using namespace shutter_controller;

Task::Task(std::string const& name)
    : TaskBase(name), periodCounter(0)
{
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine)
    : TaskBase(name, engine)
{
}

Task::~Task()
{
}

bool Task::configureHook()
{
    if (! TaskBase::configureHook())
        return false;
    const shutter_controller::Config config = _config.get();
    shutterCtrl = new shutter_controller::ShutterController(config);
    shutterCompPeriod = config.shutterCompPeriod;
    stop=false;

    return true;
}
bool Task::startHook()
{
    if (! TaskBase::startHook())
        return false;
    return true;
}
void Task::updateHook()
{
    TaskBase::updateHook();
    
    if ((periodCounter++)%shutterCompPeriod != 0)
        return;

    if (_estop.connected())
    {
        if (_estop.read(stop) == RTT::NewData)
            std::cout << " Stop shutter : " << stop << std::endl;
    }

    if (_frame.read(frame) == RTT::NewData)
    {
        // analyze input frame
        int res = shutterCtrl->analyze( frame->getImage() );

        // compute new shutter time
        int newShutterTime = shutterCtrl->calcNewShutterTime( res );

        if (!stop)
        {
            // output shutter time
            _shutter_value.write( newShutterTime );
        }

        /* operations approach
        // get operation pointers
        RTT::TaskContext* pancam_left = getPeer("pancam_left");
        //RTT::TaskContext* pancam_right = getPeer("pancam_right");
        RTT::OperationCaller<bool(const camera::int_attrib::CamAttrib,int)> setLeftIntAttrib = pancam_left->getOperation("setIntAttrib");
        //RTT::OperationCaller<bool(const double,int)> setLeftIntAttrib = pancam_left->getOperation("setIntAttrib");
        //RTT::OperationCaller<bool(camera::int_attrib::CamAttrib,int)> setRightIntAttrib = pancam_right->getOperation("setIntAttrib");

        // set shutter time on left and right camera
        bool res_left = setLeftIntAttrib(camera::int_attrib::ShutterValue, newShutterTime);
        bool res_right = setLeftIntAttrib(camera::int_attrib::ShutterValue, newShutterTime);

        // wait for results?

        // success?
        if (res_left && res_right)
        {
            std::cout << "successfully set new shutter time" << std::endl;
        }
        */
    }
}

void Task::errorHook()
{
    TaskBase::errorHook();
}
void Task::stopHook()
{
    TaskBase::stopHook();
}
void Task::cleanupHook()
{
    TaskBase::cleanupHook();
    delete shutterCtrl;
}
