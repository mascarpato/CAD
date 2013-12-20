create_clock -period 0.03 -name clk

set_input_delay  0.00014 -clock clk B 

set_max_delay  0.029 -to Out 
