#include "Task.hpp"

using namespace shutter_controller;

Task::Task(std::string const& name)
    : TaskBase(name), period_counter(0)
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
    shutter_controller = new shutter_controller::ShutterController(config);
    shutter_computation_period = config.shutterCompPeriod;
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
    
    period_counter++;
    if (period_counter % shutter_computation_period != 0)
        return;

    if (_estop.connected())
    {
        if (_estop.read(stop) == RTT::NewData)
            std::cout << " Stop shutter : " << stop << std::endl;
    }

    if (_frame.read(frame) == RTT::NewData)
    {
        int res = shutter_controller->analyze(frame->getImage());
        int new_shutter_time = shutter_controller->calcNewShutterTime(res);
        if (!stop)
        {
            _shutter_value.write(new_shutter_time);
        }
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
    delete shutter_controller;
}
