/*
  Backlash.cpp

  Adapted for FluidNC by kutukvpavel, 2025

  Copyright (c) 2024 Nikolaos Siatras
  Twitter: nsiatras
  Github: https://github.com/nsiatras
  Website: https://www.sourcerabbit.com

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Backlash.h"
#include "NutsBolts.h"
#include "Config.h"
#include "Machine/MachineConfig.h"
#include "Machine/Axes.h"

#define DIR_POSITIVE 0
#define DIR_NEGATIVE 1
#define DIR_NEUTRAL 2

// Private (Statics)
float Backlash::fPreviousTargets[MAX_N_AXIS] = {0.0};
uint8_t Backlash::fAxisDirections[MAX_N_AXIS] = {DIR_NEUTRAL};

/**
 * Initialize the Backlash Manager
 */
void Backlash::init()
{
    for (int i = 0; i < MAX_N_AXIS; i++)
    {
        fAxisDirections[i] = DIR_NEUTRAL;
    }
    Backlash::sync();
}

/**
 * Plans and queues a backlash motion into planner buffer
 */
void Backlash::compensate(float *target, plan_line_data_t *pl_data)
{
    float backlash_compensation_target[MAX_N_AXIS] = {0.0};
    bool perform_backlash_compensation_motion = false;

    for (int axis = 0; axis < Axes::_numberAxis; axis++)
    {
        backlash_compensation_target[axis] = fPreviousTargets[axis];

        if (Axes::_axis[axis]->_backlash > 0)
        {
            if (target[axis] > fPreviousTargets[axis])
            {
                // The new axis target is "Positive" compared to the previous one.
                // If the last axis target was "Negative" then alter the backlash_compensation_target for this axis.
                if (fAxisDirections[axis] != DIR_POSITIVE)
                {
                    backlash_compensation_target[axis] += Axes::_axis[axis]->_backlash;
                    perform_backlash_compensation_motion = true;
                }

                fAxisDirections[axis] = DIR_POSITIVE;
            }
            else if (target[axis] < fPreviousTargets[axis])
            {
                // The new axis target is "Negative" compared to the previous one.
                // If the last axis target was "Positive" then alter the backlash_compensation_target for this axis.
                if (fAxisDirections[axis] != DIR_NEGATIVE)
                {
                    backlash_compensation_target[axis] -= Axes::_axis[axis]->_backlash;
                    perform_backlash_compensation_motion = true;
                }

                fAxisDirections[axis] = DIR_NEGATIVE;
            }
        }
    }

    if (perform_backlash_compensation_motion)
    {
        // Queue the backlash motion into planner buffer
        plan_line_data_t pl_backlash_data;
        plan_line_data_t *backlash_data = &pl_backlash_data;
        memset(backlash_data, 0, sizeof(plan_line_data_t)); // Zero backlash_data struct

        backlash_data->spindle = pl_data->spindle;
        backlash_data->spindle_speed = pl_data->spindle_speed;
        backlash_data->feed_rate = pl_data->feed_rate;
        backlash_data->coolant = pl_data->coolant;
        backlash_data->motion = {};
        backlash_data->motion.backlashMotion = 1;

        while (plan_check_full_buffer()) {
            protocol_auto_cycle_start();  // Auto-cycle start when buffer is full.
    
            // While we are waiting for room in the buffer, look for realtime
            // commands and other situations that could cause state changes.
            protocol_execute_realtime();
            if (sys.abort) {
                return;
            }
        }

        // Plan and queue the backlash motion into planner buffer
        plan_buffer_line(backlash_compensation_target, &pl_backlash_data);
    }

    // Update fPreviousTargets[] values to target[] values
    memcpy(fPreviousTargets, target, sizeof(float) * Axes::_numberAxis); // target_prev[] = target[]
}

void Backlash::sync()
{
    motor_steps_to_mpos(fPreviousTargets, get_motor_steps());
}
