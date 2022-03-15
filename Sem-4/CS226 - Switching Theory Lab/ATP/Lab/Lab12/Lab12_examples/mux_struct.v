 
module mux_struct(out, in1,in2,sel );  //you list all inputs and outputs, by convention outputs go first
output out;                          // this tells the compile which lines are inputs and outputs 
input in1,in2,sel;

and a1 (a1_o,in1,sel);         // defines the a1 gate, see figure
not n1 (inv_sel,sel);         // defines the inverter gate, see figure
and a2 (a2_o,in2,sel);         // defines the a2 gate, see figure
or  o1 (out, a1_o, a2_o);         // defines the 01 gate, see figure

endmodule
