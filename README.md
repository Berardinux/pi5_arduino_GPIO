<h3>Enable HW PWM</h3>
<pre><code>sudo nano /boot/firmware/config.txt</code></pre>
<pre><code>
####################### config.txt #############################
dtoverlay=pwm-2chan,pin=12,func=4,pin2=13,func2=4
####################### config.txt #############################
</pre></code>

<h3>Reboot</h3>
<pre><code>sudo reboot now</pre></code>

<h3>Run this to run compile the program</h3>
<h5>You can change the name or main.cpp and app to whatever you would like it to be.</h5>
<pre><code>g++ main.cpp lib/arduino_gpio.cpp lib/software_pwm.cpp lib/hardware_pwm.cpp -o app -lgpiod</pre></code>
