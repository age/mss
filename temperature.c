/* Copyright 2009 Evgeny Grablyk <evgeny.grablyk@gmail.com>
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <onewire.h>
#include <ds18x20.h>
#include <temperature.h>

uint8_t gSensorIDs[MAXTS][OW_ROMCODE_SIZE], nSensors;

uint8_t
search_sensors (void)
{
  uint8_t i;
  uint8_t id[OW_ROMCODE_SIZE];
  uint8_t diff, nSensors;

  nSensors = 0;

  for (diff = OW_SEARCH_FIRST; diff != OW_LAST_DEVICE && nSensors < MAXTS;)
    {
      DS18X20_find_sensor (&diff, &id[0]);

      if (diff == OW_PRESENCE_ERR)
        {
          break;
        }

      if (diff == OW_DATA_ERR)
        {
          break;
        }

      for (i = 0; i < OW_ROMCODE_SIZE; i++)
        gSensorIDs[nSensors][i] = id[i];

      nSensors++;
    }
  return nSensors;

}

void
start_meas (void)
{
  DS18X20_start_meas (DS18X20_POWER_EXTERN, NULL);
}

int16_t
read_meas (int ns)
{
  uint8_t subzero, cel, cel_frac_bits;

  /* _delay_ms (DS18B20_TCONV_12BIT); */
  /* might fail */
  /* _delay_ms (5); */
  DS18X20_read_meas_single (gSensorIDs[ns][0], &subzero, &cel,
                            &cel_frac_bits);

  return DS18X20_temp_to_decicel (subzero, cel, cel_frac_bits);
}
