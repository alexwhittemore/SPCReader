# SPCReader

SPCReader is a sketch written for the Trinket M0 implementing a USB data capture cable for Mitutoyo and iGaging SPC calipers.

## Operation

The sketch appears as a keyboard. When the button is pressed, the screen contents are typed in followed by < enter >. For instance, if the screen measures "134.05" in mm, a button press will simply type "134.05\n", advancing to the next line or spreadsheet cell.

At least for the iGaging OriginCal with fractional inch readout, in fractional mode, the calipers simply return decimal inches.

The value returned is directly from the screen, and contains no information about units or whether the measurement is relative.

Additionally, the sketch flashes the onboard LED from green to red when taking a reading.

If the cable gets unplugged or the calipers turn off, the sketch will stall on red while attempting to read.

Only one reading will be typed per button press.

## Hardware Definition

A standard Mitutoyo SPC cable with keyed female 2x5 0.1" female pin header pluggs into the following Male pins. This is the exact cable I use personally: [amazon](https://www.amazon.com/gp/product/B00027957U/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)

Male 2x5 pin header, looking top down.
|   |   |KEY|   |   |
|-------|-------|---------|--------|----------|
| 1: NC | 3: NC | 5: /REQ | 7: CLK | 9: GND   |
| 2: NC | 4: NC | 6: NC   | 8: BTN | 10: DATA |

The signals above are connected as follows:

| SPC Cable | TrinketM0 |
|-----------|-----------|
| /REQ      | 1         |
| BTN       | 2         |
| CLK       | 3         |
| DATA      | 4         |
| GND       | Gnd       |

