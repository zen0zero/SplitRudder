# SplitRudder
Small project to add drag/differential rudder to flying wing RC plane.

I wanted to provide my custom-built flying wing with Z-axis stabilization, so I needed to somehow control yaw axis.
Inspired by the [B2 bomber](https://it.wikipedia.org/wiki/Northrop_Grumman_B-2_Spirit), I proceeded with designing two small split airbrakes, each with its servo.

This result could be easily achieved with a simple mix on the RC transmitter, but I chose not to because:
 * I am using a very cheap transmitter (with great results, actually), the [Turnigy TGY-i6](https://hobbyking.com/it_it/turnigy-tgy-i6-afhds-transmitter-and-6ch-receiver-mode-2.html).<p>
 Radio is great -- mix configuration a little less so, and it only supports three of them
 * the idea was to later install on the plane some sort of electronic stabilization (gyro/accel) so I wanted to expose the yaw axis as a single RC channel

So I set off to write this small program, which takes
 * RC rudder input on a pin
 * another pwm input to signal both left and right surfaces to open the same amount (a common airbrake, since the surfaces are already there why not use them to gain more insight on their effect?)

and outputs inverse PWM signals (1000->2000 usecs on one side, 2000->1000 usecs on the other) to drive the two servos using the standard library.

As they say, it works for me, YMMV, use at your own risk.

Have fun and stay away from propellers :)
