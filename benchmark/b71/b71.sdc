create_clock -period 0.03 -name clk

set_input_delay  0 -clock clk y 
set_input_delay  0 -clock clk x 
set_input_delay  0 -clock clk cin 

set_max_delay  0.03 -to s 
set_max_delay  0.03 -to cout 
