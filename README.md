## arduino_unbounce
Library for Arduino Software Debouncing

This library aims to enable simple software debouncing in Arduino projects. All that is required is to define the pin that should be watched, the type of transition to watch (0: pin was high, is now low, 1: pin was low, is now high) and the function that should be called if such a transition occurs.

Callbacks are only processed after a stable-time that can be tweaked when creating Unbounce (from 0 - 16ms). All readings within this time window must return the same value or the time window is reset. No more than 1 transition can occur within a tweakable time-window (from 0 - 254ms).

The library uses 10 bytes of global variables and 25 clock cycles in the loop if no transitions occur.


TL;DR: This will fire a callback if a connected button has been pressed long enough and the previous press was long enough ago.

### Quickstart

The following minimal example toggles the built-in LED each time Pin 7 is pulled low

```
#include <Unbounce.h>
void buttonDown() { // this toggles the built-in LED
 digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

Unbounce unbounce(7, 0, buttonDown); // Watch Pin 7, call "buttonDown" on transition to low (0)

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(7, INPUT_PULLUP);
}

void loop()
{
  unbounce.handle();
}
```

### METHODS

Steps required for using Unbounce
- Declare a function to be called when a transition occurs
- Create an Unbounce object and pass it at least the pin, the type of transition and the declared callback
- [Optional: activate internal pullups in your setup method]
- Call the handle() method of the created object regularly (in your main loop)

#### Unbounce (int pin, int type, void (*callback)()[, int thresholdReadingMillis, int thresholdCooldownMillis]);

Creates a new Unbounce object

| Parameter  | Default   | Description |
|-----------|-----------|-------------|
| pin      |  | Pin to watch  |
| type      |  | 0 to trigger callback on transition from high to low, 1 to trigger callback on transition from low to high |
| callback |  | function to be called when a specified transition is recognized |
| [thresholdReadingMillis] | 1 | All readings done via calls to handle() within a time window specified by this parameter have to return the same value for a transition to occur. Possible values: 0 (0ms, assume transition after first change in readings), 1 (1ms), 2 (4ms) and 3 (16ms) |
| [thresholdCooldownMillis] | 5 | A timespan defined by this parameter has to have passed before another transition is assumed. Possible values: 0 (0ms, a transition can occur in the next call to handle()), 1 (2ms), 2 (6ms), 3 (14ms), 4 (30ms), 5 (62ms), 6 (126ms) and 7 (254ms)|

Examples
```
// These examples won't work on their own, refer to the examples folder for fully working ones
/*
* This will call "someFunction" when pin 7 (PIND.7 on an Arduino UNO R3) transitions to low
* The default threshold levels require the following statements to be true to recognize a transition
* - All readings made during the handle() calls within a time window of 1ms must be of the same value
* - At least 62ms have passed since the previous transition 
*/
Unbounce unbounce(7, 0, someFunction);

/*
* This will call "anotherFunction" when pin 9 (PINB.1 on an Arduino UNO R3) transitions to high
* The threshold levels 3 and 7 require the following statements to be true to recognize a transition
* - All readings made during the handle() calls within a time window of 16ms must be of the same value
* - At least 254ms have passed since the previous transition 
*/
Unbounce unbounce(9, 1, anotherFunction, 3, 7);
```

#### void handle()
Trigger the unbounce logic, needs to be called in the main loop. Takes 25 cycles if no changes occurred.

```
// These examples won't work on their own, refer to the examples folder for fully working ones
Unbounce unbounce(7, 0, someFunction);
void loop()
{
  unbounce.handle();
}
```
