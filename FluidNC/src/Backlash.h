/*
  Backlash.h

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

#pragma once

#include "Planner.h"

class Backlash
{
public:
  static void init();
  static void compensate(float *target, plan_line_data_t *pl_data);

  static void sync();

private:
  static float fPreviousTargets[];
  static uint8_t fAxisDirections[];
};
