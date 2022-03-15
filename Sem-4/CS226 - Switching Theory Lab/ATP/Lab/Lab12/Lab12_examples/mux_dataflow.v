module mux_dataflow(out, in1,in2,sel );  //you list all inputs and outputs, by convention outputs go first
output out;                          // this tells the compile which lines are inputs and outputs 
input in1,in2,sel;

assign out=sel ? in1: in2;
endmodule
