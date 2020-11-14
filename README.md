# SpotWelderController

## What it is
SpotWelderController is a firmware for spot welders used in building li-ion battery packs for e-bikes, e-scooters, uniwheels, hoverboards etc.
The firmware works on arduino boards like Uno, Nano or Pro mini. It should also work on other plaftorms like Arduino Mega.

## How it works
Main schematic about how SpotWelderController works is shown below.

![Main Diagram](docs/MainDiagram.svg?raw=true "MainDiagram")

SpotWelderController after pushing TRIG key sends two pulses on TRIGGER line. These pulses activate thyristors and high-power tranformer which causes spot welding on electrodes. Between the pulses is a time gap. The second pulse is optional and it is possible to weld with only one pulse. The resolution of the pulses width and the time gap is 1ms. 

In order to limit the shock current, transformer activation should be done at maxium voltage of sine. To do that, additional signal SYNC is used for zero crossing detecion. After detecting zero crossing SpotWelderController waits 5ms for maximum volatage of the sine and then sends a pulse on the TRIGGER line.

Parameters of the pulses and the gap are shown on the oled display and can be easily changed with only one SET button. In addition, SpotWelderController measures continuously high-power transformer temperature and shows on the display.

## Hardware

**High voltage is dangerous. Before operate make sure that you have appropriate skills and equipment.**


There is no dedicated hardware for SpotWelderController. You have to build entire device yourself. Type of thyristors depends on your high-power tranformer. In most cases, high-power transformer will be retrived from old microwave oven. Such transformer has power about 700-800W. You have to remove secondary windings and rewind 3-5 turns with thick cable (e.g. 25mm2). The thicker cable the better result. Sample driving circuit with zero crossing detector can look like below.

![Sample driving circuit](docs/SpotWelderTrigger.svg?raw=true "SpotWelderTrigger")
