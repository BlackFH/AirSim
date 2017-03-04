// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_vehicles_Board3DFly_hpp
#define msr_airlib_vehicles_Board3DFly_hpp

#include "vehicles/MultiRotorParams.hpp"

namespace msr { namespace airlib {

class Board3DFly : public MultiRotorParams {
protected:
    virtual void setup(Params& params, SensorCollection& sensors, unique_ptr<DroneControllerBase>& controller) override
    {
        //set up arm lengths
        //dimensions are for F450 frame: http://artofcircuits.com/product/quadcopter-frame-hj450-with-power-distribution
        params.rotor_count = 4;
        std::vector<real_T> arm_lengths(params.rotor_count, 0.2275f);

        //set up mass
        params.mass = 1.0f; //can be varied from 0.800 to 1.600
        real_T motor_assembly_weight = 0.055f;  //weight for MT2212 motor for F450 frame
        real_T box_mass = params.mass - params.rotor_count * motor_assembly_weight;

        //set up dimensions of core body box
        params.body_box.x = 0.180f; params.body_box.y = 0.11f; params.body_box.z = 0.040f;
        real_T rotor_z = 2.5f / 100;

        params.enabled_sensors.barometer = false;
        params.enabled_sensors.gps = false;
        params.enabled_sensors.magnetometer = false;
        params.enabled_sensors.imu = true;
        
        //computer rotor poses
        initializeRotorPoses(params.rotor_poses, params.rotor_count, arm_lengths.data(), rotor_z);
        //compute inertia matrix
        computeInertiaMatrix(params.inertia, params.body_box, params.rotor_poses, box_mass, motor_assembly_weight);
        //create sensors
        createStandardSensors(sensors, params.enabled_sensors);

        //leave everything else to defaults
    }
};

}} //namespace
#endif
