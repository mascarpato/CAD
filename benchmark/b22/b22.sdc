create_clock -period 0.03 -name clk

set_input_delay  0.02 -clock clk pi2

set_max_delay  0.028 -to po0 